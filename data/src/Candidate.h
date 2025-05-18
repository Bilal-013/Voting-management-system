#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
using namespace std;
class Candidate
{
    string name;
    string party;
    int votes;
    int candidateId;

public:
    Candidate();
    Candidate(const string &n, const string &p, int id = -1);
    void incrementVotes();
    int getVoteCount() const;
    void getCandidateInfo() const;
    const string &getName() const;
    const string &getParty() const { return party; }
    int getId() const { return candidateId; }
    void setId(int id) { candidateId = id; }
};

#endif