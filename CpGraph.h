#include "Utility.h"
#include <QtWidgets/QApplication.h>
#include <QtWidgets/QPushbutton.h>
#include <QtWidgets/QMainWindow>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QLabel>
#include <QtWidgets/qbuttongroup.h>
#include <QtGui/QPainter>
#include <QtCore/QElapsedTimer>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QMenu>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QVBoxLayout>

struct vec2 { float x, y; };

void loadData(const std::string& filePath);

float pointLineDist(vec2 point, vec2 startPoint, vec2 endPoint);

class Shape
    {
    public:
    Shape(int inx, int iny);
    Shape(int inx, int iny, std::string colour);

    virtual void draw(QPainter* qp) {};

    vec2 coords(int print = 0);
    virtual void move(int xto, int yto, int offset = 0);

    void changeColour(std::string col);
    void revertColour();

    int getID() { return id; };
    std::string getColour() { return colour; };

    QRectF bbox;

    int moving = 0;

    protected:
    int id;
    int x, y;
    std::string colour = "red";
    const std::string origionalColour; 
    };

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
    Circle(int inx, int iny, int radius, std::string col = "red");

    void draw(QPainter* qp) override;

    // overrides because of the different shapes
    virtual void move(int xto, int yto, int offset = 0) override;

    int getID() { return id; };
    int getRadius() { return radius; };

    protected:
    int radius;
    int moving = 0;
    };

class Node
    {
    public:
    Node(vec2 pos, std::string label, int radius, std::string col = "red");

    void draw(QPainter* qp);

    vec2 coords(int print = 0) { return cir->coords(print); };

    Shape* getShape();
    Text* getText();

    private:
    vec2 position;
    Text* tex;
    Circle* cir;
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

    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    };

class Table : public QTableWidget
    {
    public:
    Table(QWidget* parent = nullptr);

    void add(vec2 tablePos, std::string item);

    private:

    std::vector<QTableWidgetItem*> tableItems;
    

    };

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
    QVBoxLayout* dockinglayout;
    QMenu* fileMen;
    Table* table;
    };

int loadnew(GraphicWindow* gwin, Table* table);

int CpMain();