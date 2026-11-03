#include "../include/FileHandler.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>

using namespace std;

vector<Room> FileHandler::loadRooms(const string& filename) {
    vector<Room> rooms;

    ifstream file(filename);

    if (!file.is_open()) {
      cerr << "Error: could not open "
           << filename << std::endl;

        return rooms;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string roomID;
        string type_str;
        RoomType type;
        string capacityText;
        string maxcapacityText;
        string location;
        string availableText;

        getline(ss, roomID, ',');
        getline(ss, type_str, ',');
        getline(ss, capacityText, ',');
        getline(ss, maxcapacityText, ',');
        getline(ss, location, ',');
        getline(ss, availableText, ',');
        
        if (type_str == "Classroom") {
            type = RoomType::CLASSROOM;
        }
        else if (type_str == "Meeting room") {
            type = RoomType::MEETING_ROOM;
        }
        else if (type_str == "Study room") {
            type = RoomType::STUDY_ROOM;
        }
        else {
            type = RoomType::CLASSROOM;
        }

        
        int capacity = stoi(capacityText);
        int maxcapacity = stoi(maxcapacityText);
        bool available = (availableText == "1");

        Room room(
            roomID,
            type,
            capacity,
            maxcapacity,
            location,
            available
        );

        rooms.push_back(room);
    }

    file.close();

    return rooms;
}

bool FileHandler::saveRooms(
    const string& filename,
    const vector<Room>& rooms
) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: could not open "
                  << filename << endl;

        return false;
    }

    for (const Room& room : rooms) {
        file << room.getRoomID() << ","
             << room.getTypeString() << ","
             << room.getCapacity() << ","
             << room.getMaxcapacity() << ","
             << room.getLocation() << ","
             << room.isAvailable()
             << endl;
    }

    file.close();

    return true;
}

bool FileHandler::loadUsers(
    const string& filename,
    unordered_map<std::string, User*>& users
) {
    ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: could not open "
                  << filename << endl;

        return false;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);

        string type;
        string userID;
        string name;
        string email;
        string extraInfo;

        getline(ss, type, ',');
        getline(ss, userID, ',');
        getline(ss, name, ',');
        getline(ss, email, ',');
        getline(ss, extraInfo, ',');

        User* user = nullptr;

        if (type == "Student") {

            user = new Student(
                userID,
                name,
                email,
                extraInfo
            );
        }

        else if (type == "Staff") {

            user = new Staff(
                userID,
                name,
                email,
                extraInfo
            );
        }

        else if (type == "Admin") {
            
            user = new Admin(
                userID,
                name,
                email,
                extraInfo
            );
        }

        else {
            std::cerr << "Unknown user type: "
                      << type << std::endl;

            continue;
        }

        users[userID] = user;
    }

    file.close();

    return true;
}

bool FileHandler::saveUsers(
    const string& filename,
    const unordered_map<string, User*>& users
) {
    const string temporary = filename + ".tmp";
    ofstream file(temporary);
    if (!file) {
        return false;
    }
    for (const auto& entry : users) {
        const User* user = entry.second;
        string extra;
        if (const auto* student = dynamic_cast<const Student*>(user)) {
            extra = student->getMajor();
        } else if (const auto* staff = dynamic_cast<const Staff*>(user)) {
            extra = staff->getDepartment();
        } else if (const auto* admin = dynamic_cast<const Admin*>(user)) {
            extra = admin->getOffice();
        } else {
            file.close();
            std::remove(temporary.c_str());
            return false;
        }
        file << user->getUserType() << ',' << user->getUserID() << ','
             << user->getName() << ',' << user->getEmail() << ',' << extra << '\n';
    }
    file.close();
    if (!file || std::rename(temporary.c_str(), filename.c_str()) != 0) {
        std::remove(temporary.c_str());
        return false;
    }
    return true;
}
