#include "GraphicWindow.h"

void GraphicWindow::add(Shape* shape)
{
shapes.push_back(shape);
}

void GraphicWindow::paintEvent(QPaintEvent* event)
{    
QPainter p(this);
p.setRenderHint(QPainter::Antialiasing);

// draw the box select if selecting
if(boxSelecting == 1)
    {
    boxSel->draw(&p);
    }

// drawing the shapes backward so the oldest stuff gets drawn on the top
for (int i = shapes.size()-1; i > -1; i--)
    {
    shapes[i]->draw(&p);
    }

// draws forward
/* for (Shape* s : shapes)
    {
    s->draw(&p);
    } */
};

void GraphicWindow::mousePressEvent(QMouseEvent* event)
{
// pointer pos vec2 for functions
pPos = getPointerPos(event);

// the extra range for the line clicking
int padding = 1;

// the index of the shape clicked on
int colShape = Colliding(pPos);

// should continue box selecting
int contBox = 0;

// if the shape clicked is in selected set the box selection to continue
if(selected.size() > 1)
    for (Shape* s : selected)
        if(s == shapes[colShape])
            contBox = 1;
    
int tmpLDel = lineDeleted;
if(event->button() == Qt::RightButton)
    {
    for(int i = 0; i < shapes.size(); i++)
        {
        // checking the shapes are lines
        if(shapes[i]->shapeType != Shape::ShapeType::LINE) continue;
        
        Line* l = (Line*)shapes[i];

        // 0 is the enum ID for shape (IF CHANGED HERE WILL BE SOURCE OF ERROR)
        vec2 s = l->getSource(), t = l->getTarget();
        float dst = pointLineDist(pPos, s, t);

        // if it is within the bounds it is being clicked
        if(dst <= l->getWidth() + padding)
            {
            // remove the line
            shapes.erase(shapes.begin() + i);

            // the shape has been deleted and a shape has been collided with
            lineDeleted = 1;
            colShape = 1;
            update();
            }
        }
    }

// if the previous state is equal to the new state reset the new state
if(lineDeleted == tmpLDel)
    {
    lineDeleted = 0;
    }

// if not clicking on anything deselect all and say we haven't pressed a shape
if(colShape == -1) { pressedShape = 0; Deselect(); groupDeselect(); }
else pressedShape = 1;

// if the box selection shouldn't continue deselect all
if(contBox == 0) groupDeselect();


// early return if there is no left click because the rest is all for left click
if(event->button() != Qt::LeftButton) return;

// if a shift click is happening
if(event->modifiers() == Qt::ShiftModifier && colShape == -1)
    {
    // deselects anything selected
    Deselect();

    // if there isn't anything colliding
    if(colShape == -1)
        {
        // create a new selection box
        boxSel = new SRectangle(pPos, 0, 0);
        boxSel->changeColour("light grey");
        boxSel->updateOpacity(50);

        // start the box selecting
        boxSelecting = 1;

        update();
        }
    
    return;
    }

// if it's a left click and it is on something and move it to the top of the render queue
if(pressedShape != 0) 
    {
    // selects the clicked shape
    Select(colShape); 
    // swaps the first element of the vector with the clicked one
    std::swap(shapes[colShape], shapes[0]); 
    }

};

void GraphicWindow::mouseMoveEvent(QMouseEvent* event)
{
// get the pointer position
pPos = getPointerPos(event);

// return here if left button isn't being pressed as we can put all things requiring left click below here to not have a massive if
if(event->buttons() != Qt::LeftButton) return;

// start box selecting if we are able to box select and shift is being pressed
if(event->modifiers() == Qt::ShiftModifier && boxSelecting == 1) boxSelect();
 
// if the mouse is being pressed move the shape if the shapes start lagging behind it's most likely because of slow computation
// it's here at the end so no checking has to be done
if(pressedShape) moveShape(event);

};

void GraphicWindow::mouseReleaseEvent(QMouseEvent* event)
{
// getting the position of the release
pPos = getPointerPos(event);

// checking if there is an active object
if(active != nullptr)
    {
    // checking the active object is where the mouse was released and that something is actually moving
    if(active->bbox.contains(pPos.x, pPos.y) && active->moving == 1)
        {            
        // set the shape as inactive and stop it moving
        active->moving = 0;
        active = nullptr;
        }
    }

// release the select box
if(boxSelecting == 1)
    {
    // select all shapes in the box
    groupSelect();

    // deselect the box
    boxSelecting = 0;
    delete boxSel;
    boxSel = nullptr;
    update();
    }

// there won't be a pressed shape if the mouse has been released
pressedShape = 0;
};

