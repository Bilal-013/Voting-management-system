#ifndef LOCALELECTION_H
#define LOCALELECTION_H
#include "Election.h"
using namespace std;
class LocalElection : public Election
{
    string city;

public:
    LocalElection(const string &t, const string &cityName);
    bool isPartyAllowed(const string &party) override;
    const string &getCity() const { return city; }
};

#endif