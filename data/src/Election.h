#ifndef ELECTION_H
#define ELECTION_H

#include "Candidate.h"
#include <string>
#include <ctime>

#define MAX_CANDIDATES 20
using namespace std;
class Election
{
protected:
    string title;
    Candidate candidates[MAX_CANDIDATES];
    int candidateCount;
    bool isActive;
    int durationMinutes;
    int durationSeconds;
    time_t startTime;
    int nextCandidateId;

public:
    time_t endTime;
    Election();
    Election(const string &t);
    virtual void addCandidate(const string &name, const string &party);
    virtual bool isPartyAllowed(const string &party) = 0;
    virtual void startElection(int minutes, int seconds);
    virtual void endElection();
    virtual void displayCandidates();
    virtual void castVote(int candidateIndex);
    virtual void showResults();
    virtual void editCandidate(int index, const string &newName, const string &newParty);
    virtual void removeCandidate(int index);
    int getCandidateCount() const { return candidateCount; }
    Candidate *getCandidate(int index) { return (index >= 0 && index < candidateCount) ? &candidates[index] : nullptr; }
    Candidate *getCandidateById(int id);
    bool checkDuplicateCandidateId(int id);
    bool getStatus() const;
    const string &getTitle() const;
    bool isVotingOpen();
    virtual ~Election() {}
};


#endif