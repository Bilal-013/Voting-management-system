#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <string>
#include "Candidate.h"
#include "Election.h"
#include "User.h"
using namespace std;
class fileHandler
{
public:
    static void saveCandidates(Candidate *candidates, int candidateCount, const string &filename);
    static int loadCandidates(Candidate *&candidates, const string &filename);
    static void saveElections(Election **elections, int electionCount, const string &filename);
    static int loadElections(Election **&elections, const string &filename);
    static void saveUsers(User **users, int userCount, const string &filename);
    static int loadUsers(User **&users, const string &filename);
};
#endif
