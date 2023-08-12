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


std::vector<int> getPrimes(std::string filePath = "");

std::vector<int> findPrimes(std::vector<int> pnums, int upto);
int appendvec(std::vector<int>& a, std::vector<int> b);

// if higher = -1 then it will find the lower
int findClosestPrime(std::vector<int> pnums, int tofind, int higher = 0);

int writePrimes(std::string filePath, std::vector<int> pnums);

#endif