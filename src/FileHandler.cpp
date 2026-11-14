#include "../include/FileHandler.h"


#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>

// Include the filesystem header for file operations in windows
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

using namespace std;

// Helper function to replace a file with a temporary file to support Windows atomic file replacement
namespace {
    bool replaceFile(const string& temporary, const string& filename) {
#ifdef _WIN32
        return MoveFileExW(
            filesystem::path(temporary).c_str(),
            filesystem::path(filename).c_str(),
            MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH
        ) != 0;
#else
        return rename(temporary.c_str(), filename.c_str()) == 0;
#endif
    }
}

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
        string maxCapacityText;
        string location;
        string availableText;

        getline(ss, roomID, ',');
        getline(ss, type_str, ',');
        getline(ss, maxCapacityText, ',');
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

        
        int maxCapacity = stoi(maxCapacityText);
        bool available = (availableText == "1");

        Room room(
            roomID,
            type,
            maxCapacity,
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
             << room.getMaxCapacity() << ","
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
    if (!file || !replaceFile(temporary, filename)) {
        std::remove(temporary.c_str());
        return false;
    }
    return true;
}

bool FileHandler::loadReservations(
    const string& filename, unordered_map<string, Reservation>& reservations) {
    ifstream file(filename);
    if (!file) return false;
    unordered_map<string, Reservation> loaded;
    string line;
    while (getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        stringstream row(line);
        vector<string> fields;
        string field;
        while (getline(row, field, ',')) fields.push_back(field);
        if (fields.size() != 6 || line.back() == ',') return false;
        if (fields[0].empty() || fields[1].empty() || fields[2].empty()) return false;
        long long start, end;
        auto parseSeconds = [](const string& text, long long& value) {
            istringstream input(text);
            return (input >> value) && (input >> ws).eof();
        };
        if (!parseSeconds(fields[3], start) || !parseSeconds(fields[4], end) || end <= start) {
            return false;
        }
        using Clock = chrono::system_clock;
        const auto minimum = chrono::duration_cast<chrono::seconds>(Clock::duration::min()).count();
        const auto maximum = chrono::duration_cast<chrono::seconds>(Clock::duration::max()).count();
        if (start < minimum || start > maximum || end < minimum || end > maximum) return false;
        ReservationStatus status;
        if (fields[5] == "Confirmed") status = ReservationStatus::Confirmed;
        else if (fields[5] == "CheckedIn") status = ReservationStatus::CheckedIn;
        else if (fields[5] == "CheckedOut") status = ReservationStatus::CheckedOut;
        else if (fields[5] == "Cancelled") status = ReservationStatus::Cancelled;
        else return false;
        Reservation reservation(fields[1], fields[2], fields[0],
            Clock::time_point(chrono::seconds(start)), Clock::time_point(chrono::seconds(end)));
        reservation.setStatus(status);
        if (!loaded.emplace(fields[0], reservation).second) return false;
    }
    if (!file.eof()) return false;
    reservations.swap(loaded);
    return true;
}

bool FileHandler::saveReservations(
    const string& filename, const unordered_map<string, Reservation>& reservations) {
    const string temporary = filename + ".tmp";
    ofstream file(temporary);
    if (!file) return false;
    for (const auto& entry : reservations) {
        const Reservation& reservation = entry.second;
        const string fields[] = {reservation.getReservationID(), reservation.getOwnerID(),
                                 reservation.getRoomID()};
        for (const string& value : fields) {
            if (value.empty() || value.find_first_of(",\r\n") != string::npos) {
                file.close();
                std::remove(temporary.c_str());
                return false;
            }
        }
        string status;
        switch (reservation.getStatus()) {
            case ReservationStatus::Confirmed: status = "Confirmed"; break;
            case ReservationStatus::CheckedIn: status = "CheckedIn"; break;
            case ReservationStatus::CheckedOut: status = "CheckedOut"; break;
            case ReservationStatus::Cancelled: status = "Cancelled"; break;
            default:
                file.close();
                std::remove(temporary.c_str());
                return false;
        }
        file << fields[0] << ',' << fields[1] << ',' << fields[2] << ','
             << chrono::duration_cast<chrono::seconds>(reservation.getStartTime().time_since_epoch()).count()
             << ',' << chrono::duration_cast<chrono::seconds>(reservation.getEndTime().time_since_epoch()).count()
             << ',' << status << '\n';
    }
    file.close();
    if (!file || !replaceFile(temporary, filename)) {
        std::remove(temporary.c_str());
        return false;
    }
    return true;
}