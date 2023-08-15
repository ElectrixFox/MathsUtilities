#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "Utility.h"
#include "qt_pch.h"
#include "Factor.h"

struct Detail
{
int number;

std::vector<Number> factors;
};

class DetailContainer
    {
    public:
    DetailContainer() { loadData("Composite.txt"); };

    Detail getTop();
    void incrementTop() { top++; };

    int toggleUniqueFactor() { uniFactors ^= 1; return uniFactors; };
    int getUniqueFactor() { return uniFactors; };

    // has the factor already been
    int hasBeen(int value);


    private:
    int top = 0;
    int uniFactors = 0;

    std::vector<int> numbers;
    std::vector<std::vector<int>> factors;
    std::vector<std::vector<int>> exponents;

    Detail getAt(int n);

    void loadData(const std::string& filePath);
    };

#endif