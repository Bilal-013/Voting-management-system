#ifndef ELECTION_H
#define ELECTION_H

#include "Candidate.h"
#include <string>
#include <ctime>

#define MAX_CANDIDATES 20

class Election
{
protected:
    std::string title;
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
    Election(const std::string &t);
    virtual void addCandidate(const std::string &name, const std::string &party);
    virtual bool isPartyAllowed(const std::string &party) = 0;
    virtual void startElection(int minutes, int seconds);
    virtual void endElection();
    virtual void displayCandidates();
    virtual void castVote(int candidateIndex);
    virtual void showResults();
    virtual void editCandidate(int index, const std::string &newName, const std::string &newParty);
    virtual void removeCandidate(int index);
    int getCandidateCount() const { return candidateCount; }
    Candidate *getCandidate(int index) { return (index >= 0 && index < candidateCount) ? &candidates[index] : nullptr; }
    Candidate *getCandidateById(int id);
    bool checkDuplicateCandidateId(int id);
    bool getStatus() const;
    const std::string &getTitle() const;
    bool isVotingOpen();
    virtual ~Election() {}
};

class LocalElection : public Election
{
    std::string city;

public:
    LocalElection(const std::string &t, const std::string &cityName);
    bool isPartyAllowed(const std::string &party) override;
    const std::string &getCity() const { return city; }
};

class NationalElection : public Election
{
    std::string country;

public:
    NationalElection(const std::string &t);
    bool isPartyAllowed(const std::string &party) override;
    const std::string &getCountry() const { return country; }
};

#endif