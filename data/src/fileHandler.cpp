#include "fileHandler.h"
#include <fstream>
#include <iostream>
#include <sstream> // For istringstream
#include "LocalElection.h"
#include "NationalElection.h"
#include "Voter.h"
#include "Administrator.h"
using namespace std;
void fileHandler::saveCandidates(Candidate *candidates, int candidateCount, const string &filename)
{
    ofstream out(filename);
    for (int i = 0; i < candidateCount; ++i)
    {
        out << candidates[i].getId() << ' ' << candidates[i].getName() << ' ' << candidates[i].getParty() << ' ' << candidates[i].getVoteCount() << '\n';
    }
    out.close();
}

int fileHandler::loadCandidates(Candidate *&candidates, const string &filename)
{
    ifstream in(filename);
    string name, party;
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

void fileHandler::saveElections(Election **elections, int electionCount, const string &filename)
{
    ofstream out(filename);
    for (int i = 0; i < electionCount; ++i)
    {
        string type = "local";
        string cityOrCountry = "-";
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

int fileHandler::loadElections(Election **&elections, const string &filename)
{
    ifstream in(filename);
    string type, title, cityOrCountry;
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
            string cname, cparty;
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

void fileHandler::saveUsers(User **users, int userCount, const string &filename)
{
    ofstream out(filename);
    for (int i = 0; i < userCount; ++i)
    {
        // Check if it's a voter
        Voter* voter = dynamic_cast<Voter*>(users[i]);
        if (voter)
        {
            // Save voter with all fields
            out << voter->getId() << ' ' 
                << voter->getUsername() << ' ' 
                << voter->getPassword() << ' '
                << (voter->getName().empty() ? "Unknown" : voter->getName()) << ' ' 
                << voter->getAge() << ' ' 
                << (voter->getRegion().empty() ? "Unknown" : voter->getRegion()) << '\n';
        }
        else
        {
            // Save admin or other user types with minimal fields
            out << users[i]->getId() << ' ' 
                << users[i]->getUsername() << ' ' 
                << users[i]->getPassword() << ' '
                << "Admin 0 System" << '\n';
        }
    }
    out.close();
}

int fileHandler::loadUsers(User **&users, const string &filename)
{
    ifstream in(filename);
    string line, username, password, name, region;
    int id, age;
    int count = 0;
    int capacity = 10;
    users = new User *[capacity];
    
    // Read the file line by line for more robust handling
    while (getline(in, line))
    {
        // Skip empty lines or comment lines
        if (line.empty() || line[0] == '/' || line[0] == '#')
            continue;
            
        istringstream iss(line);
        
        // Try to read all fields
        if (!(iss >> id >> username >> password))
            continue;  // Skip if basic fields can't be read
        
        // Handle optional fields
        if (!(iss >> name)) name = "Unknown";
        if (!(iss >> age)) age = 0;
        if (!(iss >> region)) region = "Unknown";
        
        // Resize array if needed
        if (count >= capacity)
        {
            capacity *= 2;
            User **temp = new User *[capacity];
            for (int i = 0; i < count; ++i)
                temp[i] = users[i];
            delete[] users;
            users = temp;
        }
        
        // Special handling for admin user
        if (username == "admin")
            users[count++] = new Administrator(username, password);
        else
            users[count++] = new Voter(username, password, id, name, age, region);
    }
    in.close();
    return count;
}
