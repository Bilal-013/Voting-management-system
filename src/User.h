#ifndef USER_H
#define USER_H
#include <string>

class User
{
protected:
    std::string username;
    std::string password;
    int userId;
    std::string name;
    int age;
    std::string region;

public:
    User();
    User(const std::string &uname, const std::string &pwd, int id = -1);
    virtual bool login(const std::string &uname, const std::string &pwd);
    virtual ~User() {}

    const std::string &getUsername() const { return username; }
    const std::string &getPassword() const { return password; }
    int getId() const { return userId; }
    void setId(int id) { userId = id; }
    void setUsername(const std::string &uname) { username = uname; }
    void setPassword(const std::string &pwd) { password = pwd; }

    const std::string &getName() const { return name; }
    void setName(const std::string &n) { name = n; }
    int getAge() const { return age; }
    void setAge(int a) { age = a; }
    const std::string &getRegion() const { return region; }
    void setRegion(const std::string &r) { region = r; }
};

#endif