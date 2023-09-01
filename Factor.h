#ifndef FACTOR_H
#define FACTOR_H

#include "Utility.h"
#include "Checkprime.h"

struct facPair { superint a, b; };

struct Number
    {
    superint base;
    superint exponent;

    std::string getStr() { return (std::to_string(base) + "^" + std::to_string(exponent)); };
    };

struct Factors
    {
    // holds all of the factors in unsimplified form e.g. 4 = 2 * 2 not 2^2
    std::vector<superint> x;

    void flush() { using namespace std; x.erase(remove(x.begin(), x.end(), 1), x.end()); };
    superint compute() { superint res = 1; for (superint num : x) { res *= num; } return res; };
    std::string compress(int exponential = 0);

    // returns the unique integers
    std::vector<superint> uniqueI();

    // returns the unique factors as a string of numbers separated by the separator
    std::string unique(std::string separator = ", ");

    std::vector<Number> getAsNumbers();
    };

Factors getAsFactors(std::vector<Number> numbers);

superint outFactors(Factors f);
superint readPrimes();

Factors BruteFactor(superint n);

facPair fermat(superint n);

superint baseFermat(superint n, std::vector<superint>& f);
Factors Factor(superint n);

std::vector<Number> exponentiate(std::vector<superint> nums);
std::vector<superint> remDuplicates(std::vector<superint> vec);

#endif