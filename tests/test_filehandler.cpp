#include <cassert>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "../include/Room.h"
#include "../include/User.h"
#include "../include/Student.h"
#include "../include/Staff.h"
#include "../include/Admin.h"
#include "../include/FileHandler.h"

using namespace std;


// =====================================================
// Test loading rooms
// =====================================================
void testLoadRooms() {

    vector<Room> rooms =
        FileHandler::loadRooms("data/rooms.txt");

    // Check number of rooms
    assert(rooms.size() == 3);

    // First room
    assert(rooms[0].getRoomID() == "R101");
    assert(rooms[0].getTypeString() == "Classroom");
    assert(rooms[0].getCapacity() == 10);
    assert(rooms[0].getMaxcapacity() == 30);
    assert(rooms[0].getLocation() == "Library");
    assert(rooms[0].isAvailable() == true);

    // Second room
    assert(rooms[1].getRoomID() == "R102");
    assert(rooms[1].getTypeString() == "Study room");
    assert(rooms[1].getCapacity() == 20);
    assert(rooms[1].getMaxcapacity() == 40);

    // Third room
    assert(rooms[2].getRoomID() == "R103");
    assert(rooms[2].getTypeString() == "Meeting room");
    assert(rooms[2].getCapacity() == 8);
    assert(rooms[2].getMaxcapacity() == 20);
    assert(rooms[2].isAvailable() == false);

    cout << "testLoadRooms passed!" << endl;
}


// =====================================================
// Test loading users
// =====================================================
void testLoadUsers() {

    unordered_map<string, User*> users;

    bool result =
        FileHandler::loadUsers(
            "data/users.txt",
            users
        );

    // Check that file loaded successfully
    assert(result == true);

    // Example users.txt has 5 users
    assert(users.size() == 5);


    // -------------------------------------------------
    // Test Student
    // -------------------------------------------------
    assert(users.find("S001") != users.end());

    User* user1 = users["S001"];

    assert(user1 != nullptr);
    assert(user1->getUserID() == "S001");
    assert(user1->getName() == "Anh Duong");
    assert(user1->getEmail() == "anh@student.edu");
    assert(user1->getUserType() == "Student");

    Student* student =
        dynamic_cast<Student*>(user1);

    assert(student != nullptr);
    assert(student->getMajor() == "Computer Science");


    // -------------------------------------------------
    // Test Staff
    // -------------------------------------------------
    assert(users.find("T001") != users.end());

    User* user2 = users["T001"];

    assert(user2 != nullptr);
    assert(user2->getUserID() == "T001");
    assert(user2->getUserType() == "Staff");

    Staff* staff =
        dynamic_cast<Staff*>(user2);

    assert(staff != nullptr);
    assert(staff->getDepartment() == "Computer Science");


    // -------------------------------------------------
    // Test Admin
    // -------------------------------------------------
    assert(users.find("A001") != users.end());

    User* user3 = users["A001"];

    assert(user3 != nullptr);
    assert(user3->getUserID() == "A001");
    assert(user3->getUserType() == "Admin");

    Admin* admin =
        dynamic_cast<Admin*>(user3);

    assert(admin != nullptr);
    assert(admin->getOffice() == "Main Office");


    // FileHandler created these objects with new,
    // so clean them up after the test.
    for (auto& pair : users) {
        delete pair.second;
    }

    users.clear();

    cout << "testLoadUsers passed!" << endl;
}


// =====================================================
// Main test runner
// =====================================================
int main() {

    testLoadRooms();

    testLoadUsers();

    cout << "\nAll FileHandler tests passed!"
         << endl;

    return 0;
}
