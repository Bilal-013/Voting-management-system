#include <iostream>
#include <fstream>
#include <direct.h> // For _mkdir on Windows
#define MAX_USERS 10
#define MAX_ELECTIONS 10
#include "Voter.h"
#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
#include "fileHandler.h"
using namespace std;

void registerVoter(Voter** voters, int& voterCount, int& nextUserId, Administrator* admin) {
    if (voterCount >= MAX_USERS) {
        cout << "Maximum number of voters reached.\n";
        return;
    }
    string newUname, newPwd;
    cout << "Enter new username: "; cin >> newUname;
    cout << "Enter new password: "; cin >> newPwd;
    // Check for duplicate username
    bool exists = false;
    for (int i = 0; i < voterCount; ++i) {
        if (voters[i]->getUsername() == newUname) {
            exists = true;
            break;
        }
    }
    if (exists) {
        cout << "Username already exists.\n";
    } else {
        voters[voterCount++] = new Voter(newUname, newPwd, nextUserId++);
        cout << "Registration successful.\n";
        // Save users after registration
        User** userPtrs = new User*[voterCount+1];
        for (int i = 0; i < voterCount; ++i) userPtrs[i] = voters[i];
        userPtrs[voterCount] = admin;
        fileHandler::saveUsers(userPtrs, voterCount+1, "data/users.txt");
        delete[] userPtrs;
    }
}

