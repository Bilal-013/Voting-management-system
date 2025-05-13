#ifndef FILEHANDLER_H
#define FILEHANDLER_H
#include <string>
#include "Candidate.h"
#include "Election.h"
#include "User.h"
class fileHandler
{
public:
    static void saveCandidates(Candidate *candidates, int candidateCount, const std::string &filename);
    static int loadCandidates(Candidate *&candidates, const std::string &filename);
    static void saveElections(Election **elections, int electionCount, const std::string &filename);
    static int loadElections(Election **&elections, const std::string &filename);
    static void saveUsers(User **users, int userCount, const std::string &filename);
    static int loadUsers(User **&users, const std::string &filename);
};
#endif
