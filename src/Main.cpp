#include <iostream>
#include <fstream>
#include <direct.h> // For _mkdir on Windows
#include <cstdlib>
#define MAX_USERS 10
#define MAX_ELECTIONS 10
#include "Voter.h"
#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
#include "fileHandler.h"
using namespace std;

void registerVoter(Voter **voters, int &voterCount, int &nextUserId, Administrator *admin)
{
    if (voterCount >= MAX_USERS)
    {
        cout << "Maximum number of voters reached.\n";
        return;
    }
    string newCNIC, newPwd, newName, newRegion;
    int newAge;
    cout << "Enter CNIC: ";
    cin >> newCNIC;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, newName);
    cout << "Enter age: ";
    cin >> newAge;
    cout << "Enter region: ";
    cin >> newRegion;
    cout << "Enter new password: ";
    cin >> newPwd;
    bool exists = false;
    for (int i = 0; i < voterCount; ++i)
    {
        if (voters[i]->getUsername() == newCNIC)
        {
            exists = true;
            break;
        }
    }
    if (exists)
    {
        cout << "A voter with this CNIC already exists.\n";
    }
    else
    {
        voters[voterCount++] = new Voter(newCNIC, newPwd, nextUserId++, newName, newAge, newRegion);
        cout << "Registration successful.\n";
        User **userPtrs = new User *[voterCount + 1];
        for (int i = 0; i < voterCount; ++i)
            userPtrs[i] = voters[i];
        userPtrs[voterCount] = admin;
        fileHandler::saveUsers(userPtrs, voterCount + 1, "data/users.txt");
        delete[] userPtrs;
    }
}

