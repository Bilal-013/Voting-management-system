#include "Candidate.h"
#include <iostream>
using namespace std;

Candidate::Candidate() : name(""), party(""), votes(0), candidateId(-1) {}
Candidate::Candidate(const std::string &n, const std::string &p, int id) : name(n), party(p), votes(0), candidateId(id) {}
void Candidate::incrementVotes() { votes++; }
int Candidate::getVoteCount() const { return votes; }
void Candidate::getCandidateInfo() const
{
    cout << "ID: " << candidateId << ", Name: " << name << ", Party: " << party << ", Votes: " << votes << endl;
}
const std::string &Candidate::getName() const { return name; }