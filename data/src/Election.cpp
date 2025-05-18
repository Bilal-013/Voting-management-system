#include "Election.h"
#include <iostream>
#include <windows.h> // For console information
#include <iomanip>   // For setw
#include "ColorConsole.h" // For colored output
using namespace std;

Election::Election() : title(""), candidateCount(0), isActive(false), durationMinutes(0), durationSeconds(0), startTime(0), endTime(0), nextCandidateId(1) {}
Election::Election(const string &t) : title(t), candidateCount(0), isActive(false), durationMinutes(0), durationSeconds(0), startTime(0), endTime(0), nextCandidateId(1) {}
void Election::addCandidate(const string &name, const string &party)
{
    if (candidateCount < MAX_CANDIDATES)
    {
        candidates[candidateCount++] = Candidate(name, party, nextCandidateId++);
    }
}
void Election::startElection(int minutes, int seconds)
{
    durationMinutes = minutes;
    durationSeconds = seconds;
    startTime = time(nullptr);
    endTime = startTime + minutes * 60 + seconds;
    isActive = true;
    cout << "Election '";
    ColorConsole::printColored(title, ColorConsole::Color::CYAN);
    cout << "' started. It will end in ";
    ColorConsole::printColored(to_string(minutes), ColorConsole::Color::YELLOW);
    cout << " minutes and ";
    ColorConsole::printColored(to_string(seconds), ColorConsole::Color::YELLOW);
    cout << " seconds.\n";
}
void Election::endElection()
{
    isActive = false;
    cout << "Election '";
    ColorConsole::printColored(title, ColorConsole::Color::CYAN);
    cout << "' ended.\n";
}
void Election::displayCandidates()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    string title = "ELECTION CANDIDATES";
    int padding = (consoleWidth - title.length()) / 2;
    cout << string(consoleWidth, '-') << endl;
    cout << string(padding, ' ') << title << endl;
    cout << string(consoleWidth, '-') << endl;
    
    if (candidateCount == 0) {
        cout << string(padding, ' ') << "No candidates registered for this election." << endl;
        return;
    }
    
    // Table for candidates
    const string tableHeader = "| No | ID | Name                | Party               | Votes    |";
    const string tableDivider = "+----+----+--------------------+--------------------+----------+";
    int tablePadding = (consoleWidth - tableHeader.length()) / 2;
    
    cout << string(tablePadding, ' ') << tableDivider << endl;
    cout << string(tablePadding, ' ') << tableHeader << endl;
    cout << string(tablePadding, ' ') << tableDivider << endl;
    
    for (int i = 0; i < candidateCount; ++i)
    {
        cout << string(tablePadding, ' ');
        cout << "| " << setw(2) << left << i + 1 << " ";
        cout << "| " << setw(2) << left << candidates[i].getId() << " | ";
        cout << setw(18) << left << candidates[i].getName() << " | ";
        cout << setw(18) << left << candidates[i].getParty() << " | ";
        cout << setw(8) << left << candidates[i].getVoteCount() << " |" << endl;
    }
    
    cout << string(tablePadding, ' ') << tableDivider << endl;
}
void Election::castVote(int candidateIndex)
{
    if (isVotingOpen() && candidateIndex >= 0 && candidateIndex < candidateCount)
    {
        candidates[candidateIndex].incrementVotes();
    }
}
void Election::editCandidate(int index, const string &newName, const string &newParty)
{
    if (index >= 0 && index < candidateCount)
    {
        int id = candidates[index].getId();
        candidates[index] = Candidate(newName, newParty, id);
    }
}
void Election::removeCandidate(int index)
{
    if (index >= 0 && index < candidateCount)
    {
        for (int i = index; i < candidateCount - 1; ++i)
        {
            candidates[i] = candidates[i + 1];
        }
        --candidateCount;
    }
}
void Election::showResults()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    string headerText = "ELECTION RESULTS: " + title;
    int padding = (consoleWidth - headerText.length()) / 2;
    cout << string(consoleWidth, '=') << endl;
    cout << string(padding, ' ') << headerText << endl;
    cout << string(consoleWidth, '=') << endl << endl;
    
    if (candidateCount == 0) {
        cout << string(padding, ' ') << "No candidates participated in this election." << endl;
        return;
    }
    
    // Find the maximum votes to scale the bar chart
    int maxVotes = 1;  // Default to 1 to avoid division by zero
    for (int i = 0; i < candidateCount; ++i) {
        if (candidates[i].getVoteCount() > maxVotes)
            maxVotes = candidates[i].getVoteCount();
    }
    
    // Table for results
    const string tableHeader = "| Rank | ID | Name                | Party               | Votes    | Bar Chart         |";
    const string tableDivider = "+------+----+--------------------+--------------------+----------+------------------+";
    int tablePadding = (consoleWidth - tableHeader.length()) / 2;
    
    cout << string(tablePadding, ' ') << tableDivider << endl;
    cout << string(tablePadding, ' ') << tableHeader << endl;
    cout << string(tablePadding, ' ') << tableDivider << endl;
    
    // Create array of indices sorted by vote count
    int* rankIndices = new int[candidateCount];
    for (int i = 0; i < candidateCount; ++i) {
        rankIndices[i] = i;
    }
    
    // Sort indices based on vote counts (simple bubble sort)
    for (int i = 0; i < candidateCount - 1; ++i) {
        for (int j = 0; j < candidateCount - i - 1; ++j) {
            if (candidates[rankIndices[j]].getVoteCount() < candidates[rankIndices[j+1]].getVoteCount()) {
                int temp = rankIndices[j];
                rankIndices[j] = rankIndices[j+1];
                rankIndices[j+1] = temp;
            }
        }
    }
    
    // Display results in ranked order
    for (int rank = 0; rank < candidateCount; ++rank) {
        int i = rankIndices[rank];
        int voteCount = candidates[i].getVoteCount();
        
        // Calculate bar length (max 20 characters)
        int barLength = (maxVotes > 0) ? (voteCount * 15 / maxVotes) : 0;
        string bar = string(barLength, '#');
        
        cout << string(tablePadding, ' ');
        cout << "| " << setw(4) << left << rank + 1 << " ";
        cout << "| " << setw(2) << left << candidates[i].getId() << " | ";
        cout << setw(18) << left << candidates[i].getName() << " | ";
        cout << setw(18) << left << candidates[i].getParty() << " | ";
        cout << setw(8) << left << voteCount << " | ";
        cout << setw(16) << left << bar << " |" << endl;
    }
    
    cout << string(tablePadding, ' ') << tableDivider << endl;
    
    // Free allocated memory
    delete[] rankIndices;
}
bool Election::getStatus() const { return isActive; }
const string &Election::getTitle() const { return title; }
bool Election::isVotingOpen()
{
    if (!isActive)
        return false;
    time_t now = time(nullptr);
    if (now >= endTime)
    {
        isActive = false;
        ColorConsole::printColored("Election duration is over. Voting is closed.\n", ColorConsole::Color::LIGHTRED);
        return false;
    }
    return true;
}
Candidate *Election::getCandidateById(int id)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getId() == id)
            return &candidates[i];
    }
    return nullptr;
}
bool Election::checkDuplicateCandidateId(int id)
{
    for (int i = 0; i < candidateCount; ++i)
    {
        if (candidates[i].getId() == id)
            return true;
    }
    return false;
}