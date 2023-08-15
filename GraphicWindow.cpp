#include "GraphicWindow.h"

int top;

std::vector<int> numbers;
std::vector<std::vector<int>> factors;
std::vector<std::vector<int>> exponents;

void loadData(const std::string& filePath)
{
numbers.clear();
factors.clear();
exponents.clear();

// regex pattern
std::regex pattern(R"((\d+):\s*((?:\d+\^\d+\s*\*\s*)*\d+\^\d+)$)");

// opens the file to read
std::ifstream file(filePath);

// all of the lines will be stored here
std::vector<std::string> lines;

// current line
std::string line;

// reads back all of the lines
while (std::getline(file, line))
    {
    // adding the line to the stream
    lines.push_back(line);
    }

// closes file
file.close();

for(std::string s : lines)
    {
    // arrays
    std::vector<int> facs;
    std::vector<int> exps;

    // gets all of the matches of the pattern
    std::smatch match;

    if (std::regex_match(s, match, pattern))
        {
        // group 1 matches are the numbers
        int number = std::stoi(match[1]);

        // splits at the multiplication sign
        std::string factorString = match[2];

        int startPos = 0;
        int foundPos = 0;

        // there are x + 1 numbers
        int numcount = (int)std::count(factorString.begin(), factorString.end(), '*') + 1;

        int a = 0;
        int b = 0;

        int c = 0;

        for(int i = 0; i < numcount; i++)
            {
            foundPos = (factorString.find("*", 0) != std::string::npos) ? factorString.find("*", 0) : -1;

            // substring the string
            factorString = factorString.substr(startPos, factorString.length()-startPos);  

            if(foundPos == -1)
                {   
                // get back the base and the exponent
                sscanf(factorString.c_str(), "%d^%d", &a, &b);
                
                // push them to their respective queues
                facs.push_back(a);
                exps.push_back(b);
                }
            else
                {
                // get back the base and the exponent
                sscanf(factorString.c_str(), "%d^%d", &a, &b);
                
                // push them to their respective queues
                facs.push_back(a);
                exps.push_back(b);
                }

            // the new start pos will be the old found pos + 1
            startPos = foundPos + 1;
            }
        
        // adds all of them to the respective global variables
        numbers.push_back(number);
        factors.push_back(facs);
        exponents.push_back(exps);
        }
    }
}

void GraphicWindow::add(Shape* shape)
{
shapes.push_back(shape);
}

void GraphicWindow::paintEvent(QPaintEvent* event)
{    
QPainter p(this);
p.setRenderHint(QPainter::Antialiasing);

for (Shape* s : shapes)
    {
    s->draw(&p);
    }
};

void GraphicWindow::mousePressEvent(QMouseEvent* event)
{
int px = event->pos().rx();
int py = event->pos().ry();

if(event->button() == Qt::RightButton)
    {
    for(int i = 0; i < shapes.size(); i++)
        {
        // checking the shapes are lines
        if(shapes[i]->shapeType != Shape::ShapeType::LINE) continue;
        
        Line* l = (Line*)shapes[i];

        // 0 is the enum ID for shape (IF CHANGED HERE WILL BE SOURCE OF ERROR)
        vec2 s = l->getSource(), t = l->getTarget();
        float dst = pointLineDist((vec2){(float)px, (float)py}, s, t);

        // if it is within the bounds it is being clicked
        if(dst <= l->getWidth())
            {
            shapes.erase(shapes.begin() + i);
            update();
            }
        
        }
    
    }

// if it's a left click say it's pressed so that it can be used elsewhere
if(event->button() == Qt::LeftButton)
    pressed = 1;
};

