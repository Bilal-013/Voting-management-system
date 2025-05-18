#ifndef VOTER_H
#define VOTER_H
using namespace std;
#include "User.h"
#include "Election.h"

#define MAX_ELECTIONS 10

class Voter : public User
{
    bool hasVoted[MAX_ELECTIONS];

public:
    Voter();
    Voter(const string &uname, const string &pwd, int id = -1);
    Voter(const string &uname, const string &pwd, int id, const string &name, int age, const string &region);
    void viewElections(Election *elections[], int count);
    void castVote(Election *election, int electionIndex, int candidateIndex);
    void checkVoteStatus(int electionIndex);
};

#endif