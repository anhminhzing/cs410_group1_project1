#include <iostream>
#include <vector>
#include <unordered_map>

#include "Room.h"
#include "User.h"
#include "Student.h"
#include "Staff.h"
#include "Admin.h"
#include "FileHandler.h"
#include "UserManagementSystem.h"

using namespace std;

int main() {

    // ==========================================
    // Load rooms
    // ==========================================
    vector<Room> rooms =
        FileHandler::loadRooms("data/rooms.txt");

    cout << "===== ROOM LIST =====" << endl;

    for (const Room& room : rooms) {
        room.displayRoom();
        cout << "---------------------" << endl;
    }


    // ==========================================
    // Load users
    // ==========================================
    UserManagementSystem userSystem;

    bool usersLoaded =
        FileHandler::loadUsers(
            "data/users.txt",
            userSystem.getUsers()
        );

    if (!usersLoaded) {
        cout << "Failed to load users." << endl;
        return 1;
    }

    cout << endl;
    cout << "===== USER LIST =====" << endl;

    for (const auto& pair : userSystem.getUsers()) {

        User* user = pair.second;

        cout << "User ID: "
             << user->getUserID()
             << endl;

        cout << "Name: "
             << user->getName()
             << endl;

        cout << "Email: "
             << user->getEmail()
             << endl;

        cout << "User Type: "
             << user->getUserType()
             << endl;


        // Student information
        if (Student* student =
                dynamic_cast<Student*>(user)) {

            cout << "Major: "
                 << student->getMajor()
                 << endl;
        }

        // Staff information
        else if (Staff* staff =
                     dynamic_cast<Staff*>(user)) {

            cout << "Department: "
                 << staff->getDepartment()
                 << endl;
        }

        // Admin information
        else if (Admin* admin =
                     dynamic_cast<Admin*>(user)) {

            cout << "Office: "
                 << admin->getOffice()
                 << endl;
        }

        cout << "---------------------" << endl;
    }


    return 0;
}
