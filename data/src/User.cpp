#include "User.h"
#include <string>
using namespace std;
User::User() : username(""), password(""), userId(-1), name(""), age(0), region("") {}
User::User(const string &uname, const string &pwd, int id) : username(uname), password(pwd), userId(id), name(""), age(0), region("") {}
bool User::login(const string &uname, const string &pwd)
{
    return username == uname && password == pwd;
}