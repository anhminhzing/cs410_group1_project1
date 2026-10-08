
#include <iostream>
#include "../include/Staff.h"

using namespace std;

Staff::Staff(string userID, string name, string email, string department)
    : User(userID, name, email), department(department) {
}

string Staff::getDepartment() const {
    return department;
}

string Staff::getUserType() const {
    return "Staff";
}

void Staff::displayStaff() const {
    cout << "Staff ID: " << getUserID() << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Department: " << department << endl;
}