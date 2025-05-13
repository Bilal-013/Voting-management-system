#include "User.h"
#include <string>

User::User() : username(""), password(""), userId(-1), name(""), age(0), region("") {}
User::User(const std::string &uname, const std::string &pwd, int id) : username(uname), password(pwd), userId(id), name(""), age(0), region("") {}
bool User::login(const std::string &uname, const std::string &pwd)
{
    return username == uname && password == pwd;
}