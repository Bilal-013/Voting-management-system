#ifndef USER_H
#define USER_H
#include <string>

class User
{
protected:
    std::string username;
    std::string password;
    int userId;

public:
    User();
    User(const std::string &uname, const std::string &pwd, int id = -1);
    virtual bool login(const std::string &uname, const std::string &pwd);
    virtual ~User() {}

    const std::string &getUsername() const { return username; }
    const std::string &getPassword() const { return password; }
    int getId() const { return userId; }
    void setId(int id) { userId = id; }
};

#endif