// Manage spaces and reservations
// Add, edit, and delete reservations
// Add, edit, and delete rooms
// Manage staff and student accounts

#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <string>
using namespace std;

class Admin : public User {
private:
    string office;

public:
    Admin(string userID, string name, string email,
          string office);

    string getOffice() const;
    string getUserType() const override;
    
    void displayAdmin() const;

    // Admin-specific operations
    void deactivateAccount();
};

#endif

