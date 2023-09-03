#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent, GraphicWindow* graphicWindow)
    : QMainWindow(parent), gwin(graphicWindow)
{
connect(gwin, SIGNAL(eventOccurred()), this, SLOT(updEditorUI()));

#pragma region MenuBar
// menu bar setup
QMenuBar* menubar = new QMenuBar(this);
setMenuBar(menubar);

// create the menu item and make it clickable
QAction* uniFac = new QAction("Unique Factors", this);
uniFac->setCheckable(1);
uniFac->setChecked(0);

// create the menu item and make it clickable
QAction* onlyFacs = new QAction("Only Factors", this);
onlyFacs->setCheckable(1);
onlyFacs->setChecked(1);

// add the options main menu
QMenu* optionsMenu = new QMenu("Options");
menubar->addMenu(optionsMenu);
optionsMenu->addAction(uniFac);
optionsMenu->addAction(onlyFacs);

// connect up all of the actions
connect(uniFac, &QAction::triggered, this, uniqueFactors);
connect(onlyFacs, &QAction::triggered, this, onlyFactors);

#pragma endregion

// setting up the main widget
QDockWidget* dockWidget = new QDockWidget("Side pannel", this);

// creating its' content widget
QWidget* dockContent = new QWidget(dockWidget);

// setting the docking widget's content up
dockWidget->setWidget(dockContent);

// setting up the layout of the widget
dockinglayout = new QVBoxLayout(dockContent);
QHBoxLayout* buttonlayout = new QHBoxLayout();
QHBoxLayout* numberCheckLayout = new QHBoxLayout();
QVBoxLayout* tablelayout = new QVBoxLayout();
QVBoxLayout* nodeInfoLayout = new QVBoxLayout();

// add the widget to the window
addDockWidget(Qt::RightDockWidgetArea, dockWidget);

// create the table
table = new Table();

// button setup
QPushButton* loadNewButton = new QPushButton("Load new set");
QPushButton* resetButton = new QPushButton("Reset");

inNum = new QLineEdit();
inNum->setPlaceholderText("Enter Number to check");
QPushButton* submit = new QPushButton("Submit");

/*// create the new node
QPushButton* createNewNode = new QPushButton("Create new Node");

// all of the stuff for changing the colour
QLabel* label_inColour = new QLabel("Colour: ");
QLineEdit* inColour = new QLineEdit();
inColour->setPlaceholderText("Enter Colour");

QLabel* label_nodeNumber = new QLabel("Number: ");
QLineEdit* inNumber = new QLineEdit();
inNumber->setPlaceholderText("Enter Number");

nodeinfo_current = { nullptr, label_inColour, inColour, label_nodeNumber, inNumber };*/

nodeEditor = new NodeEditor();

// resizing the buttons
resizeButton(loadNewButton);
resizeButton(resetButton);
resizeButton(submit);

// adding them all to here to stop them having to be individually stored in the class
widgets.push_back(dockContent);

// adding both widgets to the dock
buttonlayout->addWidget(loadNewButton);
buttonlayout->addWidget(resetButton);
numberCheckLayout->addWidget(inNum);
numberCheckLayout->addWidget(submit);
nodeInfoLayout->addWidget(nodeEditor);
tablelayout->addWidget(table);


// create the separating line
QFrame *separator1 = new QFrame;
separator1->setFrameShape(QFrame::HLine);
separator1->setFrameShadow(QFrame::Sunken);

// create the separating line
QFrame *separator2 = new QFrame;
separator2->setFrameShape(QFrame::HLine);
separator2->setFrameShadow(QFrame::Sunken);

// create the separating line
QFrame *separator3 = new QFrame;
separator3->setFrameShape(QFrame::HLine);
separator3->setFrameShadow(QFrame::Sunken);


// add the layout's together
dockinglayout->addLayout(buttonlayout, 1);
dockinglayout->addWidget(separator1);
dockinglayout->addLayout(numberCheckLayout);
dockinglayout->addWidget(separator2);
dockinglayout->addLayout(nodeInfoLayout);
dockinglayout->addWidget(separator3);
dockinglayout->addLayout(tablelayout);


// setting the dock layout
dockContent->setLayout(dockinglayout);

// setting the main widget of the dock
dockWidget->setWidget(dockContent);

// connecting the button to the window so that it can be used
connect(loadNewButton, &QPushButton::clicked, this, pressy);
connect(resetButton, &QPushButton::clicked, this, remAll);
connect(submit, &QPushButton::clicked, this, testIsPrime);
};

