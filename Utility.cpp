#include "Utility.h"

using namespace std;

vector<int> loadPrimes(string filePath)
{
vector<int> primes;

// if file path empty give the base primes
if(filePath == "")
    {
    vector<int> p = {2, 3, 5, 7};
    return p;
    }

// reading back the file
ifstream in(filePath, ios::in);

string line;
while(getline(in, line))
    {
    // if the line isn't a new line it adds it to the prime list
    if(line != "\n")
        primes.push_back(atoi(line.c_str()));
    }

in.close();

return primes;
}

std::vector<int> sort(std::vector<int> arr)
{
int swaps = 0;

do
{
swaps = 0;

for (int i = 0; i < arr.size()-1; i++)
    {
    if(arr[i] > arr[i+1])
        {
        std::swap(arr[i], arr[i+1]);
        swaps += 1;
        }
    }

} while (swaps != 0);

return arr;
}