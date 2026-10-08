//
//  Admin.cpp
//  
//
//  Created by Anh Minh Duong on 10/6/26.
//

#include "../include/Admin.h"
#include <iostream>


using namespace std;

Admin::Admin(string userID, string name, string email,
             string office)
    : User(userID, name, email), office(office) {
}

string Admin::getOffice() const {
    return office;
}

string Admin::getUserType() const {
    return "Admin";
}

void Admin::displayAdmin() const {
    cout << "Admin ID: " << getUserID() << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Office: " << office << endl;
}

void Admin::deactivateAccount() {
    // Implementation for deactivating an account
}

