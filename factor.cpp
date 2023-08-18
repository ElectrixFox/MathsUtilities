#include "Factor.h"

extern std::vector<superint> primes;

std::string Factors::compress(int exponential)
{
std::string res = "";
superint len = x.size();

if(exponential == 1)
    {
    // gets all of the numbers in exponential form
    std::vector<Number> nums = exponentiate(x);
    len = nums.size();

    for (superint i = 0; i < len; i++)
        {
        std::string numstr = nums[i].getStr();

        // sets the suffix as blank if i+1 is the end of the array, if not then sets it to * 
        std::string end = ((i+1 == len) ? "" : " * ");

        // concat the strings
        res += (numstr + end);
        }
    }
else
    {
    for (superint i = 0; i < len; i++)
        {
        // turns the ith element superinto a string
        std::string body = std::to_string(x[i]);
        
        // same as before
        std::string end = ((i+1 == len) ? "" : " * ");

        // concating
        res += (body + end);
        }
    }

return res;
};

superint outFactors(Factors f)
{
std::vector<Number> ns = exponentiate(f.x);

std::cout << '\n';
for (superint i = 0; i < ns.size(); i++)
    {
    std::cout << ns[i].getStr();
    std::cout << (((i + 1) == ns.size()) ? "" : " * ");
    }

return 0;
}

facPair fermat(superint n)
{
superint a = 0;
superint b = 0;

while((a*a - n) < 0) a+= 1;

// these here to stop bad things happening (overloading)
superint c1 = floor(sqrt(a*a-n));
float c2 = sqrt(a*a-n);

while(floor(sqrt(a*a-n)) < sqrt(a*a-n))
    {
    a += 1;
    }

b = sqrt(a*a-n);

return {(a-b), (a+b)};
}

superint baseFermat(superint n, std::vector<superint>& f)
{ 

// getting any factors of two immediately out of there
while((n % 2) == 0)
    {
    n = n / 2;
    f.push_back(2);
    }

// linear search to test if prime
auto ptest = [](superint n)
    {
    
    if(n == 1)
        return -1;

    for(superint i = 0; i < primes.size()-1; i++)
        {
        if(primes[i] == n)
            return 1;
        }

    return 0;
    };


// the factors from the fermat factorisation
facPair facs = fermat(n);

// get the vals from the tuple
superint v1 = facs.a;
superint v2 = facs.b;


// testing if they're prime
superint res1 = ptest(v1);
superint res2 = ptest(v2);

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

Factors Factor(superint n)
{
Factors f;

// factor by trial division for small numbers
auto trialdiv = [](superint n, Factors f) {
    for(superint i = 0; i < primes.size(); i++)
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

superint readPrimes()
{
primes = loadPrimes("primes.txt");
std::cin.ignore();
std::cin.clear();

return 0;
}

std::vector<Number> exponentiate(std::vector<superint> nums)
{
// exponent count
superint expc = 1;
superint cur = 0;
superint prev = 0;

// sorts the numbers
nums = sort(nums);

// final nums
std::vector<Number> fins;

// removes the duplicates
std::vector<superint> tmp = nums;

tmp = remDuplicates(tmp);

for (superint i = 0; i < tmp.size(); i++)
    {
    // counts all ocurrances of tmp[i] in nums because tmp is unique it can be done as so
    expc = std::count(nums.begin(), nums.end(), tmp[i]);

    // a temp number to store in the vector
    Number tnum = {tmp[i], expc};

    // adds number tmp[i] with the exponent of expc
    fins.push_back(tnum);
    }


return fins;
}

std::vector<superint> remDuplicates(std::vector<superint> vec)
{
std::sort(vec.begin(), vec.end());
vec.erase(std::unique(vec.begin(), vec.end()), vec.end());

return vec;
}