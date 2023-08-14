#include "CpGraph.h"

int n = 1;

struct vec2 { float x, y; };

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

class Shape
{
public:
    Shape(int inx, int iny);

    virtual void draw(QPainter* qp) {};
    
    vec2 coords(int print = 0) { if(print == 1) printf("\n(%f, %f)", x, y); return {(float)x, (float)y}; };
    virtual void move(int xto, int yto, int offset = 0) { x = xto; y = yto; bbox.moveTo(x - offset, y - offset); };

    void changeColour(std::string col);

    int getID() { return id; };
    std::string getColour() { return colour; };

    QRectF bbox;

    int moving = 0;

protected:
    int id;
    int x, y;
    std::string colour = "red";
};

void Shape::changeColour(std::string col)
{
colour = col;
};

Shape::Shape(int inx, int iny) : x(inx), y(iny) { id = n; n += 1; };

class Text
    {
    public:

    // constructor to create the object, defaulting "" text so that it can be empty to start for future changing
    Text(int inx, int iny, std::string content = "");
    Text(int inx, int iny, std::string content, Shape* par);

    // drawing
    void draw(QPainter* qp);

    private:
    float x, y;
    std::string text;
    Shape* parent; 

    };

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

class Edge
{
public:
    // source and target shapes
    Edge(Shape* insource, Shape* intarget, int wid = 1);

    // drawing
    void draw(QPainter* qp);

    vec2 getSourceCoords(int print = 0) { return source->coords(print); };
    vec2 getTargetCoords(int print = 0) { return target->coords(print); };
    int getWidth() { return width; };

private:
    int width = 1;

    Shape* source;
    Shape* target;
};

class Circle : public Shape
{
public:
    Circle(int inx, int iny, int radius);

    void draw(QPainter* qp) override;

    // overrides because of the different shapes
    virtual void move(int xto, int yto, int offset = 0) override;

    int getID() { return id; };
    int getRadius() { return radius; };

protected:
    int radius;
    int moving = 0;
};

Circle::Circle(int inx, int iny, int radius) : Shape(inx, iny), radius(radius)
{
x = inx;
y = iny;
id = n;
n += 1;

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
};

class Node
    {
    public:
    Node(vec2 pos, std::string label, int radius);

    void draw(QPainter* qp);

    vec2 coords(int print = 0) { return cir->coords(print); };

    Shape* getShape();
    Text* getText();

    private:
    vec2 position;
    Text* tex;
    Circle* cir;

    };

Shape* Node::getShape()
{
return cir;
}

Text* Node::getText()
{
return tex;
}

Node::Node(vec2 pos, std::string label, int radius)
{
// again I don't like floats
cir = new Circle((int)pos.x, (int)pos.y, radius);
tex = new Text((int)pos.x, (int)pos.y, label, cir);

position = cir->coords();

};

void Node::draw(QPainter* qp)
{
cir->draw(qp);
tex->draw(qp);
};

class GraphicWindow : public QWidget
{
public: 
    GraphicWindow(QWidget* parent = nullptr) : QWidget(parent) { setMouseTracking(true); };

    void add(Shape* shape);
    void add(Edge* edge);
    void add(Node* node);
    
    Shape* getShape(int index);

protected:
    std::vector<Shape*> shapes;
    std::vector<Edge*> edges;
    std::vector<Node*> nodes;

    Shape* active = nullptr;
    std::string prevcol = "";
    int pressed = 0;
    int selected = 0;

    void paintEvent(QPaintEvent* event) override 
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

    void mousePressEvent(QMouseEvent* event) override 
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

            std::cout << dst;

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

