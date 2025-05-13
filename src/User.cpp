#include "User.h"
#include <string>

User::User() : username(""), password(""), userId(-1) {}
User::User(const std::string &uname, const std::string &pwd, int id) : username(uname), password(pwd), userId(id) {}
bool User::login(const std::string &uname, const std::string &pwd)
{
    return username == uname && password == pwd;
}