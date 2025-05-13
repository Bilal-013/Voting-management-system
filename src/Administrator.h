#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "User.h"
#include "Election.h"
#include <string>

class Administrator : public User
{
public:
    Administrator(const std::string &uname, const std::string &pwd);
    void createElection(Election *elections[], int &count, const std::string &type, const std::string &title);
    void addCandidate(Election *election, const std::string &name, const std::string &party);
    void editCandidate(Election *election, int index, const std::string &newName, const std::string &newParty);
    void removeCandidate(Election *election, int index);
    void startElection(Election *election, Election **elections, int electionCount);
    void endElection(Election *election);
    void viewResults(Election *election);
};

#endif