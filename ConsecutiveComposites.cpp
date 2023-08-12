#include "ConsecutiveComposites.h"

extern std::vector<int> primes;

int writeCompositesToFile(std::vector<Factors> compfacs, int adding)
{
int num;
std::string facs;
const int fsize = compfacs.size() - 1;

std::tuple<int, std::string> element[fsize] = {};

for (int i = 0; i < fsize; i++)
    {
    num = compfacs[i].compute();
    facs = compfacs[i].compress();

    std::get<0>(element[i]) = num;
    std::get<1>(element[i]) = facs;
    }

std::ios_base::openmode mode = std::ios::binary;

if(adding == 1)
    mode = std::ios::app;

std::ofstream out = std::ofstream("Composite.txt", mode);

std::string line;

for (int i = 0; i < fsize; i++)
    {
    line = std::to_string(std::get<0>(element[i])) + ": " + std::get<1>(element[i]);
    out.write(line.c_str(), line.length());
    out.write("\n", 1);
    }



return 1;   
}

std::vector<Factors> genComposites(int start, int end)
{
std::vector<Factors> comps;

if(isPrime(start) == 1) { std::cout << "\nStarting on a prime isn't valid"; return {}; }

// gets the new end
end = findClosestPrime(primes, start, 1);

for (int i = start; i < end+1; i++)
    {
    comps.push_back(Factor(i));
    }

return comps;
}

