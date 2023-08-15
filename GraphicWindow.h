#ifndef GRAPHICWINDOW_H
#define GRAPHICWINDOW_H

#include "Utility.h"
#include "qt_pch.h"
#include "Factor.h"

#include "CpGraph.h"

class GraphicWindow : public QWidget
    {
    public: 
    GraphicWindow(QWidget* parent = nullptr) : QWidget(parent) { setMouseTracking(true); };

    void add(Shape* shape);

    Shape* getShape(int index);

    protected:
    std::vector<Shape*> shapes;

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

int loadnew(GraphicWindow* gwin, Table* table);

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

int CpMain();


#endif