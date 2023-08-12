#ifndef FACTOR_H
#define FACTOR_H

#include "Utility.h"
#include "Checkprime.h"

struct facPair { int a, b; };

struct Factors
    {
    std::vector<int> x;

    void flush() { using namespace std; x.erase(remove(x.begin(), x.end(), 1), x.end()); };
    int compute() { int res = 1; for (int num : x) { res *= num; } return res; };
    std::string compress(int exponential = 0);
    };

struct Number
    {
    int base;
    int exponent;

    std::string getStr() { return (std::to_string(base) + "^" + std::to_string(exponent)); };
    };


int outFactors(Factors f);
int readPrimes();

facPair fermat(int n);

int baseFermat(int n, std::vector<int>& f);
Factors Factor(int n);

std::vector<Number> exponentiate(std::vector<int> nums);
std::vector<int> remDuplicates(std::vector<int> vec);

#endif