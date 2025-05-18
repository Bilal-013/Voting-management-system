#include "Voter.h"
#include <iostream>
#include <windows.h> // For console information
#include "ColorConsole.h" // For colored output
using namespace std;

Voter::Voter() : User()
{
    for (int i = 0; i < MAX_ELECTIONS; ++i)
        hasVoted[i] = false;
}
Voter::Voter(const string &uname, const string &pwd, int id) : User(uname, pwd, id)
{
    for (int i = 0; i < MAX_ELECTIONS; ++i)
        hasVoted[i] = false;
}
Voter::Voter(const string &uname, const string &pwd, int id, const string &name, int age, const string &region)
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
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    string title = "AVAILABLE ELECTIONS";
    int padding = (consoleWidth - title.length()) / 2;
    cout << string(consoleWidth, '-') << endl;
    cout << string(padding, ' ') << title << endl;
    cout << string(consoleWidth, '-') << endl;
    cout << endl;
    
    bool found = false;
    for (int i = 0; i < count; ++i)
    {
        if (elections[i]->getStatus()) {
            found = true;
            string item = to_string(i + 1) + ". " + elections[i]->getTitle();
            cout << string(padding, ' ') << item << endl;
        }
    }
    
    if (!found) {
        cout << string(padding, ' ') << "No active elections available." << endl;
    }
    
    cout << endl;
}
void Voter::castVote(Election *election, int electionIndex, int candidateIndex)
{
    if (!hasVoted[electionIndex])
    {
        if (election->isVotingOpen())
        {
            election->castVote(candidateIndex);
            hasVoted[electionIndex] = true;
            ColorConsole::printColored("Vote cast successfully.\n", ColorConsole::Color::LIGHTGREEN);
        }
        else
        {
            ColorConsole::printColored("Election is not active or voting period has ended.\n", ColorConsole::Color::LIGHTRED);
        }
    }
    else
    {
        ColorConsole::printColored("You have already voted.\n", ColorConsole::Color::YELLOW);
    }
}
void Voter::checkVoteStatus(int electionIndex)
{
    if (hasVoted[electionIndex]) {
        ColorConsole::printColored("Voted", ColorConsole::Color::LIGHTGREEN);
    } else {
        ColorConsole::printColored("Not Voted", ColorConsole::Color::YELLOW);
    }
    cout << endl;
}