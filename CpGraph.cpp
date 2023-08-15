#include "CpGraph.h"

int n = 1;

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

float pointLineDist(vec2 point, vec2 startPoint, vec2 endPoint)
{
float a = startPoint.x;
float b = startPoint.y;

float c = endPoint.x;
float d = endPoint.y;

float x = point.x;
float y = point.y;

// using vector things to get this
// it may look ugly but it's like this temporarily whilst I sort everything else out, then it will be made more readable 
float lambda = (((a*a + b*b + c*x + d*y) - (a*c + a*x + d*b + b*y)) / ((a*a + b*b + c*c + d*d + a) - (2*a*c + 2*b*d)));

// gets the magnitude of the line
float dst = sqrt(pow((lambda*(c - a) + a - x), 2)+pow((lambda*(d - b) + b - y), 2));

return dst;
}

vec2 Shape::coords(int print)
{
if(print == 1)
    printf("\n(%f, %f)", x, y);

return { (float)x, (float)y };
}

void Shape::move(int xto, int yto, int offset)
{
x = xto;
y = yto;
bbox.moveTo(x - offset, y - offset);
}

void Shape::changeColour(std::string col)
{
colour = col;
};

Shape::Shape(int inx, int iny) : x(inx), y(iny), origionalColour("red") { id = n; n += 1; };

Shape::Shape(int inx, int iny, std::string col)
: x(inx), y(iny), origionalColour(col)
{
id = n;
n += 1;
}

void Shape::revertColour()
{
changeColour(origionalColour);
}

Text::Text(int inx, int iny, std::string content)
{
// I don't like floats so I'm casting
x = (float)inx;
y = (float)iny;

text = content;
}

Text::Text(int inx, int iny, std::string content, Shape* par)
{
// I don't like floats so I'm casting
x = (float)inx;
y = (float)iny;

// setting the text
text = content;

// setting a parent to center the text
parent = par;
}

void Text::move(vec2 pos)
{
x = pos.x;
y = pos.y;
}

void Text::draw(QPainter* qp)
{
// add options in the future for colour, size, font, etc...

// updating the position if the shape parent moves
if(parent != nullptr)
    x = parent->coords().x;
    y = parent->coords().y;

// setting the font
QFont font("Arial", 14);
qp->setFont(font);

// getting the font details
QFontMetrics fontmets(font);

// getting the height
int fheight = fontmets.height();

// getting the width
int fwidth = fontmets.horizontalAdvance(text.c_str());

// need a y-offset otherwise it won't draw to centre
int yoffset = floor(fheight/4);

// declaring the x-offset
int xoffset = 0;

// if there is a parent then change the offset
if(parent != nullptr)
    xoffset = (floor(fwidth/2));

// writing the text
qp->drawText(x - xoffset, y + yoffset, text.c_str());
}

Circle::Circle(int inx, int iny, int radius, std::string col) : Shape(inx, iny, col), radius(radius)
{
x = inx;
y = iny;
id = n;
n += 1;

colour = col;


int a = x - radius;
int b = y - radius;
int c = 2*radius;

bbox = QRectF(a, b, c, c);
}

void Circle::draw(QPainter* qp)
{
QColor col = QColor(colour.c_str());
qp->setBrush(col);

qp->drawEllipse(bbox);
}

void Circle::move(int xto, int yto, int offset)
{
x = xto;
y = yto;

offset = radius;

bbox.moveTo(x - offset, y - offset);
}

Edge::Edge(Shape* insource, Shape* intarget, int wid)
{
source = insource;
target = intarget;

width = wid;
};

Edge::Edge(Node* insource, Node* intarget, int wid)
{
nodesource = insource;
nodetarget = intarget;

source = insource->getShape();
target = intarget->getShape();

width = wid;
}

void Edge::createText(std::string content)
{
// get the midpoint
vec2 pos = (source->coords()-target->coords())/2;

// create the text at the midpoint
tex = new Text(pos.x, pos.y, content);
}

