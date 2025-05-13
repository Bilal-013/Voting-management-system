#ifndef CANDIDATE_H
#define CANDIDATE_H
#include <string>
class Candidate
{
    std::string name;
    std::string party;
    int votes;
    int candidateId;

public:
    Candidate();
    Candidate(const std::string &n, const std::string &p, int id = -1);
    void incrementVotes();
    int getVoteCount() const;
    void getCandidateInfo() const;
    const std::string &getName() const;
    const std::string &getParty() const { return party; }
    int getId() const { return candidateId; }
    void setId(int id) { candidateId = id; }
};

#endif