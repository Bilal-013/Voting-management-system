#include "Election.h"
#include <iostream>
using namespace std;

Election::Election() : title(""), candidateCount(0), isActive(false), durationMinutes(0), durationSeconds(0), startTime(0), endTime(0), nextCandidateId(1) {}
Election::Election(const std::string &t) : title(t), candidateCount(0), isActive(false), durationMinutes(0), durationSeconds(0), startTime(0), endTime(0), nextCandidateId(1) {}
void Election::addCandidate(const std::string &name, const std::string &party)
{
    if (candidateCount < MAX_CANDIDATES)
    {
        candidates[candidateCount++] = Candidate(name, party, nextCandidateId++);
    }
}
void Election::startElection(int minutes, int seconds)
{
    durationMinutes = minutes;
    durationSeconds = seconds;
    startTime = time(nullptr);
    endTime = startTime + minutes * 60 + seconds;
    isActive = true;
    cout << "Election started. It will end in " << minutes << " minutes and " << seconds << " seconds.\n";
}
void Election::endElection()
{
    isActive = false;
    cout << "Election ended.\n";
}
void Election::displayCandidates()
{
    for (int i = 0; i < candidateCount; ++i)
    {
        cout << i + 1 << ". ";
        candidates[i].getCandidateInfo();
    }
}
void Election::castVote(int candidateIndex)
{
    if (isVotingOpen() && candidateIndex >= 0 && candidateIndex < candidateCount)
    {
        candidates[candidateIndex].incrementVotes();
    }
}
void Election::editCandidate(int index, const std::string &newName, const std::string &newParty)
{
    if (index >= 0 && index < candidateCount)
    {
        int id = candidates[index].getId();
        candidates[index] = Candidate(newName, newParty, id);
    }
}
void Election::removeCandidate(int index)
{
    if (index >= 0 && index < candidateCount)
    {
        for (int i = index; i < candidateCount - 1; ++i)
        {
            candidates[i] = candidates[i + 1];
        }
        --candidateCount;
    }
}
void Election::showResults()
{
    cout << "Results for " << title << ":\n";
    for (int i = 0; i < candidateCount; ++i)
    {
        candidates[i].getCandidateInfo();
    }
}
bool Election::getStatus() const { return isActive; }
const std::string &Election::getTitle() const { return title; }
bool Election::isVotingOpen()
{
    if (!isActive)
        return false;
    time_t now = time(nullptr);
    if (now >= endTime)
    {
        isActive = false;
        cout << "Election duration is over. Voting is closed.\n";
        return false;
    }
    return true;
}
Candidate *Election::getCandidateById(int id)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getId() == id)
            return &candidates[i];
    }
    return nullptr;
}
bool Election::checkDuplicateCandidateId(int id)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getId() == id)
            return true;
    }
    return false;
}