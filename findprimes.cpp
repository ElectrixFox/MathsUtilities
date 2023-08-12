#include "FindPrimes.h"

using namespace std;

extern std::vector<int> primes;

int findClosestPrime(vector<int> pnums, int tofind, int higher)
{
int current = pnums[0];

// can be improved by using a pseudo-binary search
for (int i = 0; i < pnums.size()-1; i++)
    {
    int res1 = tofind - pnums[i];
    int res2 = tofind - pnums[i+1];

    // if it has to be lower then it won't be the absolute
    if(higher == 0)
        {
        res1 = abs(res1);
        res2 = abs(res2);
        }

    if(higher == -1 && res2 < 0)
        return pnums[i];
    if(higher == 1 && res2 < 0)
        return pnums[i+1];

    // if x - p_n < x - p_n+1 then p_n will be further away from x 
    if(res1 <= res2)
        {
        return pnums[i];
        }
    }


cout << "\nNot in range";

return pnums[pnums.size()];
}

vector<int> findPrimes(vector<int> pnums, int upto)
{
int size = upto;
int psize = pnums.size()-1;
vector<int> numbers;

cout << "\nupto: " << upto;

if(ceil(sqrt(upto)) > pnums[psize])
    {
    appendvec(pnums, findPrimes(pnums, ceil(sqrt(upto))));
    psize = pnums.size() - 1;
    }


// go through all of the numbers up to the upper bound
for (int i = 1; i < size; i++)
    {
    int passcount = 0;

    for (int j = 0; j < psize; j++)
        {
        // checking for any multiples
        if(i % pnums[j] == 0 && i != pnums[j])
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
vector<int> pnums;

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
        pnums.push_back(atoi(line.c_str()));
    }

in.close();

return pnums;
}

int writePrimes(string filePath, vector<int> pnums)
{
ofstream out(filePath, ios::out);

for (int i = 0; i < pnums.size(); i++)
    {
    string expr = to_string(pnums[i]) + "\n";
    out.write(expr.c_str(), expr.size());
    }

out.close();

return 1;
}

int appendvec(vector<int>& a, vector<int> b)
{
for (int i = 0; i < b.size(); i++)
    {
    a.push_back(b[i]);
    }

return 1;
}