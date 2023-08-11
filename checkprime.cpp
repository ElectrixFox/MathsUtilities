#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<int> loadPrimes(string filePath = "");
int findPrime(vector<int> primes, int p);

int main(int argc, char const *argv[])
{
int num;
vector<int> primes;

primes = loadPrimes("primes.txt");

cin >> num;

if(findPrime(primes, num) != 0)
    {
    printf("\n%d is a prime", num);
    }
else
    {
    printf("\n%d is not a prime", num);
    }

return 0;
}

vector<int> loadPrimes(string filePath = "")
{
ifstream fin(filePath, ios::in);
vector<int> primes;
string line;

// loops through and if the line isn't a new line it converts the line to an int and adds it to the primes vector
while(getline(fin, line))
    {
    if(line != "\n")
        primes.push_back(atoi(line.c_str()));
    }

fin.close();

return primes;
}

int findPrime(vector<int> primes, int p)
{

// simple linear search
// could work far more efficiently using a binary search
for (int i = 0; i < primes.size(); i++)
    {
    if(primes[i] == p)
        {
        return i;
        }
    }

return -1;
}
