#include <iostream>
#include <fstream>
#include <direct.h> // For _mkdir on Windows
#include <cstdlib>
#include <iomanip> // For setw
#define MAX_USERS 10
#define MAX_ELECTIONS 10
#include "Voter.h"
#include "Administrator.h"
#include "LocalElection.h"
#include "NationalElection.h"
#include "fileHandler.h"
#include <windows.h> // For getting console information
#include "ColorConsole.h" // For colored console output
using namespace std;

// Helper functions are now provided by ColorConsole.h

// Helper function to check if string contains only letters and spaces
bool containsOnlyLetters(const string &str) {
    for (char c : str) {
        if (!isalpha(c) && !isspace(c)) {
            return false;
        }
    }
    return true;
}

void registerVoter(Voter **voters, int &voterCount, int &nextUserId, Administrator *admin)
{
    if (voterCount >= MAX_USERS)
    {
        cout << "Maximum number of voters reached.\n";
        return;
    }
    string newCNIC, newPwd, newName, newRegion;
    int newAge;
    
    // CNIC validation - must be exactly 13 digits
    bool validCNIC = false;
    while (!validCNIC) {
        cout << "Enter CNIC (must be exactly 13 digits): ";
        cin >> newCNIC;
        
        // Check if CNIC is 13 digits
        if (newCNIC.length() != 13) {
            cout << "Error: CNIC must be exactly 13 digits. Try again.\n";
            continue;
        }
        
        // Check if CNIC contains only digits
        bool onlyDigits = true;
        for (char c : newCNIC) {
            if (!isdigit(c)) {
                onlyDigits = false;
                break;
            }
        }
        
        if (!onlyDigits) {
            cout << "Error: CNIC must contain only digits. Try again.\n";
            continue;
        }
        
        validCNIC = true;
    }
    
    // Name validation - must not contain numbers
    bool validName = false;
    while (!validName) {
        cout << "Enter name (must not contain numbers): ";
        cin.ignore();
        getline(cin, newName);
        
        if (!containsOnlyLetters(newName)) {
            cout << "Error: Name must not contain numbers or special characters. Try again.\n";
            continue;
        }
        
        validName = true;
    }
    
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
    Administrator *admin = nullptr;    for (int i = 0; i < loadedUserCount; ++i)
    {
        if (loadedUsers[i]->getUsername() == "admin")
        {
            admin = new Administrator(loadedUsers[i]->getUsername(), loadedUsers[i]->getPassword());
        }
        else
        {
            // Cast the generic User to a Voter to maintain all voter data
            Voter* loadedVoter = dynamic_cast<Voter*>(loadedUsers[i]);
            if (loadedVoter) {
                // Create a new voter with all the information
                voters[voterCount++] = new Voter(
                    loadedVoter->getUsername(), 
                    loadedVoter->getPassword(), 
                    loadedVoter->getId(),
                    loadedVoter->getName(),
                    loadedVoter->getAge(),
                    loadedVoter->getRegion()
                );
                
                if (loadedVoter->getId() >= nextUserId)
                    nextUserId = loadedVoter->getId() + 1;
            }
        }
        delete loadedUsers[i];
    }
    delete[] loadedUsers;
    if (!admin)
        admin = new Administrator("admin", "adminpass");

    bool running = true;    while (running)
    {
        system("cls");
        ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTCYAN);
        ColorConsole::centerColoredText("ONLINE VOTING SYSTEM", ColorConsole::Color::YELLOW);
        ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTCYAN);
        cout << endl;
        ColorConsole::centerColoredText("1. Login as Voter", ColorConsole::Color::LIGHTGREEN);
        ColorConsole::centerColoredText("2. Login as Admin", ColorConsole::Color::LIGHTMAGENTA);
        ColorConsole::centerColoredText("3. Exit", ColorConsole::Color::LIGHTRED);
        cout << endl;
        ColorConsole::coloredDivider('-', ColorConsole::Color::LIGHTCYAN);
        ColorConsole::setTextColor(ColorConsole::Color::WHITE);
        cout << "Select option: ";
        int mainChoice;
        cin >> mainChoice;        if (mainChoice == 1)
        {
            system("cls");
            ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTGREEN);
            ColorConsole::centerColoredText("VOTER LOGIN", ColorConsole::Color::GREEN);
            ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTGREEN);
            cout << endl;
            string uname, pwd;
            ColorConsole::setTextColor(ColorConsole::Color::CYAN);
            cout << "Enter username: ";
            ColorConsole::resetColor();
            cin >> uname;
            ColorConsole::setTextColor(ColorConsole::Color::CYAN);
            cout << "Enter password: ";
            ColorConsole::resetColor();
            cin >> pwd;
            int voterIndex = -1;
            for (int i = 0; i < voterCount; ++i)
            {
                if (voters[i]->login(uname, pwd))
                {
                    voterIndex = i;
                    break;
                }
            }            if (voterIndex != -1)
            {
                ColorConsole::printColored("Login successful.\n", ColorConsole::Color::LIGHTGREEN);
                bool voterMenu = true;while (voterMenu)
                {
                    system("cls");
                    ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTGREEN);
                    ColorConsole::centerColoredText("VOTER MENU", ColorConsole::Color::GREEN);
                    ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTGREEN);
                    cout << endl;
                    ColorConsole::centerColoredText("1. View Elections", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("2. Vote", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("3. Check Vote Status", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("4. View Results", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("5. Logout", ColorConsole::Color::LIGHTRED);
                    cout << endl;
                    ColorConsole::coloredDivider('-', ColorConsole::Color::LIGHTGREEN);
                    ColorConsole::setTextColor(ColorConsole::Color::WHITE);
                    cout << "Select option: ";
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
            }            else
            {
                ColorConsole::printColored("Login failed.\n", ColorConsole::Color::LIGHTRED);
                system("pause");
            }
        }        else if (mainChoice == 2)
        {
            system("cls");
            ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTMAGENTA);
            ColorConsole::centerColoredText("ADMIN LOGIN", ColorConsole::Color::MAGENTA);
            ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTMAGENTA);
            cout << endl;
            string uname, pwd;
            ColorConsole::setTextColor(ColorConsole::Color::CYAN);
            cout << "Enter admin username: ";
            ColorConsole::resetColor();
            cin >> uname;
            ColorConsole::setTextColor(ColorConsole::Color::CYAN);
            cout << "Enter admin password: ";
            ColorConsole::resetColor();
            cin >> pwd;            if (admin->login(uname, pwd))
            {
                ColorConsole::printColored("Admin login successful.\n", ColorConsole::Color::LIGHTGREEN);
                bool adminMenu = true;while (adminMenu)
                {
                    system("cls");
                    ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTMAGENTA);
                    ColorConsole::centerColoredText("ADMIN MENU", ColorConsole::Color::MAGENTA);
                    ColorConsole::coloredDivider('=', ColorConsole::Color::LIGHTMAGENTA);
                    cout << endl;
                    
                    // Election management options
                    ColorConsole::centerColoredText("ELECTION MANAGEMENT", ColorConsole::Color::YELLOW);
                    ColorConsole::centerColoredText("1. Create Election", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("6. Start Election", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("7. End Election", ColorConsole::Color::WHITE);
                    cout << endl;
                    
                    // Candidate management
                    ColorConsole::centerColoredText("CANDIDATE MANAGEMENT", ColorConsole::Color::YELLOW);
                    ColorConsole::centerColoredText("2. Add Candidate", ColorConsole::Color::WHITE);
                    cout << endl;
                    
                    // Voter management
                    ColorConsole::centerColoredText("VOTER MANAGEMENT", ColorConsole::Color::YELLOW);
                    ColorConsole::centerColoredText("3. Add Voter", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("4. Edit Voter", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("5. Remove Voter", ColorConsole::Color::WHITE);
                    cout << endl;
                    
                    // Reporting
                    ColorConsole::centerColoredText("REPORTS", ColorConsole::Color::YELLOW);
                    ColorConsole::centerColoredText("8. View Results", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("9. View All Voters", ColorConsole::Color::WHITE);
                    ColorConsole::centerColoredText("10. View All Candidates", ColorConsole::Color::WHITE);
                    cout << endl;
                    
                    // Exit option
                    ColorConsole::centerColoredText("11. Logout", ColorConsole::Color::LIGHTRED);
                    cout << endl;
                    
                    ColorConsole::coloredDivider('-', ColorConsole::Color::LIGHTMAGENTA);
                    ColorConsole::setTextColor(ColorConsole::Color::WHITE);
                    cout << "Select option: ";
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
                    }                    else if (aOpt == 2)
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
                        
                        // Validate candidate name (must not contain numbers)
                        bool validName = false;
                        while (!validName) {
                            cout << "Enter candidate name (must not contain numbers): ";
                            cin.ignore();
                            getline(cin, cname);
                            
                            if (!containsOnlyLetters(cname)) {
                                cout << "Error: Name must not contain numbers or special characters. Try again.\n";
                                continue;
                            }
                            
                            validName = true;
                        }
                        
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
                    }                    else if (aOpt == 4) // Edit Voter
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

                        // CNIC validation - must be exactly 13 digits
                        bool validCNIC = false;
                        while (!validCNIC) {
                            cout << "Enter new CNIC (must be exactly 13 digits): ";
                            cin >> newCNIC;
                            
                            // Check if CNIC is 13 digits
                            if (newCNIC.length() != 13) {
                                cout << "Error: CNIC must be exactly 13 digits. Try again.\n";
                                continue;
                            }
                            
                            // Check if CNIC contains only digits
                            bool onlyDigits = true;
                            for (char c : newCNIC) {
                                if (!isdigit(c)) {
                                    onlyDigits = false;
                                    break;
                                }
                            }
                            
                            if (!onlyDigits) {
                                cout << "Error: CNIC must contain only digits. Try again.\n";
                                continue;
                            }
                            
                            validCNIC = true;
                        }

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

                        // Name validation - must not contain numbers
                        bool validName = false;
                        while (!validName) {
                            cout << "Enter new name (must not contain numbers): ";
                            cin.ignore();
                            getline(cin, newName);
                            
                            if (!containsOnlyLetters(newName)) {
                                cout << "Error: Name must not contain numbers or special characters. Try again.\n";
                                continue;
                            }
                            
                            validName = true;
                        }
                        
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
                    }                    else if (aOpt == 9) // View All Voters
                    {
                        system("cls");
                        admin->viewAllVoters(voters, voterCount);
                        system("pause");
                    }
                    else if (aOpt == 10) // View All Candidates
                    {
                        system("cls");
                        admin->viewAllCandidates(elections, electionCount);
                        system("pause");
                    }
                    else if (aOpt == 11) // Logout
                    {
                        adminMenu = false;
                    }
                    else
                    {
                        cout << "Invalid option.\n";
                        system("pause");
                    }
                }
            }            else
            {
                ColorConsole::printColored("Admin login failed.\n", ColorConsole::Color::LIGHTRED);
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