void GraphicWindow::mouseMoveEvent(QMouseEvent* event)
{
// early return if nothing is being pressed at the same time
if(pressed != 1)
    return;

int px = event->pos().rx();
int py = event->pos().ry();

// before setting a new active deselect the old and set its colour back
if(active != nullptr && prevcol != "")
    {
    active->revertColour();
    active = nullptr;
    }

// for each shape in shapes
for(Shape* s : shapes)
    {
    // check the click is in the shape
    if(s->bbox.contains(px, py))
        {
        
        // if it is then set the shape as active and moving
        s->moving = 1;
        active = s;
        }
    }


// if there is something active
if(active != nullptr)
    {
    // and the active thing is moving
    if(active->moving == 1)
        {
        // get the position of the mouse
        int x = event->pos().rx();
        int y = event->pos().ry();
        
        // move the active item to the mouse's position
        active->move({(float)x, (float)y});

        // redraw
        update();
        }
    }
};

void GraphicWindow::mouseReleaseEvent(QMouseEvent* event)
{
// getting the position of the release
int px = event->pos().rx();
int py = event->pos().ry();

// checking if there is an active object
if(active != nullptr)
    {
    // checking the active object is where the mouse was released and that something is actually moving
    if(active->bbox.contains(px, py) && active->moving == 1)
        {            
        // set the shape as inactive and stop it moving
        active->moving = 0;
        active = nullptr;
        }
    }

pressed = 0;
};

void GraphicWindow::mouseDoubleClickEvent(QMouseEvent* event) 
{
// change if right click is needed
if(event->button() != Qt::LeftButton) return;

// getting the position of the click
int px = event->pos().rx();
int py = event->pos().ry();

// flag to show if the box contains the pointer
int cont = 0;

// clicked shape
Shape* clicked = nullptr;

// going through all of the node's to check if they are valid options
for (Shape* s : shapes)
    {
    // if the click is on a box
    if(s->bbox.contains(px, py))
        {
        clicked = s;
        cont = 1;
        }
    }

// if the click wasn't in a node deselect and return
if(cont == 0)
    {
    if(active != nullptr)
        {        
        // change it's colour back
        active->revertColour();

        // remove the node's shape from active
        active = nullptr;

        // redraw
        update();
        }

    return;
    }

// if there is already an active node and it isn't the clicked one
if(active != nullptr && active != clicked)
    {
    // create a new edge between the active and the newly selected
    Line* l = new Line(active, clicked, 2);

    // checing it's a node
    if(active->shapeType == Shape::ShapeType::NODE)
        {
        // power string to add (equal to "" in case neither are prime or both are prime)
        std::string power;

        // get both labels
        std::string lab1 = ((Node*)active)->getText();
        std::string lab2 = ((Node*)clicked)->getText();

        // convert them to ints
        int ilab1 = atoi(lab1.c_str());
        int ilab2 = atoi(lab2.c_str());

        // check see if it is a factor or a number
        int primeTest1 = isPrime(ilab1);
        int primeTest2 = isPrime(ilab2);

        if((primeTest1 == 1 && primeTest2 == 1) || (primeTest1 == 0 && primeTest2 == 0)) goto end;

        // if the first is prime find the power it should be using
        if(primeTest1 == 1)
            power = ((Node*)clicked)->preemptPower(lab1);
        // if the second is prime find the power again
        else if (primeTest2 == 1)
            power = ((Node*)active)->preemptPower(lab2);
        
        if(power == "") goto end;

        if(power == "1") power = "";

        // set the text
        l->setText(power);
        }

    // add the new edge
    add(l);

    end:

    // change the colour of the active shape back
    active->revertColour();

    // make the shape inactive
    active = nullptr;

    // redraw
    update();
    }
// if there isn't an active node select the clicked one
else if(active == nullptr)
    {
    // get the current colour to store
    prevcol = clicked->getColour();

    // change the colour to grey to indicate it has been selected
    clicked->changeColour("grey");

    // set the node's shape as the active shape
    active = clicked;
    }
else
    {   
    // change colour back
    active->revertColour();

    // remove the node's shape from active
    active = nullptr;
    }

update();
};

Shape* GraphicWindow::getShape(int index)
{
return shapes[index];
}

