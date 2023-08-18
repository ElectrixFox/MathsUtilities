#include "FindPrimes.h"

using namespace std;

extern std::vector<superint> primes;

superint findClosestPrime(vector<superint> pnums, superint tofind, int higher)
{
superint current = pnums[0];

// can be improved by using a pseudo-binary search
for (superint i = 0; i < pnums.size()-1; i++)
    {
    long long int res1 = tofind - pnums[i];
    long long int res2 = tofind - pnums[i+1];

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

vector<superint> findPrimes(vector<superint> pnums, superint upto)
{
int size = upto;
int psize = pnums.size()-1;
vector<superint> numbers;

// the starting number of the loop
superint startnum = 1;

cout << "\nupto: " << upto;

// re calls the function at a smaller degree if the largest prime is smaller than the sqrt of the upto
// this enables more primes to be collected before
if(ceil(sqrt(upto)) > pnums[psize])
    {
    appendvec(pnums, findPrimes(pnums, ceil(sqrt(upto))));
    psize = pnums.size() - 1;
    }


// we can set the starting number to the largest prime we have since we know all of the primes before that
if(upto > pnums[psize]) startnum = pnums[psize];

// set numbers equal to pnums so that all of the current primes are already in the vector
numbers = pnums;

// go through all of the numbers up to the upper bound
for (superint i = startnum; i < size; i++)
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

    //printf("\n%f", );
    
    std::cout << "\nProgress: " << 100*((float)((float)i)/((float)size)) << "%";
    }


cout << '\n';
for (int i = 0; i < numbers.size(); i++)
    {
    cout << numbers[i] << ", ";
    }
cout << '\n';

return numbers;
}

vector<superint> getPrimes(string filePath)
{
vector<superint> pnums;

if(filePath == "")
    {
    vector<superint> p = {2, 3, 5, 7};
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

int writePrimes(string filePath, vector<superint> pnums)
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

int appendvec(vector<superint>& a, vector<superint> b)
{
for (int i = 0; i < b.size(); i++)
    {
    a.push_back(b[i]);
    }

return 1;
}