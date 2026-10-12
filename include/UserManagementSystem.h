#ifndef USER_MANAGEMENT_SYSTEM_H
#define USER_MANAGEMENT_SYSTEM_H

#include <string>
#include <unordered_map>

#include "User.h"
using namespace std;

class UserManagementSystem {
private:
    unordered_map<string, User*> users;

public:
    UserManagementSystem();

    ~UserManagementSystem();

    // Get one user by ID
    User* getUser(const std::string& userID);

    // Access all users
    unordered_map<std::string, User*>& getUsers();

    // User management
    bool addUser(User* user);

    bool deleteUser(const string& userID);

    bool userExists(const string& userID) const;

    // Check whether a user is an Admin
    bool isAdmin(const string& userID);
};

#endif
