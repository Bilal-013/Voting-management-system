#ifndef USER_H
#define USER_H
#include <string>
using namespace std;
class User
{
protected:
    string username;
    string password;
    int userId;
    string name;
    int age;
    string region;

public:
    User();
    User(const string &uname, const string &pwd, int id = -1);
    virtual bool login(const string &uname, const string &pwd);
    virtual ~User() {}

    const string &getUsername() const { return username; }
    const string &getPassword() const { return password; }
    int getId() const { return userId; }
    void setId(int id) { userId = id; }
    void setUsername(const string &uname) { username = uname; }
    void setPassword(const string &pwd) { password = pwd; }

    const string &getName() const { return name; }
    void setName(const string &n) { name = n; }
    int getAge() const { return age; }
    void setAge(int a) { age = a; }
    const string &getRegion() const { return region; }
    void setRegion(const string &r) { region = r; }
};

#endif