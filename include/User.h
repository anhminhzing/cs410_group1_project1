#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User {
protected:
    string userID;
    string name;
    string email;
public:
    // Constructors
    User(const string& userID, const string& name, const string& email);

    // virtual destructor for inheritance
    virtual ~User();

    // Getters
    string getUserID() const;
    string getName() const;
    string getEmail() const;
    
    // setters
    void setUserID(const string& userID);
    void setName(const string& name);
    void setEmail(const string& email);

    // User abstract
    virtual string getUserType() const = 0;

};

#endif
