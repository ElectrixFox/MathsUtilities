#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "Utility.h"
#include "Factor.h"

struct Detail
{
superint number;

std::vector<Number> factors;
};

class DetailContainer
    {
    public:
    DetailContainer() { loadData("Composite.txt"); };

    Detail getTop();
    const int getTopV() const { return top; };
    void incrementTop() { top++; };

    int toggleUniqueFactor() { uniFactors ^= 1; return uniFactors; };
    int getUniqueFactor() { return uniFactors; };

    void Reset() { top = 0; };

    // has the factor already been
    int hasBeen(int value);

    // print the top value
    void printTopV() { printf("\n%d: ", numbers[top]); for (int i = 0; i < factors[top].size(); i++)
    {
    printf("%d^%d", factors[top][i], exponents[top][i]);

    if(i+1 != factors[top].size()) printf(" * ");
    }
     };


    private:
    int top = 0;
    int uniFactors = 0;

    std::vector<superint> numbers;
    std::vector<std::vector<superint>> factors;
    std::vector<std::vector<superint>> exponents;

    Detail getAt(int n);

    void loadData(const std::string& filePath);
    };

#endif