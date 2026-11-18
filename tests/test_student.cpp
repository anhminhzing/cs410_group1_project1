#include <iostream>
#include <cassert>
#include "../include/Student.h"

using namespace std;

int main() {
    Student std("S001", "My",
                    "thidiemmy.huynh001@umb.edu", "Computer Science");

    // Test inherited User information
    assert(std.getUserID() == "S001");
    assert(std.getName() == "My");
    assert(std.getEmail() == "thidiemmy.huynh001@umb.edu");

    // Test Student-specific information
    assert(std.getMajor() == "Computer Science");
    assert(std.getUserType() == "Student");
    std.displayStd();
    cout << "-----------------------------------STUDENT TESTS PASSED---------------------------------\n" << endl;

    return 0;
}

