#ifndef VOTER_H
#define VOTER_H

#include "User.h"
#include "Election.h"

#define MAX_ELECTIONS 10

class Voter : public User
{
    bool hasVoted[MAX_ELECTIONS];

public:
    Voter();
    Voter(const std::string &uname, const std::string &pwd, int id = -1);
    Voter(const std::string &uname, const std::string &pwd, int id, const std::string &name, int age, const std::string &region);
    void viewElections(Election *elections[], int count);
    void castVote(Election *election, int electionIndex, int candidateIndex);
    void checkVoteStatus(int electionIndex);
};

#endif