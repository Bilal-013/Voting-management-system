#include "Election.h"
#include "LocalElection.h"

LocalElection::LocalElection(const std::string &t, const std::string &cityName) : Election(t), city(cityName) {}

bool LocalElection::isPartyAllowed(const std::string &party)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getParty() == party)
            return false;
    }
    return true;
}