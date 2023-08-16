#ifndef SHAPES_H
#define SHAPES_H

#include "Utility.h"
#include "qt_pch.h"
#include "Factor.h"

float pointLineDist(vec2 point, vec2 startPoint, vec2 endPoint);

class Text
    {
    public:
    // constructor to create the object, defaulting "" text so that it can be empty to start for future changing
    Text(vec2 pos, std::string content = "");

    // drawing
    void draw(QPainter* qp);

    void setPosition(vec2 pos) { position = pos; };

    void move(vec2 pos);

    std::string text = "";
    
    private:
    int parental = 0;

    vec2 position;
    };

class Shape
    {
    public:
    Shape();
    Shape(vec2 pos);
    Shape(vec2 pos, std::string col);
    Shape(vec2 pos, std::string col, std::string tex);

    // when implementing the text should be queried
    virtual void draw(QPainter* qp) {};

    vec2 coords(int print = 0);
    vec2* getCoords();

    virtual void move(vec2 to, int offset = 0);

    void setText(std::string tex) { text = new Text(position, tex); shouldShowText = 1; };
    void toggleShowText() { shouldShowText = !shouldShowText; };

    void changeColour(std::string col);
    void revertColour();

    int getID() { return id; };
    std::string getColour() { return colour; };

    QRectF bbox;

    int moving = 0;

    enum ShapeType
    {
    DEFAULT,
    LINE,
    CIRCLE,
    RECTANGLE,
    NODE
    };

    // to identify the shape
    ShapeType shapeType;

    protected:
    Text* text = nullptr;
    std::string textContent;
    int shouldShowText = 0;

    int id;
    vec2 position;
    std::string colour = "red";
    const std::string origionalColour; 
    };

class SRectangle : public Shape
    {
    public:
    SRectangle(vec2 pos, int wid, int hig, std::string col = "red", std::string tex = "");

    void draw(QPainter* qp) override;
    void setScale(vec2 scale);

    vec2 getScale(int print = 0);

    virtual void move(vec2 pos, int offset = 0) override;

    float updateOpacity(int op) { opacity = op; return opacity; };

    protected:
    int width, height;
    float opacity = 100.0f;

    int moving = 0;
    };

class Line : public Shape
    {
    protected:
        enum Type { SHAPE, VALUE };

    public:
    Line() { source = nullptr; target = nullptr; };
    Line(Shape* insource, Shape* intarget, int wid = 1);
    Line(vec2 insource, vec2 intarget, int wid = 1);

    // line draws different
    void draw(QPainter* qp) override;

    // overrides because of the different shapes
    void move(vec2 to, int offset = 0) override;
    int getWidth() { return width; };

    Type getType() { return type; };

    vec2 getSource() { return (type == Type::SHAPE) ? source->coords() : Nsource; };
    vec2 getTarget() { return (type == Type::SHAPE) ? target->coords() : Ntarget; };

    protected:
    int width = 1;

    Type type;

    // these are two separate for the moment
    vec2 Nsource;
    vec2 Ntarget;

    Shape* source;
    Shape* target;

    vec2 getMidp(vec2 startp, vec2 finishp) { return ((startp + finishp)/2); };
    float getLength(vec2 startp, vec2 finishp) { return ((finishp - startp)).mag(); };

    void drawForShape(QPainter* qp);

    };

class Circle : public Shape
    {
    public:
    Circle(vec2 pos, int radius, std::string col = "red", std::string tex = "");

    void draw(QPainter* qp) override;

    // overrides because of the different shapes
    virtual void move(vec2 pos, int offset = 0) override;

    int getRadius() { return radius; };

    protected:
    int radius;
    int moving = 0;
    };

class Node : public Circle
    {
    public:
    Node(vec2 pos, std::string label, int radius, std::string col = "red");

    void draw(QPainter* qp) override;

    // tells the edge what power it should be
    std::string preemptPower(std::string factor);

    std::string getText() { return textContent; };

    void addFactor(Number n);

    private:
    Line e;
    std::vector<Number> nums;
    };

#endif