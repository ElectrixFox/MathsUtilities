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

// the index of the shape clicked on
int colShape = Colliding(pPos);

// should continue box selecting
int contBox = 0;

// if the shape clicked is in selected set the box selection to continue
if(selected.size() > 1)
    for (Shape* s : selected)
        if(s == shapes[colShape])
            contBox = 1;
    

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
        if(dst <= l->getWidth())
            {
            shapes.erase(shapes.begin() + i);
            update();
            }
        
        }
    
    }

// if not clicking on anything deselect all and say we haven't pressed a shape
if(colShape == -1) { pressedShape = 0; Deselect(); groupDeselect(); }
else pressedShape = 1;

// if the box selection shouldn't continue deselect all
if(contBox == 0) groupDeselect();


// early return if there is no left click because the rest is all for left click
if(event->button() != Qt::LeftButton) return;

// if a shift click is happening
if(event->modifiers() == Qt::ShiftModifier)
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

// if it's a left click and it is on something
if(pressedShape != 0) Select(colShape);
};

// FIX BOX SELECTING CRASHING WHEN STARTING SELECT ON NODE
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


if(clicked != nullptr)
    std::cout << "\nClicked: " << clicked->getID() << "\tActive: " << active->getID();
else
    std::cout << "\nClicked: " << clicked << "\tActive: " << active->getID();

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

// if there is already an active node and it isn't the clicked one
if(active != clicked)
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
    }
};

vec2 GraphicWindow::getPointerPos(QMouseEvent* event)
{
int px = event->pos().rx();
int py = event->pos().ry();

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

for(int i = 1; i < selected.size(); i++)
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

boxSel->getScale(1);

pPos.out();

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

Shape* GraphicWindow::getShape(int index)
{
return shapes[index];
}

MainWindow::MainWindow(QWidget* parent, GraphicWindow* graphicWindow)
    : QMainWindow(parent), gwin(graphicWindow)
{
// menu bar setup
QMenuBar* menubar = new QMenuBar(this);
setMenuBar(menubar);

// add the main menu
QMenu* fileMen = new QMenu("FILE");
menubar->addMenu(fileMen);

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

// add the widget to the window
addDockWidget(Qt::RightDockWidgetArea, dockWidget);

// adding the unique factor checkbox
QCheckBox* uniqueFac = new QCheckBox("Unique Factors", dockContent);

QCheckBox* onlyFac = new QCheckBox("Only Factors", dockContent);
onlyFac->setChecked(1);

table = new Table(dockContent);

table->resizeColumnsToContents();
table->resizeRowsToContents();

// button setup
QPushButton* loadNew = new QPushButton("Load new set", dockContent);

QPushButton* resetButton = new QPushButton("Reset", dockContent);

// the width of the button's text
int wid = fontMetrics().horizontalAdvance("  Load new set  ");

// setting the width and height of the button
loadNew->setFixedSize(wid, 30);
resetButton->setFixedSize(fontMetrics().horizontalAdvance("  Reset  "), 30);


// setting the layout
dockinglayout->setContentsMargins(0, 0, 0, 0);


// setting the minimum size to be the button's width so that it is always readable
dockContent->setMinimumWidth(table->width());

// set spacing
dockinglayout->setSpacing(0);

// adding them all to here to stop them having to be individually stored in the class
widgets.push_back(fileMen);
widgets.push_back(dockWidget);
widgets.push_back(dockContent);
widgets.push_back(table);
widgets.push_back(loadNew);
widgets.push_back(resetButton);

// adding both widgets to the dock
buttonlayout->addWidget(loadNew);
buttonlayout->addWidget(resetButton);
tablelayout->addWidget(uniqueFac);
tablelayout->addWidget(onlyFac);
tablelayout->addWidget(table);

// add the layout's together
dockinglayout->addLayout(buttonlayout);
dockinglayout->addLayout(tablelayout);


// setting the dock layout
dockContent->setLayout(dockinglayout);

// setting the main widget of the dock
dockWidget->setWidget(dockContent);

// connecting the button to the window so that it can be used
connect(loadNew, &QPushButton::clicked, this, pressy);
connect(resetButton, &QPushButton::clicked, this, remAll);
connect(uniqueFac, &QCheckBox::clicked, this, uniqueFactors);
connect(onlyFac, &QCheckBox::clicked, this, onlyFactors);
};

Table::Table(QWidget* parent)
    : QTableWidget(parent)
{
// setting the rows and the columns
this->setRowCount(3);
this->setColumnCount(3);

resizeRowsToContents();
resizeColumnsToContents();
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

void Table::Clear()
{
clearContents();
}

int loadnew(GraphicWindow* gwin, DetailContainer* dc, Table* table)
{
int curRow = dc->getTopV();

Detail detail = dc->getTop();

Node* number = new Node({50, 50}, std::to_string(detail.number), 14, "light green");
gwin->add(number);

// adding the number to the table
table->add({(float)curRow, 0}, std::to_string(detail.number));

for (Number n : detail.factors)
    {
    // adding the factors to the vector
    number->addFactor(n);
    }

// a collection of the factors
std::stringstream ss[2];

int count = 0;
for (Number i : detail.factors)
    {
    // doing this before so they can go straight to the table
    std::string strnum = std::to_string(i.base);
    ss[0] << strnum << ", ";
    
    if(dc->hasBeen(i.base) == 1 && dc->getUniqueFactor() == 1) continue;

    Node* n = new Node({(float)(100 + count*50), (float)50}, strnum, 14, "light blue");
    
    gwin->add(n);

    count++;
    }

// the stream as a string
std::string compactStr = ss[0].str();

// the final string -2 as it gets rid of the extra ", "
std::string final = compactStr.substr(0, compactStr.length()-2);

table->add({(float)curRow, 1}, final);

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