void GraphicWindow::mouseDoubleClickEvent(QMouseEvent* event) 
{
// getting mouse pos
getPointerPos(event);

// flag for not adding a line
int nadd = 0;

// for the old clicked shape
static Shape* clicked = nullptr;

// change if right click is needed
if(event->button() != Qt::LeftButton) return;

if(active == nullptr) return;

// if double clicked on the same shape twice
if(clicked == active)
    {
    // deselect the active shape
    Deselect();

    // clear the clicked shape
    clicked = nullptr;

    // start again
    return;
    }
// if the double click was when clicked is null
if(clicked == nullptr)
    {
    // set it to two so that we don't select a different shape on the next click before the double
    pressedShape = 2;

    // set clicked equal to active 
    clicked = active;

    // get the current colour to store
    prevcol = clicked->getColour();

    // change the colour to grey to indicate it has been selected
    clicked->changeColour("grey");

    update();

    // return until new call
    return;
    }


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
    superint ilab1 = atoi(lab1.c_str());
    superint ilab2 = atoi(lab2.c_str());

    // check see if it is a factor or a number
    int primeTest1 = isPrime(ilab1);
    int primeTest2 = isPrime(ilab2);

    if((primeTest1 == 1 && primeTest2 == 1) || (primeTest1 == 0 && primeTest2 == 0)) nadd = 1;

    // if the first is prime find the power it should be using
    if(primeTest1 == 1)
        power = ((Node*)clicked)->preemptPower(lab1);
    // if the second is prime find the power again
    else if (primeTest2 == 1)
        power = ((Node*)active)->preemptPower(lab2);
    
    // if there is nothing there don't add the line
    if(power == "") nadd = 1;

    // if the power is 1 then don't write the text (aka. write as empty: "")
    if(power == "1") power = "";

    // set the text
    l->setText(power);
    }

// add the new edge
if(nadd == 0 || factorConnect == 1)
    add(l);

// deselect after creating the line
Deselect();

// reset clicked
clicked = nullptr;

// redraw
update();

};

void GraphicWindow::contextMenuEvent(QContextMenuEvent* event)
{
if(Colliding(pPos) != -1) return;
if(lineDeleted == 1) return;

QMenu menu(this);

getPointerPos(event);

QAction *nNode = menu.addAction("New Node");
QAction *action2 = menu.addAction("Action 2");

connect(nNode, &QAction::triggered, this, &GraphicWindow::createNode);

menu.exec(mapToGlobal({(int)pPos.x, (int)pPos.y}));

//QAction *selectedAction = menu.exec(event->globalPos());

/* if (selectedAction == action1) 
    {
    createNode();
    }
else if (selectedAction == action2) 
    {

    } */

}

void GraphicWindow::createNode()
{
std::cout << "\nCreating node";

Node* n = new Node(pPos, "", 5, "red");

shapes.push_back(n);

update();
}

vec2 GraphicWindow::getPointerPos(QMouseEvent* event)
{
int px = event->pos().rx();
int py = event->pos().ry();

pPos = {(float)px, (float)py};

return pPos;
}

vec2 GraphicWindow::getPointerPos(QContextMenuEvent* event)
{
int px = event->pos().x();
int py = event->pos().y();

pPos = {(float)px, (float)py};

return pPos;
}

void GraphicWindow::moveShape(QMouseEvent* event)
{
// for each shape in shapes
for(Shape* s : shapes)
    {
    // check the click is in the shape
    if(s->bbox.contains(pPos.x, pPos.y))
        {
        // if it is then set the shape as active and moving
        s->moving = 1;
        s->revertColour();
        break;
        }
    }

// the change in active's position is the same change that needs to be made to each shape's position
vec2 change = pPos - active->coords();

// first move active
active->move(pPos);

for(int i = 0; i < selected.size(); i++)
    {
    // if selected at i is the active then skip it's moving
    if(selected[i] == active) continue;

    // shape's current position
    vec2 cPos = selected[i]->coords();

    // move each selected shape by the change
    selected[i]->move(cPos + change);
    }

update();
};

