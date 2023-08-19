#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Utility.h"
#include "qt_pch.h"
#include "Factor.h"

#include "Shapes.h"
#include "Composite.h"
#include "GraphicWindow.h"

class NodeEditor : public QWidget
    {
    public:
    NodeEditor(QWidget* parent = nullptr);

    void InitialiseMenu();

    void CallUpdate();
    void CallUpdate(Node* node);

    private:
    Node* activeNode;

    // menu stuff
    private:
    QLabel* label_inColour;
    QLabel* label_nodeNumber;

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

    Q_OBJECT
    
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
    void updNodeCol();
    void updEditorUI();

    private:
    std::vector<QWidget*> widgets;
    QVBoxLayout* dockinglayout;
    QLineEdit* inNum;
    NodeEditor* nodeEditor;
    Table* table;

    struct gui_nodeInfo
        {
        Node* node_active;

        QLabel* label_inColour;
        QLineEdit* inColour;
        
        QLabel* label_nodeNumber;
        QLineEdit* inNumber;
        };

    gui_nodeInfo nodeinfo_current;

    void resizeElement(QWidget* widget);
    void resizeButton(QPushButton* lebutton);

    void updateNodeColour();
    
    };

int CpMain();


#endif