MainWindow::MainWindow(QWidget* parent, GraphicWindow* graphicWindow)
    : QMainWindow(parent), gwin(graphicWindow)
{
// menu bar setup
menubar = new QMenuBar(this);
setMenuBar(menubar);

// add the main menu
fileMen = new QMenu("FILE");
menubar->addMenu(fileMen);

// setting up the main widget
dockWidget = new QDockWidget("Side pannel", this);

// creating its' content widget
dockContent = new QWidget(dockWidget);

// setting the docking widget's content up
dockWidget->setWidget(dockContent);

// setting up the layout of the widget
dockinglayout = new QVBoxLayout(dockContent);

// add the widget to the window
addDockWidget(Qt::RightDockWidgetArea, dockWidget);

table = new Table(dockContent);
table->resizeColumnsToContents();
table->resizeRowsToContents();

// button setup
button = new QPushButton("Load new set", dockContent);

// the width of the button's text
int wid = fontMetrics().horizontalAdvance("  Load new set  ");

// setting the width and height of the button
button->setFixedSize(wid, 30);

// setting the dock layout
dockContent->setLayout(dockinglayout);

// setting the minimum size to be the button's width so that it is always readable
dockContent->setMinimumWidth(table->width());

// adding both widgets to the dock
dockinglayout->addWidget(button);
dockinglayout->addWidget(table);

// setting the main widget of the dock
dockWidget->setWidget(dockContent);

// connecting the button to the window so that it can be used
connect(button, &QPushButton::clicked, this, pressy);
};

Table::Table(QWidget* parent)
    : QTableWidget(parent)
{
// setting the rows and the columns
this->setRowCount(3);
this->setColumnCount(2);
}

void Table::add(vec2 tablePos, std::string item)
{
QTableWidgetItem* element = new QTableWidgetItem(item.c_str());

// row count
int rc = rowCount();

// if the row count is less than or equal to the position trying to write to
if(rc <= tablePos.x) 
    {
    // add a new row
    setRowCount(rc+1);
    }

// setting the new item
setItem(tablePos.x, tablePos.y, element);

// updating the table
update();

// adding the element to the list
tableItems.push_back(element);
}

int loadnew(GraphicWindow* gwin, Table* table)
{
static int curRow = 0;

Node* number = new Node({50, 50}, std::to_string(numbers[top]), 14, "light green");
gwin->add(number);

// adding the number to the table
table->add({(float)curRow, 0}, std::to_string(numbers[top]));

int count = 0;

for (int i = 0; i < factors[top].size(); i++)
    {
    std::cout << '\n' << factors[top][i] << "^" << exponents[top][i];

    // adding the factors to the vector
    number->addFactor(factors[top][i], exponents[top][i]);
    }


// a collection of the factors
std::stringstream ss;

for (int i : factors[top])
    {
    std::string strnum = std::to_string(i);

    Node* n = new Node({(float)(100 + count*50), (float)50}, strnum, 14, "light blue");
    ss << strnum << ", ";
    
    gwin->add(n);

    count++;
    }

// the stream as a string
std::string compactStr = ss.str();

// the final string -2 as it gets rid of the extra ", "
std::string final = compactStr.substr(0, compactStr.length()-2);

table->add({(float)curRow, 1}, final);

curRow++;
top++;

return 0;
}

void MainWindow::pressy()
{
std::cout << "\n\nClicky Clicky";

// loads the new nodes
loadnew(this->gwin, this->table);
update();
}

int CpMain()
{
int h = 0;
QApplication app(h, {});
MainWindow* window = new MainWindow();

loadData("Composite.txt");

app.setActiveWindow(window);

GraphicWindow gwin;
window->gwin = &gwin;

window->setWindowTitle("Graphy");
window->setCentralWidget(&gwin);

window->setGeometry(200, 200, 800, 800);

window->show();


int result = app.exec();

return result;
}