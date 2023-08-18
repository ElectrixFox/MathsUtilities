#ifndef FACTOR_H
#define FACTOR_H

#include "Utility.h"
#include "Checkprime.h"

struct facPair { superint a, b; };

struct Factors
    {
    std::vector<superint> x;

    void flush() { using namespace std; x.erase(remove(x.begin(), x.end(), 1), x.end()); };
    superint compute() { superint res = 1; for (superint num : x) { res *= num; } return res; };
    std::string compress(int exponential = 0);
    };

struct Number
    {
    superint base;
    superint exponent;

    std::string getStr() { return (std::to_string(base) + "^" + std::to_string(exponent)); };
    };


superint outFactors(Factors f);
superint readPrimes();

facPair fermat(superint n);

superint baseFermat(superint n, std::vector<superint>& f);
Factors Factor(superint n);

std::vector<Number> exponentiate(std::vector<superint> nums);
std::vector<superint> remDuplicates(std::vector<superint> vec);

#endif