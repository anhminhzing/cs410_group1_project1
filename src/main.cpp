#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "FileHandler.h"
#include "ReservationManagementSystem.h"
#include "UserManagementSystem.h"

using namespace std;

namespace {
using TimePoint = chrono::system_clock::time_point;

bool readInput(const string& prompt, string& value) {
    cout << prompt;
    return static_cast<bool>(getline(cin, value));
}

bool parsePositiveInt(const string& text, int& value) {
    istringstream input(text);
    return (input >> value) && (input >> ws).eof() && value > 0;
}

bool parseDateTime(const string& date, const string& time,
                   TimePoint& result) {
    tm info{};
    istringstream input(date + " " + time);
    input >> get_time(&info, "%m/%d/%Y %H:%M");
    if (input.fail() || !(input >> ws).eof()) {
        return false;
    }

    const tm requested = info;
    info.tm_isdst = -1;
    const time_t converted = mktime(&info);
    // mktime normalizes impossible dates, so check the resulting fields.
    if (converted == -1 || info.tm_year != requested.tm_year ||
        info.tm_mon != requested.tm_mon || info.tm_mday != requested.tm_mday ||
        info.tm_hour != requested.tm_hour || info.tm_min != requested.tm_min) {
        return false;
    }
    result = chrono::system_clock::from_time_t(converted);
    return true;
}

void showUsers(UserManagementSystem& users) {
    vector<string> ids;
    for (const auto& entry : users.getUsers()) {
        ids.push_back(entry.first);
    }
    sort(ids.begin(), ids.end());
    for (const auto& id : ids) {
        const User* user = users.getUser(id);
        cout << id << " | " << user->getName()
                  << " | " << user->getUserType() << '\n';
    }
}

void showUserDetails(UserManagementSystem& users) {
    showUsers(users);
    for (const auto& entry : users.getUsers()) {
        const User* user = entry.second;
        cout << "\nID: " << user->getUserID() << "\nName: " << user->getName()
             << "\nEmail: " << user->getEmail() << "\nRole: " << user->getUserType();
        if (const auto* student = dynamic_cast<const Student*>(user)) {
            cout << "\nMajor: " << student->getMajor();
        } else if (const auto* staff = dynamic_cast<const Staff*>(user)) {
            cout << "\nDepartment: " << staff->getDepartment();
        } else if (const auto* admin = dynamic_cast<const Admin*>(user)) {
            cout << "\nOffice: " << admin->getOffice();
        }
        cout << '\n';
    }
}

bool readField(const string& prompt, string& value) {
    while (readInput(prompt, value)) {
        if (value.find_first_not_of(" \t\r") != string::npos &&
            value.find_first_of(",\r\n") == string::npos) {
            return true;
        }
        cout << "Enter a nonempty value without commas.\n";
    }
    return false;
}

unique_ptr<User> readUser(const string& id, const User* existing) {
    string role, name, email, extra;
    cout << "Enter account details" << (existing ? " (ID stays unchanged)" : "") << ".\n";
    while (readInput("Role (1. Student, 2. Staff, 3. Admin): ", role)) {
        if (role == "1" || role == "2" || role == "3") {
            break;
        }
        cout << "Please choose 1, 2, or 3.\n";
    }
    if (!cin || !readField("Name: ", name) || !readField("Email: ", email) ||
        !readField(role == "1" ? "Major: " : role == "2" ? "Department: " : "Office: ", extra)) {
        return nullptr;
    }
    if (role == "1") return make_unique<Student>(id, name, email, extra);
    if (role == "2") return make_unique<Staff>(id, name, email, extra);
    return make_unique<Admin>(id, name, email, extra);
}

void manageUsers(const string& adminID, UserManagementSystem& users) {
    // Check permission at the entry point as well as in the session menu.
    if (!users.isAdmin(adminID)) return;
    string choice;
    while (cin) {
        cout << "\n===== USER MANAGEMENT =====\n"
             << "1. View users\n2. Add user\n3. Edit user\n4. Delete user\n0. Back\n";
        if (!readInput("Enter choice: ", choice) || choice == "0") return;
        if (choice == "1") {
            showUserDetails(users);
            continue;
        }
        if (choice != "2" && choice != "3" && choice != "4") {
            cout << "Invalid choice.\n";
            continue;
        }
        string id;
        if (!readField("User ID (or 0 to go back): ", id)) return;
        if (id == "0") continue;
        if (id.find_first_of(" \t") != string::npos) {
            cout << "User IDs cannot contain spaces.\n";
            continue;
        }
        User* existing = users.getUser(id);
        if ((choice == "2" && existing) || (choice != "2" && !existing)) {
            cout << (existing ? "User ID already exists.\n" : "User ID not found.\n");
            continue;
        }
        if (choice == "4" && id == adminID) {
            cout << "You cannot delete your logged-in account.\n";
            continue;
        }

        unique_ptr<User> replacement;
        auto updated = users.getUsers();
        if (choice == "4") {
            updated.erase(id);
        } else {
            replacement = readUser(id, existing);
            if (!replacement) return;
            if (id == adminID && replacement->getUserType() != "Admin") {
                cout << "Your logged-in account must keep the Admin role.\n";
                continue;
            }
            updated[id] = replacement.get();
        }
        // Save the proposed records before changing the live accounts.
        if (!FileHandler::saveUsers("data/users.txt", updated)) {
            cout << "Could not save users. No account changes were applied.\n";
            continue;
        }
        users.getUsers().swap(updated);
        replacement.release();
        delete existing;
        cout << "User " << (choice == "2" ? "added" : choice == "3" ? "updated" : "deleted")
             << " successfully. Changes saved.\n";
    }
}

User* login(UserManagementSystem& users) {
    cout << "\n===== LOGIN =====\n"
              << "Available accounts (ID | Name | Role):\n";
    showUsers(users);
    string id;
    while (readInput("Enter your User ID (or 0 to exit): ", id)) {
        if (id == "0") {
            return nullptr;
        }
        if (User* user = users.getUser(id)) {
            return user;
        }
        cout << "User ID not found. Please try again.\n";
    }
    return nullptr;
}

void showRooms(const vector<Room>& rooms) {
    if (rooms.empty()) {
        cout << "No rooms found.\n";
    }
    for (const Room& room : rooms) {
        room.displayRoom();
        cout << "---------------------\n";
    }
}

void makeReservation(const User& user, const vector<Room>& rooms,
                     ReservationManagementSystem& reservations) {
    string date, start, end, capacityText;
    if (!readInput("Date (MM/DD/YYYY): ", date) ||
        !readInput("Start time (HH:MM, 24-hour): ", start) ||
        !readInput("End time (HH:MM, 24-hour): ", end) ||
        !readInput("Number of attendees: ", capacityText)) {
        return;
    }

    TimePoint startTime, endTime;
    int capacity = 0;
    if (!parseDateTime(date, start, startTime) ||
        !parseDateTime(date, end, endTime)) {
        cout << "Invalid date or time.\n";
        return;
    }
    if (endTime <= startTime) {
        cout << "End time must be after start time.\n";
        return;
    }
    if (!parsePositiveInt(capacityText, capacity)) {
        cout << "Number of attendees must be a positive whole number.\n";
        return;
    }

    vector<Room> available;
    for (const Room& room : rooms) {
        if (room.isAvailable() && room.canAccommodate(capacity) &&
            reservations.isRoomAvailable(room.getRoomID(), startTime, endTime)) {
            available.push_back(room);
        }
    }
    if (available.empty()) {
        cout << "No rooms are available for this selection.\n";
        return;
    }
    cout << "\n===== AVAILABLE ROOMS =====\n";
    showRooms(available);
    string roomID;
    if (!readInput("Enter Room ID (or 0 to go back): ", roomID) || roomID == "0") {
        return;
    }
    const auto selected = find_if(available.begin(), available.end(),
        [&roomID](const Room& room) { return room.getRoomID() == roomID; });
    if (selected == available.end()) {
        cout << "Please select one of the available room IDs.\n";
        return;
    }
    reservations.createReservation(roomID, user.getUserID(), startTime, endTime);
}

void cancelReservation(const User& user, bool admin,
                       ReservationManagementSystem& reservations, bool deleteRecord = false) {
    bool found = false;
    for (const auto& entry : reservations.getReservations()) {
        const Reservation& reservation = entry.second;
        if ((deleteRecord || reservation.getStatus() != ReservationStatus::Cancelled) &&
            (admin || reservation.getOwnerID() == user.getUserID())) {
            cout << entry.first << " | Room: " << reservation.getRoomID()
                      << " | User: " << reservation.getOwnerID() << '\n';
            found = true;
        }
    }
    if (!found) {
        cout << "No matching reservations available.\n";
        return;
    }
    string id;
    if (!readInput(deleteRecord ? "Reservation ID to delete (or 0 to go back): " :
                   "Reservation ID to cancel (or 0 to go back): ", id) || id == "0") {
        return;
    }
    const auto& records = reservations.getReservations();
    const auto selected = records.find(id);
    if (selected == records.end() ||
        (!deleteRecord && selected->second.getStatus() == ReservationStatus::Cancelled) ||
        (!admin && selected->second.getOwnerID() != user.getUserID())) {
        cout << "Please select one of the listed reservation IDs.\n";
        return;
    }
    if (deleteRecord) reservations.deleteReservation(id);
    else reservations.cancelReservation(id);
}

void runSession(User& user, UserManagementSystem& users,
                const vector<Room>& rooms,
                ReservationManagementSystem& reservations) {
    const bool admin = users.isAdmin(user.getUserID());
    const string userID = user.getUserID();
    cout << "\nWelcome, " << user.getName() << " (" << user.getUserType() << ")!\n";
    string choice;
    while (cin) {
        // An admin may edit their own account, replacing its User object.
        User& currentUser = *users.getUser(userID);
        cout << "\n===== " << currentUser.getUserType() << " MENU =====\n"
                  << "1. View rooms\n"
                  << "2. Search available rooms and make a reservation\n"
                  << (admin ? "3. Cancel any reservation\n" : "3. Cancel my reservation\n");
        if (admin) {
            cout << "4. Manage user accounts\n5. Delete reservation permanently\n";
        }
        cout << "0. Log out\n";
        if (!readInput("Enter choice: ", choice) || choice == "0") {
            return;
        }
        if (choice == "1") {
            showRooms(rooms);
        } else if (choice == "2") {
            makeReservation(currentUser, rooms, reservations);
        } else if (choice == "3") {
            cancelReservation(currentUser, admin, reservations);
        } else if (choice == "4" && admin) {
            manageUsers(userID, users);
        } else if (choice == "5" && admin) {
            cancelReservation(currentUser, admin, reservations, true);
        } else {
            cout << "Invalid choice. Please select a listed option.\n";
        }
    }
}
} // namespace

int main() {
    UserManagementSystem users;
    if (!FileHandler::loadUsers("data/users.txt", users.getUsers()) ||
        users.getUsers().empty()) {
        cerr << "No user accounts could be loaded from data/users.txt.\n";
        return 1;
    }
    const vector<Room> rooms = FileHandler::loadRooms("data/rooms.txt");
    ReservationManagementSystem reservations;
    for (const Room& room : rooms) {
        reservations.addRoom(room);
    }

    if (!reservations.loadReservations("data/reservations.txt")) {
        cerr << "Could not load reservation data. Check data/reservations.txt.\n";
        return 1;
    }

    cout << "===== CAMPUS ROOM RESERVATION =====\n"
              << "Sign in with an existing User ID to access your role's functions.\n"
              << "Reservations are loaded and saved automatically.\n";
    while (User* user = login(users)) {
        runSession(*user, users, rooms, reservations);
    }
    cout << "Goodbye!\n";
    return 0;
}