void GraphicWindow::groupSelect()
{
// for each shape in shapes
for (Shape* s : shapes)
    {
    // coords of the shape
    vec2 co = s->coords();

    // if they are inside of the selection box
    if(boxSel->bbox.contains(co.x, co.y))
        {
        // add them to the selected vector
        selected.push_back(s);
        }
    }
}

void GraphicWindow::groupDeselect()
{
selected.clear();
}

void GraphicWindow::boxSelect()
{
// rect pos
vec2 rPos = boxSel->coords(); 

// get the current scale
boxSel->getScale();

// set the new scale
boxSel->setScale(pPos-rPos);

update();
}

void GraphicWindow::Select(Shape* s)
{
Deselect();

active = s;
}

void GraphicWindow::Select(int index)
{
Select(shapes[index]);
}

void GraphicWindow::Deselect()
{
if(active == nullptr) return;

// change it's colour back
active->revertColour();

// remove the node's shape from active
active = nullptr;

// redraw
update(); 
}

int GraphicWindow::Colliding(vec2 mPos)
{
for(int i = 0; i < shapes.size(); i++)
    {
    Shape* s = shapes[i];

    // if the shape contains the position
    if(s->bbox.contains(mPos.x, mPos.y))
        {
        // return the shape index
        return i;
        }
    }


return -1;
}

void GraphicWindow::remAll()
{
shapes.erase(shapes.begin(), shapes.end());

update();
}

int GraphicWindow::toggleConnect()
{
factorConnect ^= 1;

return factorConnect;
}

int GraphicWindow::getMinimumRadius(std::string nodeText)
{
int minrad = 0;

// calculation to calculate the minimum size of the node to fit the number
int wid = fontMetrics().horizontalAdvance(nodeText.c_str());
int hig = fontMetrics().height();

// if the width is less than the height set the min radius equal to the height
if(wid < hig) minrad = hig;
else minrad = wid;

return minrad;
}

Shape* GraphicWindow::getShape(int index)
{
return shapes[index];
}

void MainWindow::resizeElement(QWidget* widget)
{
widget->resize(widget->sizeHint().width(), widget->sizeHint().height());
}

void MainWindow::resizeButton(QPushButton* lebutton)
{
//lebutton->setFixedSize(lebutton->fontMetrics().horizontalAdvance(lebutton->text()), lebutton->fontMetrics().height());

lebutton->setFixedSize(lebutton->sizeHint().width(), lebutton->sizeHint().height());


std::cout << "\n" << lebutton->text().toStdString();
std::cout << "\nHorizontal Advance: " << lebutton->fontMetrics().horizontalAdvance(lebutton->text());
}


