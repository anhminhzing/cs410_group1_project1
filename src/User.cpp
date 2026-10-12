#include "../include/User.h"

using namespace std;

User::User(const string& userID, const string& name,
           const string& email)
    : userID(userID), name(name), email(email) {
}

User::~User() = default;

string User::getUserID() const {
    return userID;
}

string User::getName() const {
    return name;
}

string User::getEmail() const {
    return email;
}

void User::setUserID(const std::string& userID) {
    this->userID = userID;
}

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setEmail(const std::string& email) {
    this->email = email;
}
