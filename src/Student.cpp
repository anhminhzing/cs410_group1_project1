
#include "../include/Student.h"
#include <iostream>

using namespace std;


Student::Student(string userID, string name, string email,
                 string major)
    : User(userID, name, email), major(major) {
}

string Student::getMajor() const {
    return major;
}

string Student::getUserType() const {
    return "Student";
}

void Student::displayStd() const {
    cout << "Student ID: " << getUserID() << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Major: " << major << endl;
}