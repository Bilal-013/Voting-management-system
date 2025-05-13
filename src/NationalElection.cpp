#include "Election.h"
#include "NationalElection.h"

NationalElection::NationalElection(const std::string &t) : Election(t), country("Pakistan") {}

bool NationalElection::isPartyAllowed(const std::string &party)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getParty() == party)
            return false;
    }
    return true;
}