void Edge::draw(QPainter* qp)
{
// assuming they're circles
Circle* c1 = (Circle*)source;
Circle* c2 = (Circle*)target;

// giving all of these values easy to use alias'
float x1 = c1->coords().x;
float y1 = c1->coords().y;
float r1 = c1->getRadius();

float x2 = c2->coords().x;
float y2 = c2->coords().y;
float r2 = c2->getRadius();

if((x2 - x1) == 0)
    x1 = x2 + 0.00001f;

// getting the gradient of the line between them
float m = (y2 - y1)/(x2 - x1);

// if c2 is on the other side to c1 then add 180 to make it go full 2 pi radians (360 deg)
float O = (x2 < x1) ? M_PI : 0;


// getting the angle between the centre of the circle and where the line passes through it's circumference
float p = atan(m)+O;

// cos of the angle between the line and the centre
// where r1 is the hypotenuse of the line we find the horizontal distance and then offset it by x1 to make it local to the circle
// same sort of thing with sin apart from it's used to find the vertical distance
vec2 pos1 = {x1 + r1*cos(p), y1 + r1*sin(p)};

// negative here because it is intersecting a different circle at a different point
vec2 pos2 = {x2 - r2*cos(p), y2 - r2*sin(p)};

// create a pen
QPen pen;

// set the width of the line
pen.setWidth(getWidth());

// set the pen
qp->setPen(pen);

// drawing the line
qp->drawLine(pos1.x, pos1.y, pos2.x, pos2.y);

// if there is text
if(tex != nullptr)
    {
    // get midpoint of the line
    vec2 midp = (source->coords()-target->coords())/2;

    // move to the midp
    tex->move(midp);

    // draw the text
    tex->draw(qp);
    }
};

Shape* Node::getShape()
{
return cir;
}

Text* Node::getText()
{
return tex;
}

Node::Node(vec2 pos, std::string label, int radius, std::string colour)
{
// again I don't like floats
cir = new Circle((int)pos.x, (int)pos.y, radius, colour);

tex = new Text((int)pos.x, (int)pos.y, label, cir);

position = cir->coords();

};

std::string Node::preemptPower(std::string factor)
{
// for each number in nums
for(Number n : nums)
    {
    // if the base equals the factor
    if(n.base == atoi(factor.c_str()))
        {
        // return the exponent
        return std::to_string(n.exponent);
        }
    }

// if it's not a factor return nullptr to show
return nullptr;
}

void Node::addFactor(int factor, int power)
{
nums.push_back({factor, power});
}

void Node::draw(QPainter* qp)
{
cir->draw(qp);
tex->draw(qp);
};

void GraphicWindow::add(Shape* shape)
{
shapes.push_back(shape);
}

void GraphicWindow::add(Edge* edge)
{
edges.push_back(edge);
}

void GraphicWindow::add(Node* node)
{
nodes.push_back(node);
}

void GraphicWindow::paintEvent(QPaintEvent* event)
{    
QPainter p(this);
p.setRenderHint(QPainter::Antialiasing);

for (Edge* e : edges)
    {
    e->draw(&p);
    }
for (Node* n : nodes)
    {
    n->draw(&p);
    }
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
    for(int i = 0; i < edges.size(); i++)
        {
        Edge* e = edges[i];
        vec2 s = e->getSourceCoords(), t = e->getTargetCoords();
        float dst = pointLineDist((vec2){(float)px, (float)py}, s, t);

        // if it is within the bounds it is being clicked
        if(dst <= e->getWidth())
            {
            edges.erase(edges.begin() + i);
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

// for each node in nodes
for (Node* n : nodes)
    {
    // get the shape of the node
    Shape* s = n->getShape();
    
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
        active->move(x, y);

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

// vector of shapes so that they can be easily accessed
std::vector<Shape*> shs;

// getting all of the node's shapes into a vector
for (Node* n : nodes)
    shs.push_back(n->getShape());

// going through all of the node's to check if they are valid options
for (Shape* s : shs)
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
    Edge* e = new Edge(active, clicked, 2);

    // add the new edge
    add(e);

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

window->setGeometry(100, 100, 400, 400);

window->show();


int result = app.exec();

return result;
}

