#ifndef NATIONALELECTION_H
#define NATIONALELECTION_H
#include "Election.h"
using namespace std;
class NationalElection : public Election
{
    string country;

public:
    NationalElection(const string &t);
    bool isPartyAllowed(const string &party) override;
    const string &getCountry() const { return country; }
};
#endif