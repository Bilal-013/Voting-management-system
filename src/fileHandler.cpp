#include "fileHandler.h"
#include <fstream>
#include <iostream>
#include "LocalElection.h"
#include "NationalElection.h"
#include "Voter.h"
#include "Administrator.h"

void fileHandler::saveCandidates(Candidate *candidates, int candidateCount, const std::string &filename)
{
    std::ofstream out(filename);
    for (int i = 0; i < candidateCount; ++i)
    {
        out << candidates[i].getId() << ' ' << candidates[i].getName() << ' ' << candidates[i].getParty() << ' ' << candidates[i].getVoteCount() << '\n';
    }
    out.close();
}

int fileHandler::loadCandidates(Candidate *&candidates, const std::string &filename)
{
    std::ifstream in(filename);
    std::string name, party;
    int votes, id;
    int count = 0;
    int capacity = 10;
    candidates = new Candidate[capacity];
    while (in >> id >> name >> party >> votes)
    {
        if (count >= capacity)
        {
            capacity *= 2;
            Candidate *temp = new Candidate[capacity];
            for (int i = 0; i < count; ++i)
                temp[i] = candidates[i];
            delete[] candidates;
            candidates = temp;
        }
        candidates[count] = Candidate(name, party, id);
        for (int i = 0; i < votes; ++i)
            candidates[count].incrementVotes();
        ++count;
    }
    in.close();
    return count;
}

void fileHandler::saveElections(Election **elections, int electionCount, const std::string &filename)
{
    std::ofstream out(filename);
    for (int i = 0; i < electionCount; ++i)
    {
        std::string type = "local";
        std::string cityOrCountry = "-";
        if (dynamic_cast<LocalElection *>(elections[i]))
        {
            type = "local";
            cityOrCountry = static_cast<LocalElection *>(elections[i])->getCity();
        }
        else if (dynamic_cast<NationalElection *>(elections[i]))
        {
            type = "national";
            cityOrCountry = static_cast<NationalElection *>(elections[i])->getCountry();
        }
        out << type << ' ' << elections[i]->getTitle() << ' ' << elections[i]->getStatus() << ' ' << elections[i]->endTime << ' ' << elections[i]->getCandidateCount() << ' ' << cityOrCountry << '\n';
        for (int j = 0; j < elections[i]->getCandidateCount(); ++j)
        {
            Candidate *c = elections[i]->getCandidate(j);
            out << c->getId() << ' ' << c->getName() << ' ' << c->getParty() << ' ' << c->getVoteCount() << '\n';
        }
    }
    out.close();
}

int fileHandler::loadElections(Election **&elections, const std::string &filename)
{
    std::ifstream in(filename);
    std::string type, title, cityOrCountry;
    bool status;
    time_t endTime;
    int count = 0;
    int capacity = 10;
    elections = new Election *[capacity];
    while (in >> type >> title >> status >> endTime)
    {
        int candidateCount;
        in >> candidateCount >> cityOrCountry;
        if (count >= capacity)
        {
            capacity *= 2;
            Election **temp = new Election *[capacity];
            for (int i = 0; i < count; ++i)
                temp[i] = elections[i];
            delete[] elections;
            elections = temp;
        }
        Election *e = nullptr;
        if (type == "local")
        {
            e = new LocalElection(title, cityOrCountry);
        }
        else if (type == "national")
        {
            e = new NationalElection(title);
        }
        if (status)
            e->startElection(0, 0);
        e->endTime = endTime;
        for (int j = 0; j < candidateCount; ++j)
        {
            int cid, votes;
            std::string cname, cparty;
            in >> cid >> cname >> cparty >> votes;
            e->addCandidate(cname, cparty);
            Candidate *c = e->getCandidate(j);
            c->setId(cid);
            for (int v = 0; v < votes; ++v)
                c->incrementVotes();
        }
        elections[count++] = e;
    }
    in.close();
    return count;
}

void fileHandler::saveUsers(User **users, int userCount, const std::string &filename)
{
    std::ofstream out(filename);
    for (int i = 0; i < userCount; ++i)
    {
        out << users[i]->getId() << ' ' << users[i]->getUsername() << ' ' << users[i]->getPassword()
            << ' ' << users[i]->getName() << ' ' << users[i]->getAge() << ' ' << users[i]->getRegion() << '\n';
    }
    out.close();
}

int fileHandler::loadUsers(User **&users, const std::string &filename)
{
    std::ifstream in(filename);
    std::string username, password, name, region;
    int id, age;
    int count = 0;
    int capacity = 10;
    users = new User *[capacity];
    while (in >> id >> username >> password >> name >> age >> region)
    {
        if (count >= capacity)
        {
            capacity *= 2;
            User **temp = new User *[capacity];
            for (int i = 0; i < count; ++i)
                temp[i] = users[i];
            delete[] users;
            users = temp;
        }
        users[count++] = new Voter(username, password, id, name, age, region);
    }
    in.close();
    return count;
}
