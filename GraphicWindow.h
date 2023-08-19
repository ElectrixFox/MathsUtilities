#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include "Utility.h"
#include "qt_pch.h"
#include "Factor.h"

#include "Shapes.h"
#include "Composite.h"

class GraphicWindow : public QWidget
    {
    public: 
    GraphicWindow(QWidget* parent = nullptr) : QWidget(parent) { setMouseTracking(true); };

    // add a shape to the window's collection
    void add(Shape* shape);

    // get a shape at the index
    Shape* getShape(int index);

    // clear all shapes
    void remAll();

    // toggle whether the factors can connect
    int toggleConnect();

    // returns the minimum radius of a node to contain all of the text 
    int getMinimumRadius(std::string nodeText);

    protected:
    // vector position of the pointer
    vec2 pPos;

    std::vector<Shape*> shapes;

    std::vector<Shape*> selected = {nullptr};
    Shape* active = selected[0];
    SRectangle* boxSel = nullptr;
    std::string prevcol = "";

    // eventually change these into bit flags
    int factorConnect = 0;
    int pressedShape = 0;
    int boxSelecting = 0;
    int lineDeleted = 0;

    void paintEvent(QPaintEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;    
    void contextMenuEvent(QContextMenuEvent* event) override;

    vec2 getPointerPos(QMouseEvent* event);
    vec2 getPointerPos(QContextMenuEvent* event);

    // function to create a node at the pointer
    void createNode();

    // function to handle the majority of the main box select functionalities
    void boxSelect();

    // select a group of items
    void groupSelect();

    // deselect all 
    void groupDeselect();

    // select a single shape
    void Select(Shape* s);

    // select a shape via the shape index
    void Select(int index);
    
    // deselect the active shape
    void Deselect();

    // is the pointer colliding with anything, if so return the index of the shape
    int Colliding(vec2 mPos);

    // shape dragging function
    void moveShape(QMouseEvent* event);
    };

class Table : public QTableWidget
    {
    public:
    Table(QWidget* parent = nullptr);
    
    void add(int column, std::string item, int startnewrow = 0);
    void add(vec2 tablePos, std::string item);

    void newRow();

    void minimizeTable();

    void Clear();

    int getLocalTop() { localTop = rowCount() - 1; return localTop;};

    private:
    int localTop = 0;
    std::vector<QTableWidgetItem*> tableItems;
    

    };

int loadnew(GraphicWindow* gwin, DetailContainer* dc, Table* table);

class MainWindow : public QMainWindow
    {
    public:
    MainWindow(QWidget* parent = nullptr, GraphicWindow* graphicWindow = nullptr);

    GraphicWindow* gwin;
    DetailContainer dc;

    public slots:
    void pressy();
    void remAll();
    void uniqueFactors();
    void onlyFactors();
    void testIsPrime();
    void createNode();


    private:
    std::vector<QWidget*> widgets;
    QVBoxLayout* dockinglayout;
    QLineEdit* inNum;
    Table* table;

    QLineEdit* colourInput;
    QLineEdit* internalLabel;

    void resizeElement(QWidget* widget);
    void resizeButton(QPushButton* lebutton);
    };

int CpMain();


#endif