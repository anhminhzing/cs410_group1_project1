#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
private:
    string userID;
    string name;
    string email;
public:
    // Constructors
    User(const string& userID, const string& name, const std::string& email);

    // virtual destructor for inheritance
    virtual ~User();

    // Getters
    string getUserID() const;
    string getName() const;
    string getEmail() const;

    // User abstract
    virtual string getUserType() const = 0;

};

#endif