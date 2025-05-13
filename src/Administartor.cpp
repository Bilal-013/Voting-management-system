#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
#define MAX_ELECTIONS 10
#include <cstring>
#include <iostream>

Administrator::Administrator(const std::string &uname, const std::string &pwd) : User(uname, pwd) {}

void Administrator::createElection(Election *elections[], int &count, const std::string &type, const std::string &title)
{
    if (count < MAX_ELECTIONS)
    {
        if (type == "local")
        {
            std::string city;
            std::cout << "Enter city name: ";
            std::cin >> city;
            elections[count++] = new LocalElection(title, city);
        }
        else if (type == "national")
        {
            elections[count++] = new NationalElection(title);
        }
    }
}
void Administrator::addCandidate(Election *election, const std::string &name, const std::string &party)
{
    if (!election->isPartyAllowed(party))
    {
        std::cout << "Party already exists in this election!\n";
        return;
    }
    election->addCandidate(name, party);
}
void Administrator::editCandidate(Election *election, int index, const std::string &newName, const std::string &newParty)
{
    election->editCandidate(index, newName, newParty);
}
void Administrator::removeCandidate(Election *election, int index)
{
    election->removeCandidate(index);
}
void Administrator::startElection(Election *election, Election **elections, int electionCount)
{
    // shows active national election
    if (dynamic_cast<NationalElection *>(election))
    {
        for (int i = 0; i < electionCount; ++i)
        {
            if (dynamic_cast<NationalElection *>(elections[i]) && elections[i]->getStatus())
            {
                std::cout << "Another national election is already active!\n";
                return;
            }
        }
    }
    int minutes, seconds;
    std::cout << "Enter election duration (minutes): ";
    std::cin >> minutes;
    std::cout << "Enter election duration (seconds): ";
    std::cin >> seconds;
    election->startElection(minutes, seconds);
}
void Administrator::endElection(Election *election) { election->endElection(); }
void Administrator::viewResults(Election *election) { election->showResults(); }