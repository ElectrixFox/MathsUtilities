#include "Composite.h"

Detail DetailContainer::getTop()
{
std::vector<Number> nums;

std::vector<superint> facs = factors[top];
std::vector<superint> exps = exponents[top];

for(int i = 0; i < facs.size(); i++) 
    {
    nums.push_back({facs[i], exps[i]});
    }

return {numbers[top], nums};
}

Detail DetailContainer::getAt(int n)
{
std::vector<Number> nums;

std::vector<superint> facs = factors[n];
std::vector<superint> exps = exponents[n];

for(int i = 0; i < facs.size(); i++) 
    {
    nums.push_back({facs[i], exps[i]});
    }

return {numbers[n], nums};
}

int DetailContainer::hasBeen(int value)
{
// if the value is in a previous layer it's been
for (int i = 0; i < top; i++)
    {
    for(Number n : getAt(i).factors)
        {
        // if it's found it's been
        if(n.base == value)
            {
            return 1;
            }
        }
    }

return 0;
}

void DetailContainer::loadData(const std::string& filePath)
{
numbers.clear();
factors.clear();
exponents.clear();

// regex pattern
std::regex pattern(R"((\d+):\s*((?:\d+\^\d+\s*\*\s*)*\d+\^\d+)$)");

// opens the file to read
std::ifstream file(filePath);

// all of the lines will be stored here
std::vector<std::string> lines;

// current line
std::string line;

// reads back all of the lines
while (std::getline(file, line))
    {
    // adding the line to the stream
    lines.push_back(line);
    }

// closes file
file.close();

for(std::string s : lines)
    {
    // arrays
    std::vector<superint> facs;
    std::vector<superint> exps;

    // gets all of the matches of the pattern
    std::smatch match;

    if (std::regex_match(s, match, pattern))
        {
        // group 1 matches are the numbers
        int number = std::stoi(match[1]);

        // splits at the multiplication sign
        std::string factorString = match[2];

        int startPos = 0;
        int foundPos = 0;

        // there are x + 1 numbers
        int numcount = (int)std::count(factorString.begin(), factorString.end(), '*') + 1;

        int a = 0;
        int b = 0;

        int c = 0;

        for(int i = 0; i < numcount; i++)
            {
            // finding the position of the next *
            foundPos = (factorString.find("*", 0) != std::string::npos) ? factorString.find("*", 0) : -1;

            // substring the string
            factorString = factorString.substr(startPos, factorString.length()-startPos);  

            if(foundPos == -1)
                {   
                // get back the base and the exponent
                sscanf(factorString.c_str(), "%d^%d", &a, &b);
                
                // push them to their respective queues
                facs.push_back(a);
                exps.push_back(b);
                }
            else
                {
                // get back the base and the exponent
                sscanf(factorString.c_str(), "%d^%d", &a, &b);
                
                // push them to their respective queues
                facs.push_back(a);
                exps.push_back(b);
                }

            // the new start pos will be the old found pos + 2
            startPos = foundPos + 2;
            }
        
        // adds all of them to the respective global variables
        numbers.push_back(number);
        factors.push_back(facs);
        exponents.push_back(exps);
        }
    }


}

void DetailContainer::pushTop(Detail detail)
{
/* // insert the number at the top of the vector
numbers.insert(numbers.begin() + top, detail.number);

std::vector<superint> facs;
std::vector<superint> exps;

// add all of the factors and respective exponents to vectors
for(Number n : detail.factors)
    {
    facs.push_back(n.base);
    exps.push_back(n.exponent);
    }

// add the factors and exponents to the factors and exponents
factors.insert(factors.begin() + top, facs);
exponents.insert(factors.begin() + top, exps); */

// insert the number at the top of the vector
numbers.push_back(detail.number);

std::vector<superint> facs;
std::vector<superint> exps;

// add all of the factors and respective exponents to vectors
for(Number n : detail.factors)
    {
    facs.push_back(n.base);
    exps.push_back(n.exponent);
    }

// add the factors and exponents to the factors and exponents
factors.push_back(facs);
exponents.push_back(exps);
}