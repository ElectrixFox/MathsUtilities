#include "Utility.h"
#include "Factor.h"
#include "ConsecutiveComposites.h"

std::vector<int> primes;

int main(int argc, char const *argv[])
{

primes = loadPrimes("primes.txt");

//Factors fs = Factor(55);

//outFactors(fs);

std::vector<Factors> facs = genComposites(32, 36);
writeCompositesToFile(facs);

std::cout << "\nEnd";

return 0;
}
