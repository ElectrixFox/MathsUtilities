#include "CpGraph.h"

int n = 0;

class Shape
    {
    public:
        Shape() {};

        void draw() {};
    };

class Circle : public Shape, public QWidget
    {
    public:
        Circle(int x = 0, int y = 0, int radius = 1, QWidget* parent = nullptr) : QWidget(parent), x(x), y(y), radius(radius) { id = n; n++; };

        void paintEvent(QPaintEvent* event) override 
            {
            QPainter p(this);
            p.setRenderHint(QPainter::Antialiasing);

            draw(&p);
            };

        void draw(QPainter* qp)
            {
            // jargon to do different things
            int a = x - radius;
            int b = y - radius;
            int c = 2*radius;

            qp->setBrush(Qt::blue);

            qp->drawEllipse(a, b, c, c);
            };

    private:
        int x, y, radius;
        int moving = 0;

        int id;

    };

class WindowManager : public QWidget
    {
    public:
        WindowManager(QWidget* parent = nullptr) : QWidget(parent) { setMouseTracking(true); };

        std::vector<Shape> shapes;

    private:
        int id;

        void paintEvent(QPaintEvent* event) override 
        {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        for (int i = 0; i < shapes.size(); i++)
            {
            shapes[i].draw();
            }
        };

        
        void mousePressEvent(QMouseEvent* event) override 
        {
        event->button();
        int px = event->pos().rx();
        int py = event->pos().ry();

        std::cout << "\n(" << px << ", " << py << ")";
        std::cout << "\nClicking with " << id;
        };

        void mouseMoveEvent(QMouseEvent* event) override 
        {

        update();
        };

        void mouseReleaseEvent(QMouseEvent* event) override
        {

        update();
        };
        
    };

int CpMain()
{
int h;
QApplication app(h, {});

QWidget wind;
wind.setWindowTitle("Ellipse Drawing Example");
wind.setGeometry(100, 100, 400, 400);
wind.show();

WindowManager wman(&wind);

Circle c1(150, 150, 25, &wind);
c1.setGeometry(100, 100, 400, 400);
c1.show();

Circle c2(300, 300, 25, &wind);
c2.setGeometry(100, 100, 400, 400);
c2.show();

//wman.shapes.push_back(c1);
//wman.shapes.push_back(c2);

//wman.setGeometry(100, 100, 400, 400);
//wman.show();

return app.exec();
}

