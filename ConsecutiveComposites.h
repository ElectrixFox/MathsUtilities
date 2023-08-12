#ifndef CONSECUTIVECOMPOSITES_H
#define CONSECUTIVECOMPOSITES_H

#include "Utility.h"
#include "Factor.h"
#include "Checkprime.h"
#include "FindPrimes.h"

int writeCompositesToFile(std::vector<Factors> compfacs, int adding = 0);
std::vector<Factors> genComposites(int start, int end);

#endif