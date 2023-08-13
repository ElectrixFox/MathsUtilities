#include "CpGraph.h"

int n = 0;

class Shape
{
public:
    Shape(int inx, int iny);

    virtual void draw(QPainter* qp) {};
    
    void coords() { printf("\n(%d, %d)", x, y); };

protected:
    int id;
    int x, y;
};

Shape::Shape(int inx, int iny) : x(inx), y(iny) {};

class Circle : public Shape
{
public:
    Circle(int inx, int iny, int radius);

    void draw(QPainter* qp) override;

    int x, y, radius;
protected:
    int id;
    int moving = 0;
};

Circle::Circle(int inx, int iny, int radius) : Shape(inx, iny), radius(radius)
{
x = inx;
y = iny;
id = n;
n += 1;
}

void Circle::draw(QPainter* qp)
{
// jargon to do different things
int a = x - radius;
int b = y - radius;
int c = 2*radius;

qp->setBrush(Qt::red);

qp->drawEllipse(a, b, c, c);

}

class WindowManager : public QWidget
{
public: 
    WindowManager(QWidget* parent = nullptr) : QWidget(parent) { setMouseTracking(true); };

    std::vector<Shape*> shapes;

protected:
    void paintEvent(QPaintEvent* event) override 
    {    
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    Circle c(250, 250, 40);
    c.draw(&p);

    shapes[0]->draw(&p);

    /* for (Shape s : shapes)
        {
        s.coords();
        s.draw(&p);
        } */
    };

    void mousePressEvent(QMouseEvent* event) override 
    {
    std::cout << this->paintingActive();
    
    event->button();
    int px = event->pos().rx();
    int py = event->pos().ry();

    std::cout << "\n(" << px << ", " << py << ")";


    /* for (int i = 0; i < shapes.size(); i++)
        {
        if(shapes[i].bbox.contains(px, py))
            {
            //std::cout << "\nClicking: " << shapes[i].id;
            }
        } */
    };

/*         void mouseMoveEvent(QMouseEvent* event) override 
        {

        update();
        };

        void mouseReleaseEvent(QMouseEvent* event) override
        {

        update();
        }; */

};



int CpMain()
{
int h = 0;
QApplication app(h, {});

/* QWidget wind;
wind.setWindowTitle("Ellipse Drawing Example");
wind.setGeometry(100, 100, 400, 400);
wind.show(); */

WindowManager winman;

Circle c1(150, 150, 10);
Circle c2(0, 0, 25);
winman.shapes.push_back(&c1);
winman.shapes.push_back(&c2);

winman.setGeometry(100, 100, 400, 400);
winman.show();

int result = app.exec();

return result;
}

