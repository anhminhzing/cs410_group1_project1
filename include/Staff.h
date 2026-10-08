
#ifndef STAFF_H
#define STAFF_H

#include "User.h"
#include <string>

using namespace std;

class Staff : public User {
private:
    string department;

public:
    Staff(string userID, string name, string email, string department);

    string getDepartment() const;
    string getUserType() const override;

    void displayStaff() const;
};

#endif