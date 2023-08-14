#include "CpGraph.h"

int n = 1;

struct vec2
{
float x, y;
};

class Shape
{
public:
    Shape(int inx, int iny);

    virtual void draw(QPainter* qp) {};
    
    vec2 coords(int print = 0) { if(print == 1) printf("\n(%f, %f)", x, y); return {(float)x, (float)y}; };
    virtual void move(int xto, int yto, int offset = 0) { x = xto; y = yto; bbox.moveTo(x - offset, y - offset); };

    int getID() { return id; };

    QRectF bbox;

    int moving = 0;

protected:
    int id;
    int x, y;
};

Shape::Shape(int inx, int iny) : x(inx), y(iny) { id = n; n += 1;};

class Text
    {
    public:

    // constructor to create the object, defaulting "" text so that it can be empty to start for future changing
    Text(int inx, int iny, std::string content = "");

    // drawing
    void draw(QPainter* qp);

    private:
    float x, y;
    std::string text;

    };


Text::Text(int inx, int iny, std::string content)
{
// I don't like floats so I'm casting
x = (float)inx;
y = (float)iny;

text = content;
}

void Text::draw(QPainter* qp)
{
// add options in the future for colour, size, font, etc...

// writing the text
qp->drawText(x, y, "Hello, Qt!");
}

class Edge
{
public:
    // source and target shapes
    Edge(Shape* insource, Shape* intarget);

    // drawing
    void draw(QPainter* qp);

private:
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
qp->setBrush(Qt::red);

qp->drawEllipse(bbox);
}

void Circle::move(int xto, int yto, int offset)
{
x = xto;
y = yto;

offset = radius;

bbox.moveTo(x - offset, y - offset);
}

Edge::Edge(Shape* insource, Shape* intarget)
{
source = insource;
target = intarget;
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

// drawing the line
qp->drawLine(pos1.x, pos1.y, pos2.x, pos2.y);
};

class WindowManager : public QWidget
{
public: 
    WindowManager(QWidget* parent = nullptr) : QWidget(parent) { setMouseTracking(true); };

    void add(Shape* shape);
    void add(Edge* edge);
    Shape* getShape(int index);

protected:
    std::vector<Shape*> shapes;
    std::vector<Edge*> edges;
    Shape* active = nullptr;

    void paintEvent(QPaintEvent* event) override 
    {    
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    for (Shape* s : shapes)
        {
        s->draw(&p);
        }
    for (Edge* e : edges)
        {
        e->draw(&p);
        }
    };

    void mousePressEvent(QMouseEvent* event) override 
    {    
    event->button();
    int px = event->pos().rx();
    int py = event->pos().ry();

    std::cout << "\n(" << px << ", " << py << ")";

    for(Shape* s : shapes)
        {
        if(s->bbox.contains(px, py))
            {
            std::cout << "\nClicking: " << s->getID();
            s->moving = 1;
            active = s;
            }
        }

    };

    void mouseMoveEvent(QMouseEvent* event) override 
    {
    if(active != nullptr)
        {
        if(active->moving == 1)
            {
            int x = event->pos().rx();
            int y = event->pos().ry();
            
            active->move(x, y);
            update();
            }
        }
    };

    void mouseReleaseEvent(QMouseEvent* event) override
    {
    int px = event->pos().rx();
    int py = event->pos().ry();

    if(active != nullptr)
        {
        if(active->bbox.contains(px, py))
            {
            std::cout << "\nStopped Moving: " << active->getID();
            
            active->moving = 0;
            active = nullptr;
            }
        }
    };

};

void WindowManager::add(Shape* shape)
{
shapes.push_back(shape);
}

void WindowManager::add(Edge* edge)
{
edges.push_back(edge);
}

Shape* WindowManager::getShape(int index)
{
return shapes[index];
}



int CpMain()
{
int h = 0;
QApplication app(h, {});

WindowManager winman;
winman.setWindowTitle("Graphy");

Circle* c1 = new Circle(150, 150, 25);
Circle* c2 = new Circle(50, 50, 25);
Edge* e = new Edge(c1, c2);
winman.add(c1);
winman.add(c2);
winman.add(e);

winman.setGeometry(100, 100, 400, 400);
winman.show();

QRectF f = winman.getShape(0)->bbox;

int result = app.exec();

return result;
}

