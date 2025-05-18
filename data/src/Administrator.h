#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"
#include "Election.h"
#include "Voter.h" // Include Voter class declaration
#include <string>
using namespace std;
class Administrator : public User
{
public:
    Administrator(const string &uname, const string &pwd);
    void createElection(Election *elections[], int &count, const string &type, const string &title);
    void addCandidate(Election *election, const string &name, const string &party);
    void editCandidate(Election *election, int index, const string &newName, const string &newParty);
    void removeCandidate(Election *election, int index);
    void startElection(Election *election, Election **elections, int electionCount);
    void endElection(Election *election);
    void viewResults(Election *election);
    void viewAllVoters(Voter **voters, int voterCount);
    void viewAllCandidates(Election **elections, int electionCount);
};

#endif