void MainWindow::updateNodeColour()
{
// getting the input colour as a string
std::string col = nodeinfo_current.inColour->text().toStdString();

// changing the node colour to the inputted colour
nodeinfo_current.node_active->changeColourOrigional(col);

std::string labelText = "Colour: " + col;

nodeinfo_current.label_inColour->setText(labelText.c_str());

update();
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

void MainWindow::testIsPrime()
{
std::string inputtxt = inNum->text().toStdString();

unsigned int in = atoi(inputtxt.c_str());

// variable to record if it is prime
int Isprime = isPrime(in);

// if it is prime turn the background light green
if(Isprime == 1) inNum->setStyleSheet("background-color: rgba(187, 255, 181, 1);");
// clear the background
else inNum->setStyleSheet("background-color: none;");

QTimer t;

// clear the background after 1 second
t.singleShot(1000, ([&]{inNum->setStyleSheet("background-color: none;");}));
}

void MainWindow::createNode()
{
std::string col = nodeinfo_current.inColour->text().toStdString();
std::string internalLab = nodeinfo_current.inNumber->text().toStdString();

// get the string as a number
superint number = atoi(internalLab.c_str());

// factor the inputted number
Factors fs = BruteFactor(number);

// getting the factors as numbers
std::vector<Number> nums = fs.getAsNumbers();

// increment the top
dc.incrementTop();

// create the detail to push
Detail det = {number, nums};

// pushing back a detail to the detail container
dc.pushTop(det);

// add a new row before adding
table->newRow();

// add the number in the first position
table->add(0, internalLab, 0);

// get only the unique factors
table->add(1, fs.unique(), 0);

// add the compressed exponential form of the factors to the table
table->add(2, fs.compress(1), 0);

// getting the minimum radius of the node
int minrad = gwin->getMinimumRadius(internalLab);

// create the node of the base number
Node* numberNode = new Node({50, 50}, internalLab, minrad, "light green");

// add the factors to the node
numberNode->addFactors(fs);

gwin->add(numberNode);

// a count variable to count iterations for spacing of nodes
int count = 0;

for (Number i : nums)
    {
    // if the base has already been and the factors have to be unique then continue to the next number
    if(dc.hasBeen(i.base) == 1 && dc.getUniqueFactor() == 1) continue;

    // getting the base as a string
    std::string strnum = std::to_string(i.base);

    // calculation for getting the minimum radius
    minrad = gwin->getMinimumRadius(strnum);

    // creating the node
    Node* n = new Node({(float)(100 + count*50), (float)50}, strnum, minrad, "light blue");

    // adding the node to the graphics window
    gwin->add(n);

    count++;
    }

update();
}

void MainWindow::updNodeCol()
{
Shape* curshape = gwin->getActive();

// if there isn't a shape return
if(curshape == nullptr) return;

// if the active shape isn't a node return
if(curshape->shapeType != Shape::ShapeType::NODE) return;

// set the nodeinfo node as active
nodeinfo_current.node_active = (Node*)curshape;

updateNodeColour();
}

void MainWindow::updEditorUI()
{
std::cout << "\nUpdating UI";

if(gwin->getActive() == nullptr) return;

nodeEditor->CallUpdate((Node*)gwin->getActive());
}

void MainWindow::resizeElement(QWidget* widget)
{
widget->resize(widget->sizeHint().width(), widget->sizeHint().height());
}

void MainWindow::resizeButton(QPushButton* lebutton)
{
lebutton->setFixedSize(lebutton->sizeHint().width(), lebutton->sizeHint().height());
}




Table::Table(QWidget* parent)
    : QTableWidget(parent)
{
// setting the rows and the columns
this->setRowCount(0);
this->setColumnCount(3);

resizeRowsToContents();
resizeColumnsToContents();

this->setSizeAdjustPolicy(SizeAdjustPolicy::AdjustToContents);
this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
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

void Table::add(int column, std::string item, int startnewrow)
{
QTableWidgetItem* element = new QTableWidgetItem(item.c_str());

// row count
int rc = rowCount();

// add a new row if starting new row
if(startnewrow == 1)
    setRowCount(rc+1);

// updating the local top
localTop = rc - 1;

// setting the new item
setItem(localTop, column, element);

// resizing
resizeRowsToContents();
resizeColumnsToContents();

// updating the table
update();

// adding the element to the list
tableItems.push_back(element);
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

void Table::newRow()
{
// get the row count
int rc = rowCount();

// set the new row count to the current + 1
setRowCount(rc+1);

// resizing
resizeRowsToContents();
resizeColumnsToContents();

// updating the table
update();
}

void Table::Clear()
{
// clears the table
clearContents();

// resets the row count so that additions start from index 0 again
setRowCount(0);
}



NodeEditor::NodeEditor(QWidget* parent)
{
InitialiseMenu();
}

void NodeEditor::InitialiseMenu()
{
QGridLayout* editorLayout =  new QGridLayout(this);;

// create the new node
QPushButton* createNewNode = new QPushButton("Create new Node");

// all of the stuff for changing the colour
label_inColour = new QLabel("Colour: ");
inColour = new QLineEdit();
inColour->setPlaceholderText("Enter Colour");

label_nodeNumber = new QLabel("Number: ");
inNumber = new QLineEdit();
inNumber->setPlaceholderText("Enter Number");

//connect(createNewNode, &QPushButton::clicked, this, createNode);

editorLayout->addWidget(label_inColour, 0, 0);
editorLayout->addWidget(inColour, 1, 0);
editorLayout->addWidget(label_nodeNumber, 2, 0);
editorLayout->addWidget(inNumber, 3, 0);

connect(inColour, &QLineEdit::returnPressed, this, updNode);
connect(inNumber, &QLineEdit::returnPressed, this, updNode);

update();
}


void NodeEditor::updNode()
{
if(activeNode == nullptr) return;

// the text from the number input
std::string numText = inNumber->text().toStdString();

// text from the colour input
std::string colText = inColour->text().toStdString();


// if the textbox is empty then don't update the text
if(inNumber->text().isEmpty() != 1)
    activeNode->setText(numText);

// if the textbox isn't empty change the colour
if(inColour->text().isEmpty() != 1)
    {
    // change the node's origional colour
    activeNode->changeColourOrigional(colText);

    // change the colour of the node
    activeNode->changeColour(colText);
    }

CallUpdate();
}

void NodeEditor::CallUpdate()
{
// getting the new node's colour
std::string newColour = "Colour: " + activeNode->getColour();

// setting the label for the editor
label_inColour->setText(newColour.c_str());


// getting the number of the new node
std::string newNumber = "Number: " + activeNode->getText();

// setting the new number
label_nodeNumber->setText(newNumber.c_str());

update();
}

void NodeEditor::CallUpdate(Node* node)
{
// if the clicked node isn't currently active in the editor clear the options
if(activeNode != node) { inColour->clear(); inNumber->clear(); };

// setting the new active node
activeNode = node;

// call the UI updates
CallUpdate();
}


int loadnew(GraphicWindow* gwin, DetailContainer* dc, Table* table)
{
int curRow = table->getLocalTop();

Detail detail = dc->getTop();

// composite
std::string compnum = std::to_string(detail.number);

int minrad = gwin->getMinimumRadius(compnum);

// create the node of the base number
Node* number = new Node({50, 50}, compnum, minrad, "light green");
gwin->add(number);

// create a new row for the values
table->newRow();

// get this again because we're making a new row
curRow = table->getLocalTop();

// adding the number to the table
table->add(0, compnum);

Factors factors = getAsFactors(detail.factors);

// adding all of the factors to the number node
number->addFactors(factors);

// a collection of the factors
std::stringstream ss[2];

int count = 0;
for (Number i : detail.factors)
    {
    // if the base has already been and the factors have to be unique then continue to the next number
    if(dc->hasBeen(i.base) == 1 && dc->getUniqueFactor() == 1) continue;

    // doing this down here to save processing time
    std::string strnum = std::to_string(i.base);
    ss[0] << strnum << ", ";

    // calculation for getting the minimum radius
    minrad = gwin->getMinimumRadius(strnum);

    Node* n = new Node({(float)(100 + count*50), (float)50}, strnum, minrad, "light blue");
    
    gwin->add(n);

    count++;
    }

// the stream as a string
std::string compactStr = ss[0].str();

// the final string -2 as it gets rid of the extra ", "
std::string final = compactStr.substr(0, compactStr.length()-2);

// add the factors to the second row
table->add({(float)curRow, 1}, final);

// add the multiplication to the exponential component
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

int CpMain()
{
int h = 0;
QApplication app(h, {});

GraphicWindow gwin;
MainWindow* window = new MainWindow(nullptr, &gwin);

app.setActiveWindow(window);

//EventsManager& em = EventsManager::instance();

window->gwin = &gwin;

window->setWindowTitle("Graphy");
window->setCentralWidget(&gwin);

window->setGeometry(200, 200, 800, 800);

window->show();

int result = app.exec();

return result;
}
