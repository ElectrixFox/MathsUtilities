#ifndef UTILITY_H
#define UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <algorithm>
#include <sstream>
#include <regex>

class vec2
    {
    public:
    vec2() { x = 0; y = 0; };
    vec2(float X, float Y) { x = X; y = Y; };
    float x, y; 

    // operation for vector subtraction
    vec2 operator-(const vec2& other) const
        {
        return {(x - other.x), (y - other.y)};
        };

    // operation for vector additions
    vec2 operator+(const vec2& other) const
        {
        return {(x + other.x), (y + other.y)};
        };
    
    // operation for dividing a vector by a constant
    vec2 operator/(const int divisor) const
        {
        return {(x/divisor), (y/divisor)};
        };
    
    float mag() { return sqrt(pow(x, 2) + pow(y, 2)); };

    void out() { printf("\n(%f, %f)", x, y); };
    };

std::vector<int> loadPrimes(std::string filePath = "");

std::vector<int> sort(std::vector<int> arr);



#endif