int main() {
    // Ensure data directory exists
    _mkdir("data");
    // Check if data files exist, if not, create with dummy data
    std::ifstream userFile("data/users.txt");
    std::ifstream electionFile("data/elections.txt");
    if (!userFile.good()) {
        std::ofstream out("data/users.txt");
        out << "1 voter1 pass1\n2 voter2 pass2\n100 admin adminpass\n";
        out.close();
    }
    if (!electionFile.good()) {
        std::ofstream out("data/elections.txt");
        out << "local LocalElection1 0 0 2 Lahore\n1 Alice PartyA 0\n2 Bob PartyB 0\n";
        out << "national NationalElection1 0 0 2 Pakistan\n3 Charlie PartyC 0\n4 David PartyD 0\n";
        out.close();
    }
    userFile.close();
    electionFile.close();

    // Load elections and users from file
    Election** elections = nullptr;
    int electionCount = fileHandler::loadElections(elections, "data/elections.txt");
    Voter** voters = new Voter*[MAX_USERS];
    int voterCount = 0;
    int nextUserId = 1;
    User** loadedUsers = nullptr;
    int loadedUserCount = fileHandler::loadUsers(loadedUsers, "data/users.txt");
    Administrator* admin = nullptr;
    for (int i = 0; i < loadedUserCount; ++i) {
        // If username is 'admin', treat as admin
        if (loadedUsers[i]->getUsername() == "admin") {
            admin = new Administrator(loadedUsers[i]->getUsername(), loadedUsers[i]->getPassword());
        } else {
            voters[voterCount++] = new Voter(loadedUsers[i]->getUsername(), loadedUsers[i]->getPassword(), loadedUsers[i]->getId());
            if (loadedUsers[i]->getId() >= nextUserId) nextUserId = loadedUsers[i]->getId() + 1;
        }
        delete loadedUsers[i];
    }
    delete[] loadedUsers;
    if (!admin) admin = new Administrator("admin", "adminpass");

    bool running = true;
    while (running) {
        cout << "\n--- Online Voting System ---\n";
        cout << "1. Login as Voter\n2. Login as Admin\n3. Exit\nSelect option: ";
        int mainChoice; cin >> mainChoice;
        if (mainChoice == 1) {
            string uname, pwd;
            cout << "Enter username: "; cin >> uname;
            cout << "Enter password: "; cin >> pwd;
            int voterIndex = -1;
            for (int i = 0; i < voterCount; ++i) {
                if (voters[i]->login(uname, pwd)) {
                    voterIndex = i;
                    break;
                }
            }
            if (voterIndex != -1) {
                cout << "Login successful.\n";
                bool voterMenu = true;
                while (voterMenu) {
                    cout << "\n--- Voter Menu ---\n";
                    cout << "1. View Elections\n2. Vote\n3. Check Vote Status\n4. Logout\nSelect option: ";
                    int vOpt; cin >> vOpt;
                    if (vOpt == 1) {
                        voters[voterIndex]->viewElections(elections, electionCount);
                    } else if (vOpt == 2) {
                        // Check if there is at least one active election
                        bool anyActive = false;
                        for (int i = 0; i < electionCount; ++i) {
                            if (elections[i]->getStatus()) {
                                anyActive = true;
                                break;
                            }
                        }
                        if (!anyActive) {
                            cout << "No election is currently active. You cannot vote.\n";
                            continue;
                        }
                        voters[voterIndex]->viewElections(elections, electionCount);
                        cout << "Select election number: ";
                        int electionChoice; cin >> electionChoice;
                        if (electionChoice < 1 || electionChoice > electionCount) { cout << "Invalid election.\n"; continue; }
                        if (!elections[electionChoice-1]->getStatus()) {
                            cout << "This election is not active.\n";
                            continue;
                        }
                        elections[electionChoice-1]->displayCandidates();
                        cout << "Select candidate number: ";
                        int candidateChoice; cin >> candidateChoice;
                        voters[voterIndex]->castVote(elections[electionChoice-1], electionChoice-1, candidateChoice-1);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (vOpt == 3) {
                        voters[voterIndex]->viewElections(elections, electionCount);
                        cout << "Select election number: ";
                        int electionChoice; cin >> electionChoice;
                        voters[voterIndex]->checkVoteStatus(electionChoice-1);
                    } else if (vOpt == 4) {
                        voterMenu = false;
                    } else {
                        cout << "Invalid option.\n";
                    }
                }
            } else {
                cout << "Login failed.\n";
            }
        } else if (mainChoice == 2) {
            string uname, pwd;
            cout << "Enter admin username: "; cin >> uname;
            cout << "Enter admin password: "; cin >> pwd;
            if (admin->login(uname, pwd)) {
                cout << "Admin login successful.\n";
                bool adminMenu = true;
                while (adminMenu) {
                    cout << "\n--- Admin Menu ---\n";
                    cout << "1. Create Election\n2. Add Candidate\n3. Edit Candidate\n4. Remove Candidate\n5. Start Election\n6. End Election\n7. View Results\n8. Logout\nSelect option: ";
                    int aOpt; cin >> aOpt;
                    if (aOpt == 1) {
                        if (electionCount >= MAX_ELECTIONS) { cout << "Max elections reached.\n"; continue; }
                        string type, title;
                        cout << "Enter election type (local/national): "; cin >> type;
                        cout << "Enter election title: "; cin.ignore(); getline(cin, title);
                        if (type == "local") {
                            string city;
                            cout << "Enter city name: "; cin >> city;
                            elections[electionCount++] = new LocalElection(title, city);
                        } else if (type == "national") {
                            // Only allow one active national election
                            bool activeNational = false;
                            for (int i = 0; i < electionCount; ++i) {
                                if (dynamic_cast<NationalElection*>(elections[i]) && elections[i]->getStatus()) {
                                    activeNational = true;
                                    break;
                                }
                            }
                            if (activeNational) {
                                cout << "Another national election is already active!\n";
                                continue;
                            }
                            elections[electionCount++] = new NationalElection(title);
                        }
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (aOpt == 2) {
                        if (electionCount == 0) { cout << "No elections available. Create an election first.\n"; continue; }
                        for (int i = 0; i < electionCount; ++i) cout << i+1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: "; int eIdx; cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount) { cout << "Invalid election.\n"; continue; }
                        string cname, cparty;
                        cout << "Enter candidate name: "; cin >> cname;
                        cout << "Enter party: "; cin >> cparty;
                        if (!elections[eIdx-1]->isPartyAllowed(cparty)) {
                            cout << "Party already exists in this election!\n";
                            continue;
                        }
                        elections[eIdx-1]->addCandidate(cname, cparty);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (aOpt == 3) {
                        if (electionCount == 0) { cout << "No elections available.\n"; continue; }
                        for (int i = 0; i < electionCount; ++i) cout << i+1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: "; int eIdx; cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount) { cout << "Invalid election.\n"; continue; }
                        elections[eIdx-1]->displayCandidates();
                        cout << "Select candidate number: "; int cIdx; cin >> cIdx;
                        string newName, newParty;
                        cout << "Enter new candidate name: "; cin >> newName;
                        cout << "Enter new party: "; cin >> newParty;
                        admin->editCandidate(elections[eIdx-1], cIdx-1, newName, newParty);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (aOpt == 4) {
                        if (electionCount == 0) { cout << "No elections available.\n"; continue; }
                        for (int i = 0; i < electionCount; ++i) cout << i+1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: "; int eIdx; cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount) { cout << "Invalid election.\n"; continue; }
                        elections[eIdx-1]->displayCandidates();
                        cout << "Select candidate number to remove: "; int cIdx; cin >> cIdx;
                        admin->removeCandidate(elections[eIdx-1], cIdx-1);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (aOpt == 5) {
                        if (electionCount == 0) { cout << "No elections available.\n"; continue; }
                        for (int i = 0; i < electionCount; ++i) cout << i+1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: "; int eIdx; cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount) { cout << "Invalid election.\n"; continue; }
                        admin->startElection(elections[eIdx-1], elections, electionCount);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (aOpt == 6) {
                        if (electionCount == 0) { cout << "No elections available.\n"; continue; }
                        for (int i = 0; i < electionCount; ++i) cout << i+1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: "; int eIdx; cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount) { cout << "Invalid election.\n"; continue; }
                        admin->endElection(elections[eIdx-1]);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                    } else if (aOpt == 7) {
                        if (electionCount == 0) { cout << "No elections available.\n"; continue; }
                        for (int i = 0; i < electionCount; ++i) cout << i+1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: "; int eIdx; cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount) { cout << "Invalid election.\n"; continue; }
                        admin->viewResults(elections[eIdx-1]);
                    } else if (aOpt == 8) {
                        adminMenu = false;
                    } else {
                        cout << "Invalid option.\n";
                    }
                }
            } else {
                cout << "Admin login failed.\n";
            }
        } else if (mainChoice == 3) {
            running = false;
        } else {
            cout << "Invalid option.\n";
        }
    }
    // Save users, elections to data folder using dynamic arrays
    User** userPtrs = new User*[voterCount+1];
    for (int i = 0; i < voterCount; ++i) userPtrs[i] = voters[i];
    userPtrs[voterCount] = admin;
    fileHandler::saveUsers(userPtrs, voterCount+1, "data/users.txt");
    fileHandler::saveElections(elections, electionCount, "data/elections.txt");
    delete[] userPtrs;
    for (int i = 0; i < voterCount; ++i) delete voters[i];
    delete[] voters;
    for (int i = 0; i < electionCount; ++i) delete elections[i];
    delete[] elections;
    delete admin;
    return 0;
}