#include "Checkprime.h"

using namespace std;

extern std::vector<int> primes;

int findPrime(int p, std::vector<int> primenums)
{
// simple linear search
// could work far more efficiently using a binary search
for (int i = 0; i < primenums.size(); i++)
    {
    if(primenums[i] == p)
        {
        // returns prime index if prime
        return i;
        }
    }

// returns -1 if not as it will be a distinct value
return -1;
}

// shortcut that uses the extern prime variable
int findPrime(int p) { return findPrime(p, primes); }

int isPrime(int p)
{
// gets the result if it is found
int res = findPrime(p);

// will return 0 if 0 or 1 if not 0
return res % 2;
}