MainWindow::MainWindow(QWidget* parent, GraphicWindow* graphicWindow)
    : QMainWindow(parent), gwin(graphicWindow)
{
// menu bar setup
QMenuBar* menubar = new QMenuBar(this);
setMenuBar(menubar);

QAction* uniFac = new QAction("Unique Factors", this);
uniFac->setCheckable(1);
uniFac->setChecked(0);

QAction* onlyFacs = new QAction("Only Factors", this);
onlyFacs->setCheckable(1);
onlyFacs->setChecked(1);

// add the main menu
QMenu* optionsMenu = new QMenu("Options");
menubar->addMenu(optionsMenu);
optionsMenu->addAction(uniFac);
optionsMenu->addAction(onlyFacs);

connect(uniFac, &QAction::triggered, this, uniqueFactors);
connect(onlyFacs, &QAction::triggered, this, onlyFactors);

// setting up the main widget
QDockWidget* dockWidget = new QDockWidget("Side pannel", this);

// creating its' content widget
QWidget* dockContent = new QWidget(dockWidget);

// setting the docking widget's content up
dockWidget->setWidget(dockContent);

// setting up the layout of the widget
dockinglayout = new QVBoxLayout(dockContent);
QHBoxLayout* buttonlayout = new QHBoxLayout();
QVBoxLayout* tablelayout = new QVBoxLayout();
QVBoxLayout* nodeInfoLayout = new QVBoxLayout();

// add the widget to the window
addDockWidget(Qt::RightDockWidgetArea, dockWidget);

table = new Table();

// button setup
QPushButton* loadNewButton = new QPushButton("Load new set");
QPushButton* resetButton = new QPushButton("Reset");

QLabel* nodeIDLabel = new QLabel("Node ID: ");

QPushButton* createNewNode = new QPushButton("Create");
colourInput = new QLineEdit();
internalLabel = new QLineEdit();

inNum = new QLineEdit();
inNum->setPlaceholderText("Enter Number to check");
QPushButton* submit = new QPushButton("Submit");

// resizing the buttons
resizeButton(loadNewButton);
resizeButton(resetButton);
resizeButton(createNewNode);
resizeButton(submit);

// adding them all to here to stop them having to be individually stored in the class
widgets.push_back(optionsMenu);
widgets.push_back(dockWidget);
widgets.push_back(dockContent);
widgets.push_back(table);
widgets.push_back(loadNewButton);
widgets.push_back(resetButton);

// adding both widgets to the dock
buttonlayout->addWidget(loadNewButton);
buttonlayout->addWidget(resetButton);

nodeInfoLayout->addWidget(createNewNode);
nodeInfoLayout->addWidget(colourInput);
nodeInfoLayout->addWidget(internalLabel);
nodeInfoLayout->addWidget(nodeIDLabel);
nodeInfoLayout->addWidget(inNum);
nodeInfoLayout->addWidget(submit);

tablelayout->addWidget(table);

// add the layout's together
dockinglayout->addLayout(buttonlayout);
dockinglayout->addLayout(nodeInfoLayout);
dockinglayout->addLayout(tablelayout);


// setting the dock layout
dockContent->setLayout(dockinglayout);

// setting the main widget of the dock
dockWidget->setWidget(dockContent);

// connecting the button to the window so that it can be used
connect(loadNewButton, &QPushButton::clicked, this, pressy);
connect(resetButton, &QPushButton::clicked, this, remAll);
connect(submit, &QPushButton::clicked, this, testIsPrime);
connect(createNewNode, &QPushButton::clicked, this, createNode);
};

