#include "Utility.h"
#include "Checkprime.h"

using std::cout;
using std::cin;
using std::get;

std::vector<int> primes = {2, 3, 5, 7, 11, 13, 17, 19, 23};

#define vout(x, s) cout << "\n" << #x << ": "; for(int K0 = 0; K0 < x.size(); K0++) { std::cout << x[K0] << ((K0+1 == x.size()) ? "" : s); }

struct Factors
    {
    std::vector<int> x;

    void flush() { using namespace std; x.erase(remove(x.begin(), x.end(), 1), x.end()); };

    };

struct Number
    {
    int base;
    int exponent;

    std::string getStr() {  return {std::to_string(base) + "^" + std::to_string(exponent)}; }
    };

int readPrimes();

std::tuple<int, int> fermat(int n);

int baseFermat(int n, std::vector<int>& f);
Factors Factor(int n);

std::vector<Number> exponentiate(std::vector<int> nums);
std::vector<int> remDuplicates(std::vector<int> vec);


int main(int argc, char const *argv[])
{
int n = 0;
cin >> n;

loadPrimes("primes.txt");

Factors f = Factor(n);

f.flush();
f.x = sort(f.x);


std::vector<Number> ns = exponentiate(f.x);


cout << '\n';
for (int i = 0; i < ns.size(); i++)
    {
    std::cout << ns[i].getStr();
    std::cout << (((i + 1) == ns.size()) ? "" : " * ");
    }

cin.get();

return 0;
}

std::tuple<int, int> fermat(int n)
{
int a = 0;
int b = 0;

while((a*a - n) < 0) a+= 1;

// these here to stop bad things happening (overloading)
int c1 = floor(sqrt(a*a-n));
float c2 = sqrt(a*a-n);

while(floor(sqrt(a*a-n)) < sqrt(a*a-n))
    {
    a += 1;
    }

b = sqrt(a*a-n);

return std::tuple<int, int>((a-b), (a+b));
}

int baseFermat(int n, std::vector<int>& f)
{  

// getting any factors of two immediately out of there
while((n % 2) == 0)
    {
    n = n / 2;
    f.push_back(2);
    }

// linear search to test if prime
auto ptest = [](int n)
    {
    
    if(n == 1)
        return 1;

    for(int i = 0; i < sizeof(primes) / sizeof(primes[0]); i++)
        {
        if(primes[i] == n)
            return 1;
        }

    return 0;
    };


// the factors from the fermat factorisation
std::tuple<int, int> facs = fermat(n);

// get the vals from the tuple
int v1 = get<0>(facs);
int v2 = get<1>(facs);


// testing if they're prime
int res1 = ptest(v1);
int res2 = ptest(v2);


// if it is a prime factor add it to the list if not factorise that (for both factors)
if(res1 == 1)
    f.push_back(v1);
else
    baseFermat(v1, f);

if(res2 == 1)
    f.push_back(v2);
else
    baseFermat(v2, f);



return 1;
}

Factors Factor(int n)
{
Factors f;

// factor by trial division for small numbers
auto trialdiv = [](int n, Factors f) {
    for(int i = 0; i < primes.size(); i++)
        {
        while(n % primes[i] == 0)
            {
            f.x.push_back(primes[i]);
            n /= primes[i];
            }
        }
    };


baseFermat(n, f.x);

return f;
}

int readPrimes()
{
primes = loadPrimes("primes.txt");
cin.ignore();
cin.clear();

return 0;
}

std::vector<Number> exponentiate(std::vector<int> nums)
{
// exponent count
int expc = 1;
int cur = 0;
int prev = 0;

nums = sort(nums);

// final nums
std::vector<Number> fins;

std::vector<int> tmp = nums;

tmp = remDuplicates(tmp);

for (size_t i = 0; i < tmp.size(); i++)
    {
    expc = std::count(nums.begin(), nums.end(), tmp[i]);
    fins.push_back({tmp[i], expc});
    }


return fins;
}

std::vector<int> remDuplicates(std::vector<int> vec)
{
std::sort(vec.begin(), vec.end());
vec.erase(std::unique(vec.begin(), vec.end()), vec.end());

return vec;
}