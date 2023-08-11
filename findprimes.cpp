#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <fstream>
#include <string>
#include <vector>

using namespace std;

const int baseP[] = {2, 3, 5, 7};

vector<int> getPrimes(string filePath = "");

vector<int> findPrimes(vector<int> primes, int upto);
int appendvec(vector<int>& a, vector<int> b);

int findClosestPrime(vector<int> primes, int tofind, int lower = 0);

int writePrimes(string filePath, vector<int> primes);


int main(int argc, char const *argv[])
{
int num;
int c = 0, w = 0;

cout << "Enter Number of Primes up to: ";
cin >> num;

cout << "File: ";
cin >> c;

vector<int> primes;

if(c == 0)
    primes = getPrimes();
else
    primes = getPrimes("primes.txt");

vector<int> p2 = findPrimes(primes, num);

cout << "\nWrite: ";
cin >> w;

if(w == 1)
    writePrimes("primes.txt", p2);

return 0;
}

int findClosestPrime(vector<int> primes, int tofind, int lower)
{
int current = primes[0];

// can be improved by using a pseudo-binary search
for (int i = 0; i < primes.size()-1; i++)
    {
    int res1 = tofind - primes[i];
    int res2 = tofind - primes[i+1];

    // if it has to be lower then it won't be the absolute
    if(lower != 1)
        {
        res1 = abs(res1);
        res2 = abs(res2);
        }

    // if x - p_n < x - p_n+1 then p_n will be further away from x 
    if(res1 <= res2 || res2 < 0)
        {
        return primes[i];
        }
    }


cout << "\nNot in range";

return primes[primes.size()];
}

vector<int> findPrimes(vector<int> primes, int upto)
{
int size = upto;
int psize = primes.size()-1;
vector<int> numbers;

cout << "\nupto: " << upto;

if(ceil(sqrt(upto)) > primes[psize])
    {
    appendvec(primes, findPrimes(primes, ceil(sqrt(upto))));
    psize = primes.size() - 1;
    }


// go through all of the numbers up to the upper bound
for (int i = 1; i < size; i++)
    {
    int passcount = 0;

    for (int j = 0; j < psize; j++)
        {
        // checking for any multiples
        if(i % primes[j] == 0 && i != primes[j])
            {
            break;
            }
        passcount++;
        }
    
    if(passcount == psize && i > 1)
        {
        numbers.push_back(i);
        }
    }


cout << '\n';
for (int i = 0; i < numbers.size(); i++)
    {
    cout << numbers[i] << ", ";
    }
cout << '\n';

return numbers;
}


vector<int> getPrimes(string filePath)
{
vector<int> primes;

if(filePath == "")
    {
    vector<int> p = {2, 3, 5, 7};
    return p;
    }

ifstream in(filePath, ios::in);

string line;
while(getline(in, line))
    {
    if(line != "\n")
        primes.push_back(atoi(line.c_str()));
    }

in.close();

return primes;
}

int writePrimes(string filePath, vector<int> primes)
{
ofstream out(filePath, ios::out);

for (int i = 0; i < primes.size(); i++)
    {
    string expr = to_string(primes[i]) + "\n";
    out.write(expr.c_str(), expr.size());
    }

out.close();
}

int appendvec(vector<int>& a, vector<int> b)
{
for (int i = 0; i < b.size(); i++)
    {
    a.push_back(b[i]);
    }

}