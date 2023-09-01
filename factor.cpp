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

std::vector<superint> Factors::uniqueI()
{
// sorted factors
std::vector<superint> sFacs;

// the unique factors
std::vector<superint> uniFactors;

// sort the factors
sFacs = sort(x);

// add the first factor to the list
uniFactors.push_back(sFacs[0]);

// add all of the unique ones to the unique list
for(int i = 1; i < sFacs.size(); i++)
    {
    if(sFacs[i-1] == sFacs[i])
        continue;
    else
        uniFactors.push_back(sFacs[i]);
    }

return uniFactors;
}

std::string Factors::unique(std::string separator)
{
// getting all of the unique integer factors
std::vector<superint> uniFactors = uniqueI();

std::string endProd = "";

// construct the final string
for (superint i : uniFactors)
    {
    // if i is the last number don't add any separator
    if(uniFactors[uniFactors.size() - 1] == i) separator = "";

    // add the number and the separator to the final string
    endProd += std::to_string(i) + separator;
    }

return endProd;
}

std::vector<Number> Factors::getAsNumbers()
{
// getting all of the factors in exponential form
std::vector<Number> nums = exponentiate(x);

return nums;
}

Factors getAsFactors(std::vector<Number> numbers)
{
Factors f;

// for each number in numbers
for(Number n : numbers)
    {
    // add the base exponent amount of times
    for (int i = 0; i < n.exponent; i++)
        f.x.push_back(n.base);   
    }

return f;
}

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

Factors BruteFactor(superint n)
{
Factors f;

// trial division
for(superint i = 0; i < primes.size(); i++)
    {
    // while n divides by the prime
    while(n % primes[i] == 0)
        {
        // add the prime to the list of factors
        f.x.push_back(primes[i]);

        // divide n by the prime
        n /= primes[i];
        }
    }

return f;
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