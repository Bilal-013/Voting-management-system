#include "Voter.h"
#include <iostream>
using namespace std;

Voter::Voter() : User()
{
    for (int i = 0; i < MAX_ELECTIONS; ++i)
        hasVoted[i] = false;
}
Voter::Voter(const std::string &uname, const std::string &pwd, int id) : User(uname, pwd, id)
{
    for (int i = 0; i < MAX_ELECTIONS; ++i)
        hasVoted[i] = false;
}
Voter::Voter(const std::string &uname, const std::string &pwd, int id, const std::string &name, int age, const std::string &region)
    : User(uname, pwd, id)
{
    setName(name);
    setAge(age);
    setRegion(region);
    for (int i = 0; i < MAX_ELECTIONS; ++i)
        hasVoted[i] = false;
}
void Voter::viewElections(Election *elections[], int count)
{
    cout << "Available Elections:\n";
    for (int i = 0; i < count; ++i)
    {
        if (elections[i]->getStatus())
            cout << i + 1 << ". " << elections[i]->getTitle() << endl;
    }
}
void Voter::castVote(Election *election, int electionIndex, int candidateIndex)
{
    if (!hasVoted[electionIndex])
    {
        if (election->isVotingOpen())
        {
            election->castVote(candidateIndex);
            hasVoted[electionIndex] = true;
            cout << "Vote cast successfully.\n";
        }
        else
        {
            cout << "Election is not active or voting period has ended.\n";
        }
    }
    else
    {
        cout << "You have already voted.\n";
    }
}
void Voter::checkVoteStatus(int electionIndex)
{
    cout << (hasVoted[electionIndex] ? "Voted" : "Not Voted") << endl;
}