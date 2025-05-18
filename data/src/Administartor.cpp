#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
#define MAX_ELECTIONS 10
#include <cstring>
#include <iostream>
#include <iomanip> // For setw
#include <windows.h> // For console information
#include "ColorConsole.h" // For colored output
using namespace std;
Administrator::Administrator(const string &uname, const string &pwd) : User(uname, pwd) {}

void Administrator::createElection(Election *elections[], int &count, const string &type, const string &title)
{
    if (count < MAX_ELECTIONS)
    {
        if (type == "local")
        {
            string city;
            cout << "Enter city name: ";
            cin >> city;
            elections[count++] = new LocalElection(title, city);
        }
        else if (type == "national")
        {
            elections[count++] = new NationalElection(title);
        }
    }
}
void Administrator::addCandidate(Election *election, const string &name, const string &party)
{
    if (!election->isPartyAllowed(party))
    {
        cout << "Party already exists in this election!\n";
        return;
    }
    election->addCandidate(name, party);
}
void Administrator::editCandidate(Election *election, int index, const string &newName, const string &newParty)
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
                cout << "Another national election is already active!\n";
                return;
            }
        }
    }
    int minutes, seconds;
    cout << "Enter election duration (minutes): ";
    cin >> minutes;
    cout << "Enter election duration (seconds): ";
    cin >> seconds;
    election->startElection(minutes, seconds);
}
void Administrator::endElection(Election *election) { election->endElection(); }
void Administrator::viewResults(Election *election) { election->showResults(); }

void Administrator::viewAllVoters(Voter **voters, int voterCount) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    string title = "ALL REGISTERED VOTERS";
    int padding = (consoleWidth - title.length()) / 2;
    cout << string(consoleWidth, '-') << endl;
    cout << string(padding, ' ') << title << endl;
    cout << string(consoleWidth, '-') << endl;
    cout << endl;
    
    if (voterCount == 0) {
        cout << string(padding, ' ') << "No voters registered in the system." << endl;
        return;
    }
    
    // Table width is fixed for readability
    const string tableHeader = "| ID | CNIC            | Name                | Age | Region               |";
    const string tableDivider = "+----+----------------+--------------------+-----+----------------------+";
    int tablePadding = (consoleWidth - tableHeader.length()) / 2;
    
    cout << string(tablePadding, ' ') << tableDivider << endl;
    cout << string(tablePadding, ' ') << tableHeader << endl;
    cout << string(tablePadding, ' ') << tableDivider << endl;    
    for (int i = 0; i < voterCount; ++i) {
        cout << string(tablePadding, ' ');
        cout << "| " << left << setw(2) << voters[i]->getId() << " | ";
        cout << left << setw(14) << voters[i]->getUsername() << " | ";
        cout << left << setw(18) << voters[i]->getName() << " | ";
        cout << left << setw(3) << voters[i]->getAge() << " | ";
        cout << left << setw(20) << voters[i]->getRegion() << " |" << endl;
    }
    
    cout << string(tablePadding, ' ') << tableDivider << endl;
    cout << endl;
    cout << string(padding, ' ') << "Total Voters: " << voterCount << endl;
}

void Administrator::viewAllCandidates(Election **elections, int electionCount) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    string title = "ALL REGISTERED CANDIDATES";
    int padding = (consoleWidth - title.length()) / 2;
    cout << string(consoleWidth, '-') << endl;
    cout << string(padding, ' ') << title << endl;
    cout << string(consoleWidth, '-') << endl;
    cout << endl;
    
    if (electionCount == 0) {
        cout << string(padding, ' ') << "No elections created in the system." << endl;
        return;
    }
    
    bool candidatesFound = false;
      for (int i = 0; i < electionCount; ++i) {
        if (elections[i]->getCandidateCount() > 0) {
            candidatesFound = true;
            
            // Create centered title for this election
            string electionTitle = "Election: " + elections[i]->getTitle();
            int electionPadding = (consoleWidth - electionTitle.length()) / 2;
            cout << endl;
            cout << string(electionPadding, ' ') << electionTitle << endl;
            
            // Check if it's a local or national election
            LocalElection* local = dynamic_cast<LocalElection*>(elections[i]);
            string typeInfo;
            if (local) {
                typeInfo = "Type: Local Election (City: " + local->getCity() + ")";
            } else {
                typeInfo = "Type: National Election";
            }
            int typePadding = (consoleWidth - typeInfo.length()) / 2;
            cout << string(typePadding, ' ') << typeInfo << endl;
            
            // Status
            string statusInfo = "Status: " + string(elections[i]->getStatus() ? "Active" : "Inactive");
            int statusPadding = (consoleWidth - statusInfo.length()) / 2;
            cout << string(statusPadding, ' ') << statusInfo << endl;
            cout << endl;
            
            // Table for candidates
            const string tableHeader = "| ID | Name                | Party               | Votes    |";
            const string tableDivider = "+----+--------------------+--------------------+----------+";
            int tablePadding = (consoleWidth - tableHeader.length()) / 2;
            
            cout << string(tablePadding, ' ') << tableDivider << endl;
            cout << string(tablePadding, ' ') << tableHeader << endl;
            cout << string(tablePadding, ' ') << tableDivider << endl;
              for (int j = 0; j < elections[i]->getCandidateCount(); ++j) {
                Candidate* candidate = elections[i]->getCandidate(j);
                cout << string(tablePadding, ' ');
                cout << "| " << left << setw(2) << candidate->getId() << " | ";
                cout << left << setw(18) << candidate->getName() << " | ";
                cout << left << setw(18) << candidate->getParty() << " | ";
                cout << left << setw(8) << candidate->getVoteCount() << " |" << endl;
            }
            
            cout << string(tablePadding, ' ') << tableDivider << endl;
        }
    }
      if (!candidatesFound) {
        string noCandsMsg = "No candidates registered in any election.";
        int noCandsPadding = (consoleWidth - noCandsMsg.length()) / 2;
        cout << string(noCandsPadding, ' ') << noCandsMsg << endl;
    }
}