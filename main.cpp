#include "Utility.h"
#include "Factor.h"
#include "ConsecutiveComposites.h"

std::vector<int> primes;

int main(int argc, char const *argv[])
{
primes = loadPrimes("primes.txt");

for (int i = 5; i < 25; i++)
    {
    std::vector<Factors> facs = genComposites(primes[i]+1, primes[i+1]-1);
    writeCompositesToFile(facs, 1);
    }


return 0;
}
