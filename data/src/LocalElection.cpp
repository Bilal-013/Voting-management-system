#include "Election.h"
#include "LocalElection.h"
using namespace std;
LocalElection::LocalElection(const string &t, const string &cityName) : Election(t), city(cityName) {}

bool LocalElection::isPartyAllowed(const string &party)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getParty() == party)
            return false;
    }
    return true;
}