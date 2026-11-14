#ifndef STUDENT_H
#define STUDENT_H

#include "User.h"
#include <string>

class Student : public User {
private:
    std::string major;

public:
    Student(std::string userID, std::string name, std::string email,
            std::string major);

    // Getters
    std::string getMajor() const;
    std::string getUserType() const override;

    void displayStd() const;
};

#endif
