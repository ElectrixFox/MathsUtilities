#include "Utility.h"
#include "Factor.h"
#include "ConsecutiveComposites.h"
#include "MainWindow.h"

std::vector<superint> primes;

void primeChecker()
{
superint n;

while(n != 0)
    {
    std::cin >> n;

    std::cout << n << ((isPrime(n) == 1) ? " is prime\n" : " not prime\n");
    }
}

int main(int argc, char const *argv[])
{
primes = loadPrimes("primes.txt");

CpMain();


/* primes = loadPrimes("primes.txt");

for (int i = 5; i < 25; i++)
    {
    std::vector<Factors> facs = genComposites(primes[i]+1, primes[i+1]-1);
    writeCompositesToFile(facs, 1, 1);
    } */


return 0;
}
