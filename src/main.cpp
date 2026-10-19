#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "Room.h"
#include "User.h"
#include "Student.h"
#include "Staff.h"
#include "Admin.h"
#include "FileHandler.h"
#include "UserManagementSystem.h"
#include "ReservationManagementSystem.h"

using namespace std;

// =====================================
// Helper function to convert user input before parsing it into the reservation system 
// =====================================
bool convertToTimePoint(
    const string& date,
    const string& time,
    chrono::system_clock::time_point& result){
    tm timeInfo = {};

    istringstream input(date + " " + time);

    input >> get_time(
        &timeInfo,
        "%m/%d/%Y %H:%M"
    );

    if (input.fail()){
        return false;
    }
    timeInfo.tm_isdst = -1;

    time_t convertedTime = mktime(&timeInfo);

    if (convertedTime == -1){
        return false;
    }
    result =
        chrono::system_clock::from_time_t(
            convertedTime
        );
    return true;
}


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
    // Creating a Reservation Management System
    //===========================================
    ReservationManagementSystem reservationSystem;

    for (const Room& room : rooms){
        reservationSystem.addRoom(room);
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

    // ==========================================
    // User Login 
    // ==========================================
    string userID;

    cout << endl;
    cout << "===== LOGIN =====" << endl;
    cout << "Enter User ID: ";
    cin >> userID;

    // Searching for user
    auto userIt = 
        userSystem.getUsers().find(userID);

    if (userIt == userSystem.getUsers().end()) {
        cout << "User not found." << endl;
        return 1;
    }

    // Storing the current logged in user 
    User* currentUser = userIt->second;

    cout << endl;
    cout << "Welcome,  "
         << currentUser->getName()
         << "!" << endl;

    // ===========================================
    // Main Reservation Menu
    // ===========================================
    int choice = 0;
    while (choice != 3){
        cout << endl;
        cout << "===== CAMPUS ROOM RESERVATION =====" << endl;
        cout << "1. Make a Reservation" << endl;
        cout << "2. Cancel a Reservation" << endl;
        cout << "3. Exit" << endl;

        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 1){
            string date;
            string startTime;
            string endTime;
            int capacity;

            cout << endl;
            cout << "==== MAKE RESERVATION ====." << endl;

            cout << "Enter Date (MM/DD/YYYY): ";
            cin >> date;

            cout << "Enter Start Time (HH:MM): ";
            cin >> startTime;

            cout << "Enter End Time (HH:MM): ";
            cin >> endTime;

            cout << "Enter Capacity: ";
            cin >> capacity;

            chrono::system_clock::time_point reqStart;
            chrono::system_clock::time_point reqEnd;

            bool validStart = 
                convertToTimePoint(
                    date,
                    startTime,
                    reqStart
                );
            bool validEnd =
                convertToTimePoint(
                    date, 
                    endTime,
                    reqEnd
                );

            if (!validStart || !validEnd){
                cout << "Invalid date or time format." << endl;
                continue;
            }
            if (reqEnd <= reqStart){
                cout << "End Time must be after Start Time" << endl;
                continue;
            }
            if (capacity <= 0){
                cout << "Capacity should be greater than 0" << endl;
                continue;
            }

            // Finding rooms that meet the user's criteria 
            vector<Room> availableRooms;

            for (const Room& room : rooms){
                if (room.canAccommodate(capacity)){
                    if (room.isAvailable()){
                        if (reservationSystem.isRoomAvailable(
                                room.getRoomID(),
                                reqStart,
                                reqEnd)){
                            availableRooms.push_back(room);
                        }
                    }
                }
            }

            // With no rooms found 
            if (availableRooms.empty()){
                cout << "No rooms are available for this selection." << endl;
                continue;
            }

            //Displaying available rooms 
            cout << endl;
            cout << "=== AVAILABLE ROOMS ===" << endl;
            
            for (const Room& room : availableRooms){
                room.displayRoom();
                cout << "------------------" << endl;
            }

            // User selects room 
            string roomID;
            cout << "Enter Room ID: ";
            cin >> roomID;

            // Verifying the room is available 
            bool validRoom = false;
            for (const Room& room : availableRooms){
                if (room.getRoomID() == roomID){
                    validRoom = true;
                    break;
                }
            }
            if (!validRoom){
                cout << "Invalid Room ID." << endl;
                continue;
            }

            // Creating Reservation 
            bool success = 
                reservationSystem.createReservation(roomID, currentUser->getUserID(), reqStart, reqEnd);
            if (success){
                cout << "Reservation created successfully." << endl;
            } else {
                cout << "Reservation unsuccessful." << endl;
            }
            
        } else if (choice == 2){
            cout << "Cancel Reservation selected." << endl;
        } else if (choice == 3){
            cout << "Goodbye" << endl;
        } else {
            cout << "Invalid choice" << endl;
        }
    }

    return 0;
}
