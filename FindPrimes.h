#ifndef FINDPRIMES_H
#define FINDPRIMES_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>

typedef unsigned int superint;

std::vector<superint> getPrimes(std::string filePath = "");

std::vector<superint> findPrimes(std::vector<superint> pnums, superint upto);

int appendvec(std::vector<superint>& a, std::vector<superint> b);

// if higher = -1 then it will find the lower
superint findClosestPrime(std::vector<superint> pnums, superint tofind, int higher = 0);

int writePrimes(std::string filePath, std::vector<superint> pnums);

#endif