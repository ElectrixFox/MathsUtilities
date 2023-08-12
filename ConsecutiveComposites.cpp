#include "ConsecutiveComposites.h"

extern std::vector<int> primes;

using std::tuple;

int writeCompositesToFile(std::vector<Factors> compfacs, int adding, int exponential)
{
int num;
std::string facs;
int fsize = (compfacs.size()-1);

std::vector<tuple<int, std::string>> element;

for (int i = 0; i < fsize; i++)
    {
    // gets the result of the factorisation (factorisation of 2*2 would return 4)
    num = compfacs[i].compute();

    // puts all of the factorisation together and adds the multiplication signs
    facs = compfacs[i].compress(exponential);

    // adds the number and factors to the tuple vector
    element.push_back({num, facs});
    }


// overwrite everything
std::ios_base::openmode mode = std::ios::binary;

// appending
if(adding == 1)
    mode = std::ios::app;

// opens the file with the chosen mode
std::ofstream out = std::ofstream("Composite.txt", mode);

std::string line;

for (int i = 0; i < fsize; i++)
    {
    // gets the base of the number and converts to string
    std::string base = std::to_string(std::get<0>(element[i]));

    // gets the factors of the number
    std::string factors = std::get<1>(element[i]);

    // concats them
    line = (base + ": " + factors);

    // writes the line to the file
    out.write(line.c_str(), line.length());
    out.write("\n", 1);
    }

out.close();


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

