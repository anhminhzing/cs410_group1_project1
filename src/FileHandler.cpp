// windows.h must come first so these macros take effect before it is parsed.
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

#include "FileHandler.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Replace `filename` with `temporary` in one step (MoveFileExW on Windows,
// rename elsewhere) so a failed save never leaves a half-written file.
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

    // Remove a Windows '\r' left by getline. Returns false for blank lines.
    bool cleanLine(string& line) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        return line.find_first_not_of(" \t") != string::npos;
    }

    // Accept only a whole, positive number such as "30" (not "", "abc", "3x").
    bool parseCapacity(const string& text, int& value) {
        istringstream input(text);
        return (input >> value) && (input >> ws).eof() && value > 0;
    }
}

vector<Room> FileHandler::loadRooms(const string& filename) {
    vector<Room> rooms;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << endl;
        return rooms;
    }

    string line;

    while (getline(file, line)) {
        if (!cleanLine(line)) {
            continue;
        }
        stringstream ss(line);

        string roomID;
        string type_str;
        string maxCapacityText;
        string location;
        string availableText;
        string extraField;

        // Read exactly five fields.
        if (!getline(ss, roomID, ',') ||
            !getline(ss, type_str, ',') ||
            !getline(ss, maxCapacityText, ',') ||
            !getline(ss, location, ',') ||
            !getline(ss, availableText, ',')) {
            cerr << "Skipping invalid room record: " << line << endl;
            continue;
        }

        // Reject records containing extra fields.
        if (getline(ss, extraField, ',')) {
            cerr << "Skipping room with extra fields: " << line << endl;
            continue;
        }

        // Validate room ID and location.
        if (roomID.empty() || location.empty()) {
            cerr << "Skipping room with missing required fields: "
                 << line << endl;
            continue;
        }

        // Validate room type.
        RoomType type;
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
            cerr << "Skipping room with unknown type: " << line << endl;
            continue;
        }

        // Validate capacity.
        int maxCapacity = 0;

        if (!parseCapacity(maxCapacityText, maxCapacity)) {
            cerr << "Skipping room with invalid capacity: " << line << endl;
            continue;
        }

        // Validate availability.
        if (availableText != "0" && availableText != "1") {
            cerr << "Skipping room with invalid availability: "
                 << line << endl;
            continue;
        }

        bool available = (availableText == "1");

        rooms.emplace_back(
            roomID,
            type,
            maxCapacity,
            location,
            available
        );
    }

    return rooms;
}

bool FileHandler::saveRooms(
    const string& filename,
    const vector<Room>& rooms
) {
    // Write to a temporary file first so a failed save keeps the old rooms.
    const string temporary = filename + ".tmp";
    ofstream file(temporary);

    if (!file.is_open()) {
        cerr << "Error: could not open "
                  << temporary << endl;

        return false;
    }

    for (const Room& room : rooms) {
        file << room.getRoomID() << ","
             << room.getTypeString() << ","
             << room.getMaxCapacity() << ","
             << room.getLocation() << ","
             << room.isAvailable()
             << '\n';
    }

    file.close();

    if (!file || !replaceFile(temporary, filename)) {
        std::remove(temporary.c_str());
        return false;
    }

    return true;
}

bool FileHandler::loadUsers(
    const string& filename,
    unordered_map<string, User*>& users
) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: could not open " << filename << endl;
        return false;
    }

    string line;

    while (getline(file, line)) {
        if (!cleanLine(line)) {
            continue;
        }

        stringstream ss(line);
        vector<string> fields;
        string field;

        // Read all fields.
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // Exactly five fields are required.
        if (fields.size() != 5) {
            cerr << "Skipping invalid user record: "
                 << line << endl;
            continue;
        }

        const string& type = fields[0];
        const string& userID = fields[1];
        const string& name = fields[2];
        const string& email = fields[3];
        const string& extraInfo = fields[4];

        // Validate required fields.
        if (type.empty() || userID.empty() ||
            name.empty() || email.empty() ||
            extraInfo.empty()) {
            cerr << "Skipping user with missing fields: "
                 << line << endl;
            continue;
        }

        // Keep the first record for duplicate IDs.
        if (users.count(userID) != 0) {
            cerr << "Skipping duplicate user ID: "
                 << userID << endl;
            continue;
        }

        User* user = nullptr;

        if (type == "Student") {
            user = new Student(userID, name, email, extraInfo);
        }
        else if (type == "Staff") {
            user = new Staff(userID, name, email, extraInfo);
        }
        else if (type == "Admin") {
            user = new Admin(userID, name, email, extraInfo);
        }
        else {
            cerr << "Unknown user type: " << type << endl;
            continue;
        }

        users[userID] = user;
    }

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