int main()
{

    _mkdir("data");
    // Check if data files exist, if not, create with dummy data
    std::ifstream userFile("data/users.txt");
    std::ifstream electionFile("data/elections.txt");
    if (!userFile.good())
    {
        std::ofstream out("data/users.txt");
        out << "1 voter1 pass1\n2 voter2 pass2\n100 admin adminpass\n";
        out.close();
    }
    if (!electionFile.good())
    {
        std::ofstream out("data/elections.txt");
        out << "local LocalElection1 0 0 2 Lahore\n1 Alice PartyA 0\n2 Bob PartyB 0\n";
        out << "national NationalElection1 0 0 2 Pakistan\n3 Charlie PartyC 0\n4 David PartyD 0\n";
        out.close();
    }
    userFile.close();
    electionFile.close();

    Election **elections = nullptr;
    int electionCount = fileHandler::loadElections(elections, "data/elections.txt");
    Voter **voters = new Voter *[MAX_USERS];
    int voterCount = 0;
    int nextUserId = 1;
    User **loadedUsers = nullptr;
    int loadedUserCount = fileHandler::loadUsers(loadedUsers, "data/users.txt");
    Administrator *admin = nullptr;
    for (int i = 0; i < loadedUserCount; ++i)
    {

        if (loadedUsers[i]->getUsername() == "admin")
        {
            admin = new Administrator(loadedUsers[i]->getUsername(), loadedUsers[i]->getPassword());
        }
        else
        {
            voters[voterCount++] = new Voter(loadedUsers[i]->getUsername(), loadedUsers[i]->getPassword(), loadedUsers[i]->getId());
            if (loadedUsers[i]->getId() >= nextUserId)
                nextUserId = loadedUsers[i]->getId() + 1;
        }
        delete loadedUsers[i];
    }
    delete[] loadedUsers;
    if (!admin)
        admin = new Administrator("admin", "adminpass");

    bool running = true;
    while (running)
    {
        system("cls");
        cout << "\n--- Online Voting System ---\n";
        cout << "1. Login as Voter\n2. Login as Admin\n3. Exit\nSelect option: ";
        int mainChoice;
        cin >> mainChoice;
        if (mainChoice == 1)
        {
            system("cls");
            string uname, pwd;
            cout << "Enter username: ";
            cin >> uname;
            cout << "Enter password: ";
            cin >> pwd;
            int voterIndex = -1;
            for (int i = 0; i < voterCount; ++i)
            {
                if (voters[i]->login(uname, pwd))
                {
                    voterIndex = i;
                    break;
                }
            }
            if (voterIndex != -1)
            {
                cout << "Login successful.\n";
                bool voterMenu = true;
                while (voterMenu)
                {
                    system("cls");
                    cout << "\n--- Voter Menu ---\n";
                    cout << "1. View Elections\n2. Vote\n3. Check Vote Status\n4. View Results\n5. Logout\nSelect option: ";
                    int vOpt;
                    cin >> vOpt;
                    if (vOpt == 1)
                    {
                        system("cls");
                        voters[voterIndex]->viewElections(elections, electionCount);
                        system("pause");
                    }
                    else if (vOpt == 2)
                    {
                        system("cls");
                        bool anyActive = false;
                        for (int i = 0; i < electionCount; ++i)
                        {
                            if (elections[i]->getStatus())
                            {
                                anyActive = true;
                                break;
                            }
                        }
                        if (!anyActive)
                        {
                            cout << "No election is currently active. You cannot vote.\n";
                            system("pause");
                            continue;
                        }
                        voters[voterIndex]->viewElections(elections, electionCount);
                        cout << "Select election number: ";
                        int electionChoice;
                        cin >> electionChoice;
                        if (electionChoice < 1 || electionChoice > electionCount)
                        {
                            cout << "Invalid election.\n";
                            system("pause");
                            continue;
                        }
                        if (!elections[electionChoice - 1]->getStatus())
                        {
                            cout << "This election is not active.\n";
                            system("pause");
                            continue;
                        }
                        // Restrict voting in local elections to region
                        LocalElection* local = dynamic_cast<LocalElection*>(elections[electionChoice - 1]);
                        if (local) {
                            if (voters[voterIndex]->getRegion() != local->getCity()) {
                                cout << "You are not eligible to vote in this local election. Only residents of " << local->getCity() << " can vote.\n";
                                system("pause");
                                continue;
                            }
                        }
                        // Show all candidates before voting
                        cout << "Candidates participating in this election:\n";
                        elections[electionChoice - 1]->displayCandidates();
                        cout << "Select candidate number: ";
                        int candidateChoice;
                        cin >> candidateChoice;
                        voters[voterIndex]->castVote(elections[electionChoice - 1], electionChoice - 1, candidateChoice - 1);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                        system("pause");
                    }
                    else if (vOpt == 3)
                    {
                        system("cls");
                        voters[voterIndex]->viewElections(elections, electionCount);
                        cout << "Select election number: ";
                        int electionChoice;
                        cin >> electionChoice;
                        voters[voterIndex]->checkVoteStatus(electionChoice - 1);
                        system("pause");
                    }
                    else if (vOpt == 4)
                    {
                        system("cls");
                        cout << "Select election to view results:\n";
                        for (int i = 0; i < electionCount; ++i)
                        {
                            cout << i + 1 << ". " << elections[i]->getTitle() << endl;
                        }
                        cout << "Enter election number: ";
                        int eIdx;
                        cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount)
                        {
                            cout << "Invalid election.\n";
                            system("pause");
                            continue;
                        }
                        if (elections[eIdx - 1]->getStatus())
                        {
                            cout << "Election is still active. Results will be available after it ends.\n";
                            system("pause");
                            continue;
                        }
                        cout << "Results for: " << elections[eIdx - 1]->getTitle() << "\n";
                        int maxVotes = 1;
                        for (int c = 0; c < elections[eIdx - 1]->getCandidateCount(); ++c)
                        {
                            int votes = elections[eIdx - 1]->getCandidate(c)->getVoteCount();
                            if (votes > maxVotes)
                                maxVotes = votes;
                        }
                        for (int c = 0; c < elections[eIdx - 1]->getCandidateCount(); ++c)
                        {
                            Candidate *cand = elections[eIdx - 1]->getCandidate(c);
                            cout << cand->getName() << " (" << cand->getVoteCount() << ") ";
                            int barLen = (maxVotes > 0) ? (cand->getVoteCount() * 10 / maxVotes) : 0;
                            for (int b = 0; b < barLen; ++b)
                                cout << "#";
                            cout << endl;
                        }
                        system("pause");
                    }
                    else if (vOpt == 5)
                    {
                        voterMenu = false;
                    }
                    else
                    {
                        cout << "Invalid option.\n";
                        system("pause");
                    }
                }
            }
            else
            {
                cout << "Login failed.\n";
                system("pause");
            }
        }
        else if (mainChoice == 2)
        {
            system("cls");
            string uname, pwd;
            cout << "Enter admin username: ";
            cin >> uname;
            cout << "Enter admin password: ";
            cin >> pwd;
            if (admin->login(uname, pwd))
            {
                cout << "Admin login successful.\n";
                bool adminMenu = true;
                while (adminMenu)
                {
                    system("cls");
                    cout << "\n--- Admin Menu ---\n";
                    cout << "1. Create Election\n2. Add Candidate\n3. Add Voter\n4. Edit Voter\n5. Remove Voter\n6. Start Election\n7. End Election\n8. View Results\n9. Logout\nSelect option: ";
                    int aOpt;
                    cin >> aOpt;
                    if (aOpt == 1)
                    {
                        system("cls");
                        if (electionCount >= MAX_ELECTIONS)
                        {
                            cout << "Max elections reached.\n";
                            system("pause");
                            continue;
                        }
                        string type, title;
                        cout << "Enter election type (local/national): ";
                        cin >> type;
                        cout << "Enter election title: ";
                        cin.ignore();
                        getline(cin, title);
                        if (type == "local")
                        {
                            string city;
                            cout << "Enter city name: ";
                            cin >> city;
                            elections[electionCount++] = new LocalElection(title, city);
                        }
                        else if (type == "national")
                        {
                            bool activeNational = false;
                            for (int i = 0; i < electionCount; ++i)
                            {
                                if (dynamic_cast<NationalElection *>(elections[i]) && elections[i]->getStatus())
                                {
                                    activeNational = true;
                                    break;
                                }
                            }
                            if (activeNational)
                            {
                                cout << "Another national election is already active!\n";
                                system("pause");
                                continue;
                            }
                            elections[electionCount++] = new NationalElection(title);
                        }
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                        system("pause");
                    }
                    else if (aOpt == 2)
                    {
                        system("cls");
                        if (electionCount == 0)
                        {
                            cout << "No elections available. Create an election first.\n";
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < electionCount; ++i)
                            cout << i + 1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: ";
                        int eIdx;
                        cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount)
                        {
                            cout << "Invalid election.\n";
                            system("pause");
                            continue;
                        }
                        string cname, cparty;
                        cout << "Enter candidate name: ";
                        cin >> cname;
                        cout << "Enter party: ";
                        cin >> cparty;
                        if (!elections[eIdx - 1]->isPartyAllowed(cparty))
                        {
                            cout << "A candidate from this party already exists in this local election!\n";
                            system("pause");
                            continue;
                        }
                        elections[eIdx - 1]->addCandidate(cname, cparty);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                        system("pause");
                    }
                    else if (aOpt == 3) // Add Voter
                    {
                        system("cls");
                        registerVoter(voters, voterCount, nextUserId, admin);
                        system("pause");
                    }
                    else if (aOpt == 4) // Edit Voter
                    {
                        system("cls");
                        if (voterCount == 0) {
                            cout << "No voters available.\n";
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < voterCount; ++i)
                            cout << i + 1 << ". CNIC: " << voters[i]->getUsername() << ", Name: " << voters[i]->getName() << ", Age: " << voters[i]->getAge() << ", Region: " << voters[i]->getRegion() << endl;
                        cout << "Select voter number to edit: ";
                        int vIdx;
                        cin >> vIdx;
                        if (vIdx < 1 || vIdx > voterCount) {
                            cout << "Invalid voter.\n";
                            system("pause");
                            continue;
                        }
                        string newCNIC, newPwd, newName, newRegion;
                        int newAge;
                        cout << "Enter new CNIC: ";
                        cin >> newCNIC;
                        // Check for duplicate CNIC except for the current voter
                        bool exists = false;
                        for (int i = 0; i < voterCount; ++i) {
                            if (i != vIdx - 1 && voters[i]->getUsername() == newCNIC) {
                                exists = true;
                                break;
                            }
                        }
                        if (exists) {
                            cout << "A voter with this CNIC already exists.\n";
                            system("pause");
                            continue;
                        }
                        cout << "Enter new name: ";
                        cin.ignore();
                        getline(cin, newName);
                        cout << "Enter new age: ";
                        cin >> newAge;
                        cout << "Enter new region: ";
                        cin >> newRegion;
                        cout << "Enter new password: ";
                        cin >> newPwd;
                        voters[vIdx - 1]->setUsername(newCNIC);
                        voters[vIdx - 1]->setPassword(newPwd);
                        voters[vIdx - 1]->setName(newName);
                        voters[vIdx - 1]->setAge(newAge);
                        voters[vIdx - 1]->setRegion(newRegion);
                        User **userPtrs = new User *[voterCount + 1];
                        for (int i = 0; i < voterCount; ++i)
                            userPtrs[i] = voters[i];
                        userPtrs[voterCount] = admin;
                        fileHandler::saveUsers(userPtrs, voterCount + 1, "data/users.txt");
                        delete[] userPtrs;
                        cout << "Voter updated.\n";
                        system("pause");
                    }
                    else if (aOpt == 5) // Remove Voter
                    {
                        system("cls");
                        if (voterCount == 0) {
                            cout << "No voters available.\n";
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < voterCount; ++i)
                            cout << i + 1 << ". CNIC: " << voters[i]->getUsername() << endl;
                        cout << "Select voter number to remove: ";
                        int vIdx;
                        cin >> vIdx;
                        if (vIdx < 1 || vIdx > voterCount) {
                            cout << "Invalid voter.\n";
                            system("pause");
                            continue;
                        }
                        delete voters[vIdx - 1];
                        for (int i = vIdx - 1; i < voterCount - 1; ++i)
                            voters[i] = voters[i + 1];
                        --voterCount;
                        User **userPtrs = new User *[voterCount + 1];
                        for (int i = 0; i < voterCount; ++i)
                            userPtrs[i] = voters[i];
                        userPtrs[voterCount] = admin;
                        fileHandler::saveUsers(userPtrs, voterCount + 1, "data/users.txt");
                        delete[] userPtrs;
                        cout << "Voter removed.\n";
                        system("pause");
                    }
                    else if (aOpt == 6)
                    {
                        system("cls");
                        if (electionCount == 0)
                        {
                            cout << "No elections available.\n";
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < electionCount; ++i)
                            cout << i + 1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: ";
                        int eIdx;
                        cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount)
                        {
                            cout << "Invalid election.\n";
                            system("pause");
                            continue;
                        }
                        admin->startElection(elections[eIdx - 1], elections, electionCount);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                        system("pause");
                    }
                    else if (aOpt == 7)
                    {
                        system("cls");
                        if (electionCount == 0)
                        {
                            cout << "No elections available.\n";
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < electionCount; ++i)
                            cout << i + 1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: ";
                        int eIdx;
                        cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount)
                        {
                            cout << "Invalid election.\n";
                            system("pause");
                            continue;
                        }
                        admin->endElection(elections[eIdx - 1]);
                        fileHandler::saveElections(elections, electionCount, "data/elections.txt");
                        system("pause");
                    }
                    else if (aOpt == 8)
                    {
                        system("cls");
                        if (electionCount == 0)
                        {
                            cout << "No elections available.\n";
                            system("pause");
                            continue;
                        }
                        for (int i = 0; i < electionCount; ++i)
                            cout << i + 1 << ". " << elections[i]->getTitle() << endl;
                        cout << "Select election number: ";
                        int eIdx;
                        cin >> eIdx;
                        if (eIdx < 1 || eIdx > electionCount)
                        {
                            cout << "Invalid election.\n";
                            system("pause");
                            continue;
                        }
                        admin->viewResults(elections[eIdx - 1]);
                        system("pause");
                    }
                    else if (aOpt == 9)
                    {
                        adminMenu = false;
                    }
                    else
                    {
                        cout << "Invalid option.\n";
                        system("pause");
                    }
                }
            }
            else
            {
                cout << "Admin login failed.\n";
                system("pause");
            }
        }
        else if (mainChoice == 3)
        {
            running = false;
        }
        else
        {
            cout << "Invalid option.\n";
            system("pause");
        }
    }
    User **userPtrs = new User *[voterCount + 1];
    for (int i = 0; i < voterCount; ++i)
        userPtrs[i] = voters[i];
    userPtrs[voterCount] = admin;
    fileHandler::saveUsers(userPtrs, voterCount + 1, "data/users.txt");
    fileHandler::saveElections(elections, electionCount, "data/elections.txt");
    delete[] userPtrs;
    for (int i = 0; i < voterCount; ++i)
        delete voters[i];
    delete[] voters;
    for (int i = 0; i < electionCount; ++i)
        delete elections[i];
    delete[] elections;
    delete admin;
    return 0;
}