#include "Shapes.h"

int n = 1;

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

// shape constructors

Shape::Shape() : position({0, 0}), origionalColour("red")
{
id = n;
n += 1;

shapeType = ShapeType::DEFAULT;
}

Shape::Shape(vec2 pos) : position(pos), origionalColour("red") { id = n; n += 1; shapeType = ShapeType::DEFAULT; };

Shape::Shape(vec2 pos, std::string col)
    : position(pos), origionalColour(col)
{
id = n;
n += 1;
shapeType = ShapeType::DEFAULT;
}

Shape::Shape(vec2 pos, std::string col, std::string tex)
    : position(pos), origionalColour(col), textContent(tex)
{
id = n;
n += 1;

text = new Text(position, tex);
shapeType = ShapeType::DEFAULT;
}


vec2 Shape::coords(int print)
{
if(print == 1)
    printf("\n(%f, %f)", position.x, position.y);

return { position };
}

vec2* Shape::getCoords()
{
return { &position };
}

void Shape::move(vec2 to, int offset)
{
position = to;

bbox.moveTo(position.x - offset, position.y - offset);
}

void Shape::changeColour(std::string col)
{
colour = col;
};

void Shape::revertColour()
{
changeColour(origionalColour);
}


// text

Text::Text(vec2 pos, std::string content)
    : position(pos)
{
// I don't like floats so I'm casting
position = pos;

parental = 1;

text = content;
}

void Text::move(vec2 pos)
{
position = pos;
}

void Text::draw(QPainter* qp)
{
// add options in the future for colour, size, font, etc...

// updating the position if the shape parent moves
/* if(parental != 0)
    x = parent->coords().x;
    y = parent->coords().y; */

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
if(parental != 0)
    xoffset = (floor(fwidth/2));

// writing the text
qp->drawText(position.x - xoffset, position.y + yoffset, text.c_str());
}


// rectangle

SRectangle::SRectangle(vec2 pos, int wid, int hig, std::string col, std::string tex)
{
position = pos;
id = n;
n += 1;

width = wid;
height = hig;

colour = col;

bbox = QRect(pos.x, pos.y, wid, hig);
shapeType = ShapeType::RECTANGLE;
}

void SRectangle::draw(QPainter* qp)
{
QColor col = QColor(colour.c_str());
qp->setBrush(col);

qp->drawRect(bbox);

// draw text if it is there
if(text != nullptr)
    text->draw(qp);
}

void SRectangle::move(vec2 pos, int offset)
{
position = pos;

// if there is text
if(text != nullptr)
    {
    text->setPosition(position);
    }
}


// circle

Circle::Circle(vec2 pos, int radius, std::string col, std::string tex) : Shape(pos, col, tex), radius(radius)
{
position = pos;
id = n;
n += 1;

colour = col;

int a = position.x - radius;
int b = position.y - radius;
int c = 2*radius;

bbox = QRectF(a, b, c, c);
shapeType = ShapeType::CIRCLE;
}

void Circle::draw(QPainter* qp)
{
QColor col = QColor(colour.c_str());
qp->setBrush(col);

qp->drawEllipse(bbox);

// draw text if it is there
if(text != nullptr)
    text->draw(qp);
}

void Circle::move(vec2 pos, int offset)
{
position = pos;

offset = radius;

// if there is text
if(text != nullptr)
    {
    text->setPosition(position);
    }

bbox.moveTo(position.x - offset, position.y - offset);
}


// line

Line::Line(Shape* insource, Shape* intarget, int wid)
    : Shape(), type(Type::SHAPE)
{
source = insource;
target = intarget;

width = wid;

shapeType = ShapeType::LINE;
}

Line::Line(vec2 insource, vec2 intarget, int wid)
    : Shape(), type(Type::VALUE)
{
Nsource = insource;
Ntarget = intarget;

width = wid;

shapeType = ShapeType::LINE;
}

void Line::drawForShape(QPainter* qp)
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

// to stop the division by 0 error of m when calcing x2-x1
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

// if there is text
if(text != nullptr)
    {
    // get midpoint of the line
    vec2 midp = (source->coords()+target->coords())/2;

    // move to the midp
    text->setPosition(midp);

    // draw the text
    text->draw(qp);
    }
}

void Line::draw(QPainter* qp)
{
// create a pen
QPen pen;

// set the width of the line
pen.setWidth(getWidth());

// set the pen
qp->setPen(pen);

// drawing the line
if(type == Type::SHAPE)
    drawForShape(qp);
else
    qp->drawLine(Nsource.x, Nsource.y, Ntarget.x, Ntarget.y);
}

void Line::move(vec2 to, int offset)
{
if(type == Type::SHAPE) { std::cout << "\nTrying to move a fixed line"; return; };

vec2 cur = getMidp(Nsource, Ntarget);
vec2 change = cur - to;

Nsource = Nsource + change;
Ntarget = Ntarget + change;
}


// node

Node::Node(vec2 pos, std::string label, int radius, std::string col)
    :   Circle(pos, radius, col, label)
{
shapeType = ShapeType::NODE;

};

void Node::draw(QPainter* qp)
{
//Circle::draw(qp);

QColor col = QColor(colour.c_str());
qp->setBrush(col);

qp->drawEllipse(bbox);

// draw text if it is there
if(text != nullptr)
    text->draw(qp);
}

std::string Node::preemptPower(std::string factor)
{
// for each number in nums
for(Number n : nums)
    {
    // if the base equals the factor
    if(n.base == atoi(factor.c_str()))
        {
        // return the exponent
        return (std::to_string(n.exponent));
        }
    }

// if it's not a factor return nullptr to show
return "";
}

void Node::addFactor(Number n)
{
nums.push_back(n);
}