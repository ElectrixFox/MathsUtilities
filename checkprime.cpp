#include "Checkprime.h"

using namespace std;

int findPrime(vector<int> primes, int p)
{
// simple linear search
// could work far more efficiently using a binary search
for (int i = 0; i < primes.size(); i++)
    {
    if(primes[i] == p)
        {
        // returns prime index if prime
        return i;
        }
    }

// returns -1 if not as it will be a distinct value
return -1;
}
