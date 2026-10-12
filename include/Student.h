#ifndef STUDENT_H
#define STUDENT_H

#include "../include/User.h"
#include <string>

using namespace std;

class Student : public User {
private:
    string major;

public:
    Student(string userID, string name, string email,
            string major);
    // getter
    string getMajor() const;
    string getUserType() const override;
    
    void displayStd() const;
};

#endif 
