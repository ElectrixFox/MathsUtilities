#include "Checkprime.h"

using namespace std;

extern std::vector<superint> primes;

superint findPrime(superint p, std::vector<superint> primenums)
{
// simple linear search
// could work far more efficiently using a binary search
for (superint i = 0; i < primenums.size(); i++)
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
superint findPrime(superint p) { return findPrime(p, primes); }

int isPrime(superint p)
{
// returns 1 if it is found
for (int i = 0; i < primes.size(); i++)
    {
    if(primes[i] == p)
        return 1;
    }

return 0;
}
