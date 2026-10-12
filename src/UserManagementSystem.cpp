#include "UserManagementSystem.h"

#include "Admin.h"

#include <iostream>

using namespace std;

UserManagementSystem::UserManagementSystem() {
}

// Destructor
UserManagementSystem::~UserManagementSystem() {
    for (auto& pair : users) {
        delete pair.second;
    }

    users.clear();
}

User* UserManagementSystem::getUser(
    const string& userID
) {
    auto it = users.find(userID);

    if (it != users.end()) {
        return it->second;
    }

    return nullptr;
}

std::unordered_map<string, User*>&
UserManagementSystem::getUsers() {
    return users;
}

bool UserManagementSystem::addUser(User* user) {
    if (user == nullptr) {
        return false;
    }

    std::string userID = user->getUserID();

    // Do not allow duplicate IDs
    if (users.find(userID) != users.end()) {
        std::cout << "User ID already exists."
                  << std::endl;

        return false;
    }

    users[userID] = user;

    return true;
}

bool UserManagementSystem::deleteUser(
    const std::string& userID
) {
    auto it = users.find(userID);

    if (it == users.end()) {
        return false;
    }

    delete it->second;

    users.erase(it);

    return true;
}

bool UserManagementSystem::userExists(
    const string& userID
) const {
    return users.find(userID) != users.end();
}

bool UserManagementSystem::isAdmin(
    const string& userID
) {
    User* user = getUser(userID);

    if (user == nullptr) {
        return false;
    }

    return dynamic_cast<Admin*>(user) != nullptr;
}