Table::Table(QWidget* parent)
    : QTableWidget(parent)
{
// setting the rows and the columns
this->setRowCount(0);
this->setColumnCount(3);

resizeRowsToContents();
resizeColumnsToContents();

this->setSizeAdjustPolicy(SizeAdjustPolicy::AdjustToContents);
this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

void Table::minimizeTable()
{
int totalH = 0;
for (int i = 0; i < rowCount(); i++)
    {
    totalH += rowHeight(i);
    }

int totalW = 0;
for (int i = 0; i < columnCount(); i++)
    {
    totalW += columnWidth(i);
    }

setFixedHeight(totalH);
setFixedWidth(totalW);
}

void Table::add(int column, std::string item, int startnewrow)
{
QTableWidgetItem* element = new QTableWidgetItem(item.c_str());

// row count
int rc = rowCount();

// add a new row if starting new row
if(startnewrow == 1)
    setRowCount(rc+1);

// updating the local top
localTop = rc - 1;

// setting the new item
setItem(localTop, column, element);

// resizing
resizeRowsToContents();
resizeColumnsToContents();

// updating the table
update();

// adding the element to the list
tableItems.push_back(element);
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

// resizing
resizeRowsToContents();
resizeColumnsToContents();

// updating the table
update();

// adding the element to the list
tableItems.push_back(element);
}

void Table::newRow()
{
// get the row count
int rc = rowCount();

// set the new row count to the current + 1
setRowCount(rc+1);

// resizing
resizeRowsToContents();
resizeColumnsToContents();

// updating the table
update();
}

void Table::Clear()
{
clearContents();
}

// LOOK AT THIS IT'S BUGGING SOMETIMES WITH READING BACK FACTORS (LOOK AT 30)
int loadnew(GraphicWindow* gwin, DetailContainer* dc, Table* table)
{
int curRow = table->getLocalTop();

Detail detail = dc->getTop();

// composite
std::string compnum = std::to_string(detail.number);

int minrad = gwin->getMinimumRadius(compnum);

// create the node of the base number
Node* number = new Node({50, 50}, compnum, minrad, "light green");
gwin->add(number);

// create a new row for the values
table->newRow();

// get this again because we're making a new row
curRow = table->getLocalTop();

// adding the number to the table
table->add(0, compnum);

Factors factors = getAsFactors(detail.factors);

// adding all of the factors to the number node
number->addFactors(factors);

// a collection of the factors
std::stringstream ss[2];

int count = 0;
for (Number i : detail.factors)
    {
    // if the base has already been and the factors have to be unique then continue to the next number
    if(dc->hasBeen(i.base) == 1 && dc->getUniqueFactor() == 1) continue;

    // doing this down here to save processing time
    std::string strnum = std::to_string(i.base);
    ss[0] << strnum << ", ";

    // calculation for getting the minimum radius
    minrad = gwin->getMinimumRadius(strnum);

    Node* n = new Node({(float)(100 + count*50), (float)50}, strnum, minrad, "light blue");
    
    gwin->add(n);

    count++;
    }

// the stream as a string
std::string compactStr = ss[0].str();

// the final string -2 as it gets rid of the extra ", "
std::string final = compactStr.substr(0, compactStr.length()-2);

// add the factors to the second row
table->add({(float)curRow, 1}, final);

// add the multiplication to the exponential component
for (Number n : detail.factors)
    {
    ss[1] << n.getStr() << " * ";
    }

// the exponential stream as a string
std::string Nucompact = ss[1].str();

// the final exponential nums string -3 as it gets rid of the extra " * "
std::string Nufinal = Nucompact.substr(0, Nucompact.length()-3);

table->add({(float)curRow, 2}, Nufinal);

dc->incrementTop();

return 0;
}

void MainWindow::pressy()
{
// loads the new nodes
loadnew(this->gwin, &dc, this->table);
update();
}

void MainWindow::remAll()
{
gwin->remAll();

dc.Reset();

table->Clear();
}

void MainWindow::onlyFactors()
{
gwin->toggleConnect();
}

void MainWindow::uniqueFactors()
{
dc.toggleUniqueFactor();
}

void MainWindow::testIsPrime()
{
std::string inputtxt = inNum->text().toStdString();

unsigned int in = atoi(inputtxt.c_str());

// variable to record if it is prime
int Isprime = isPrime(in);

// if it is prime turn the background light green
if(Isprime == 1) inNum->setStyleSheet("background-color: rgba(187, 255, 181, 1);");
// clear the background
else inNum->setStyleSheet("background-color: none;");

QTimer t;

// clear the background after 1 second
t.singleShot(1000, ([&]{inNum->setStyleSheet("background-color: none;");}));
}

void MainWindow::createNode()
{
std::string col = colourInput->text().toStdString();
std::string internalLab = internalLabel->text().toStdString();

// get the string as a number
superint number = atoi(internalLab.c_str());

// factor the inputted number
Factors fs = BruteFactor(number);

// getting the factors as numbers
std::vector<Number> nums = fs.getAsNumbers();

// increment the top
dc.incrementTop();

// create the detail to push
Detail det = {number, nums};

// pushing back a detail to the detail container
dc.pushTop(det);

// add a new row before adding
table->newRow();

// add the number in the first position
table->add(0, internalLab, 0);

// get only the unique factors
table->add(1, fs.unique(), 0);

// add the compressed exponential form of the factors to the table
table->add(2, fs.compress(1), 0);

// getting the minimum radius of the node
int minrad = gwin->getMinimumRadius(internalLab);

// create the node of the base number
Node* numberNode = new Node({50, 50}, internalLab, minrad, "light green");

// add the factors to the node
numberNode->addFactors(fs);

gwin->add(numberNode);

// a count variable to count iterations for spacing of nodes
int count = 0;

for (Number i : nums)
    {
    // if the base has already been and the factors have to be unique then continue to the next number
    if(dc.hasBeen(i.base) == 1 && dc.getUniqueFactor() == 1) continue;

    // getting the base as a string
    std::string strnum = std::to_string(i.base);

    // calculation for getting the minimum radius
    minrad = gwin->getMinimumRadius(strnum);

    // creating the node
    Node* n = new Node({(float)(100 + count*50), (float)50}, strnum, minrad, "light blue");

    // adding the node to the graphics window
    gwin->add(n);

    count++;
    }

update();
}

int CpMain()
{
int h = 0;
QApplication app(h, {});
MainWindow* window = new MainWindow();

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