    void mouseMoveEvent(QMouseEvent* event) override 
    {
    // early return if nothing is being pressed at the same time
    if(pressed != 1)
        return;

    int px = event->pos().rx();
    int py = event->pos().ry();

    // before setting a new active deselect the old and set its colour back
    if(active != nullptr && prevcol != "")
        {
        active->changeColour(prevcol);
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

    void mouseReleaseEvent(QMouseEvent* event) override
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

    void mouseDoubleClickEvent(QMouseEvent* event) override
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
            std::cout << "\nPrevious: " << prevcol;
            
            // remove the node's shape from active
            std::cout << "\nDeselecting: " << active->getID();

            // change it's colour back
            active->changeColour(prevcol);
            active = nullptr;

            // redraw
            update();
            }

        return;
        }

    std::cout << "\n: " << active << ", " << clicked;

    // if there is already an active node and it isn't the clicked one
    if(active != nullptr && active != clicked)
        {
        // create a new edge between the active and the newly selected
        Edge* e = new Edge(active, clicked, 2);

        // add the new edge
        add(e);

        std::cout << "\nPrevious: " << prevcol;

        // change the colour of the active shape back
        active->changeColour(prevcol);

        // make the shape inactive
        active = nullptr;

        // redraw
        update();
        }
    // if there isn't an active node select the clicked one
    else if(active == nullptr)
        {
        std::cout << "\nSelecting: " << clicked->getID();

        // get the current colour to store
        prevcol = clicked->getColour();

        // change the colour to grey to indicate it has been selected
        clicked->changeColour("grey");

        std::cout << "\nClicked: " << clicked;

        // set the node's shape as the active shape
        active = clicked;
        }
    else
        {
        std::cout << "\nPrevious: " << prevcol;
        std::cout << "\nDeselecting: " << active->getID();
        
        // change colour back
        clicked->changeColour(prevcol);

        // remove the node's shape from active
        active = nullptr;
        }

    update();
    };

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

Shape* GraphicWindow::getShape(int index)
{
return shapes[index];
}

int loadnew(GraphicWindow* gwin)
{
Node* number = new Node({50, 50}, "15", 14);
gwin->add(number);

Node* fac1 = new Node({100, 50}, "3", 14);
Node* fac2 = new Node({150, 50}, "5", 14);

gwin->add(fac1);
gwin->add(fac2);

return 0;
}

class MainWindow : public QMainWindow
    {
    public:
    MainWindow(QWidget* parent = nullptr, GraphicWindow* graphicWindow = nullptr);

    GraphicWindow* gwin;

    public slots:
    void pressy();

    private:
    QPushButton* button;
    QMenuBar* menubar;
    QDockWidget* dockWidget;
    QWidget* dockContent;
    QMenu* fileMen;
    };

MainWindow::MainWindow(QWidget* parent, GraphicWindow* graphicWindow)
    : QMainWindow(parent), gwin(graphicWindow)
{
// menu bar setup
menubar = new QMenuBar(this);
setMenuBar(menubar);

fileMen = new QMenu("FILE");
menubar->addMenu(fileMen);

// setting up the main widget
dockWidget = new QDockWidget("Side pannel", this);

// creating its' content widget
dockContent = new QWidget(dockWidget);

// setting the docking widget's content up
dockWidget->setWidget(dockContent);

// add the widget to the window
addDockWidget(Qt::RightDockWidgetArea, dockWidget);

// button setup
button = new QPushButton("Load new set", dockContent);

// the width of the button's text
int wid = fontMetrics().horizontalAdvance("  Load new set  ");

// setting the width and height of the button
button->setFixedSize(wid, 30);

// setting the minimum size to be the button's width so that it is always readable
dockContent->setMinimumWidth(button->width());

// connecting the button to the window so that it can be used
connect(button, &QPushButton::clicked, this, pressy);
};

void MainWindow::pressy()
{
std::cout << "\n\nClicky Clicky";

loadnew(this->gwin);
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

Node* n1 = new Node({100, 100}, "Hello", 25);
Node* n2 = new Node({300, 300}, "World", 25);

gwin.add(n1);
gwin.add(n2);

window->setGeometry(100, 100, 400, 400);

window->show();


int result = app.exec();

return result;
}

