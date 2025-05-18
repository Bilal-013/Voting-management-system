#include "Election.h"
#include "NationalElection.h"
using namespace std;
NationalElection::NationalElection(const string &t) : Election(t), country("Pakistan") {}

bool NationalElection::isPartyAllowed(const string &party)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getParty() == party)
            return false;
    }
    return true;
}