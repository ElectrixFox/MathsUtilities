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

Node* n = new Node(pPos, "5", getMinimumRadius("5"), "red");

shapes.push_back(n);

emit eventOccurred();

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
lastActive = s;
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


