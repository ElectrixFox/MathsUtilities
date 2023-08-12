#include "Factor.h"

using std::cout;
using std::cin;
using std::get;


extern std::vector<int> primes;

std::string Factors::compress()
{
std::string res = "";
int len = x.size();

for (int i = 0; i < len; i++)
    {
    res += (std::to_string(x[i]) + ((i+1 != len) ? " * " : ""));
    }

return res;
};


#define vout(x, s) cout << "\n" << #x << ": "; for(int K0 = 0; K0 < x.size(); K0++) { std::cout << x[K0] << ((K0+1 == x.size()) ? "" : s); }

int outFactors(Factors f)
{
std::vector<Number> ns = exponentiate(f.x);

cout << '\n';
for (int i = 0; i < ns.size(); i++)
    {
    std::cout << ns[i].getStr();
    std::cout << (((i + 1) == ns.size()) ? "" : " * ");
    }

return 0;
}

facPair fermat(int n)
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

return {(a-b), (a+b)};
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
        return -1;

    for(int i = 0; i < primes.size()-1; i++)
        {
        if(primes[i] == n)
            return 1;
        }

    return 0;
    };


// the factors from the fermat factorisation
facPair facs = fermat(n);

// get the vals from the tuple
int v1 = facs.a;
int v2 = facs.b;


// testing if they're prime
int res1 = ptest(v1);
int res2 = ptest(v2);

// if it is a prime factor add it to the list if not factorise that (for both factors)
if(res1 == 1)
    f.push_back(v1);
else if (res1 == 0)
    baseFermat(v1, f);

if(res2 == 1)
    f.push_back(v2);
else if (res2 == 0)
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

f.x = sort(f.x);

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