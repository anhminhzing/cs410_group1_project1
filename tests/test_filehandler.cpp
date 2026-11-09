#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "FileHandler.h"
#include "UserManagementSystem.h"

using namespace std;

namespace {
// Each test run gets its own directory and never touches data/.
class TestFiles {
    filesystem::path directory;

public:
    TestFiles() {
        directory = filesystem::temp_directory_path() /
            ("cs410-filehandler-" + to_string(chrono::steady_clock::now().time_since_epoch().count()));
        assert(filesystem::create_directory(directory));
    }

    ~TestFiles() {
        error_code error;
        filesystem::remove_all(directory, error);
    }

    string path(const string& name) const {
        return (directory / name).string();
    }

    void write(const string& name, const string& content) const {
        ofstream output(path(name));
        output << content;
        output.close();
        assert(output);
    }

    string read(const string& name) const {
        ifstream input(path(name));
        assert(input);
        ostringstream content;
        content << input.rdbuf();
        return content.str();
    }
};

void checkRooms(const vector<Room>& rooms) {
    assert(rooms.size() == 3);
    const string ids[] = {"R101", "R102", "R103"};
    const RoomType types[] = {RoomType::CLASSROOM, RoomType::STUDY_ROOM, RoomType::MEETING_ROOM};
    const int limits[] = {30, 40, 20};
    const string locations[] = {"Library", "Campus Center", "Science Building"};
    for (size_t index = 0; index < rooms.size(); ++index) {
        const Room& room = rooms[index];
        assert(room.getRoomID() == ids[index]);
        assert(room.getType() == types[index]);
        assert(room.getMaxCapacity() == limits[index]);
        assert(room.getLocation() == locations[index]);
        assert(room.isAvailable() == (index != 2));
    }
}

void testRoomLoadAndSave(const TestFiles& files) {
    files.write("rooms.txt",
        "R101,Classroom,30,Library,1\n"
        "R102,Study room,40,Campus Center,1\n"
        "R103,Meeting room,20,Science Building,0\n");
    const auto rooms = FileHandler::loadRooms(files.path("rooms.txt"));
    checkRooms(rooms);
    assert(FileHandler::saveRooms(files.path("saved_rooms.txt"), rooms));
    checkRooms(FileHandler::loadRooms(files.path("saved_rooms.txt")));
    cout << "Room loading and save/reload passed.\n";
}

void checkUsers(UserManagementSystem& users) {
    assert(users.getUsers().size() == 3);
    const auto* student = dynamic_cast<const Student*>(users.getUser("S001"));
    const auto* staff = dynamic_cast<const Staff*>(users.getUser("T001"));
    const auto* admin = dynamic_cast<const Admin*>(users.getUser("A001"));
    assert(student && staff && admin);
    assert(student->getUserID() == "S001");
    assert(student->getName() == "Test Student");
    assert(student->getEmail() == "student@example.edu");
    assert(student->getUserType() == "Student");
    assert(student->getMajor() == "Computer Science");
    assert(staff->getUserID() == "T001");
    assert(staff->getName() == "Test Staff");
    assert(staff->getEmail() == "staff@example.edu");
    assert(staff->getUserType() == "Staff");
    assert(staff->getDepartment() == "Facilities");
    assert(admin->getUserID() == "A001");
    assert(admin->getName() == "Test Admin");
    assert(admin->getEmail() == "admin@example.edu");
    assert(admin->getUserType() == "Admin");
    assert(admin->getOffice() == "Main Office");
}

void testUserLoadAndSave(const TestFiles& files) {
    files.write("users.txt",
        "Student,S001,Test Student,student@example.edu,Computer Science\n"
        "Staff,T001,Test Staff,staff@example.edu,Facilities\n"
        "Admin,A001,Test Admin,admin@example.edu,Main Office\n");
    // UserManagementSystem owns and deletes the pointers allocated by loadUsers.
    UserManagementSystem users;
    assert(FileHandler::loadUsers(files.path("users.txt"), users.getUsers()));
    checkUsers(users);
    assert(FileHandler::saveUsers(files.path("saved_users.txt"), users.getUsers()));
    UserManagementSystem reloaded;
    assert(FileHandler::loadUsers(files.path("saved_users.txt"), reloaded.getUsers()));
    checkUsers(reloaded); // Compare fields, not unordered_map iteration order.
    assert(!filesystem::exists(files.path("saved_users.txt.tmp")));
    cout << "User loading and save/reload passed.\n";
}

void testReservationLoadAndSave(const TestFiles& files) {
    // Raw text tests parsing independently of the serializer, including CRLF.
    files.write("reservations.txt",
        "RES_1,S001,R101,1800000000,1800003600,Confirmed\r\n"
        "RES_2,T001,R102,1800000000,1800003600,CheckedIn\r\n"
        "RES_3,S001,R101,1800003600,1800007200,CheckedOut\r\n"
        "RES_4,A001,R103,1800000000,1800003600,Cancelled\r\n");
    unordered_map<string, Reservation> records;
    assert(FileHandler::loadReservations(files.path("reservations.txt"), records));
    assert(records.size() == 4);
    assert(records.at("RES_1").getOwnerID() == "S001");
    assert(records.at("RES_2").getRoomID() == "R102");
    assert(records.at("RES_1").getStartTime() ==
           chrono::system_clock::time_point(chrono::seconds(1800000000)));
    assert(records.at("RES_1").getEndTime() ==
           chrono::system_clock::time_point(chrono::seconds(1800003600)));
    const ReservationStatus statuses[] = {ReservationStatus::Confirmed, ReservationStatus::CheckedIn,
        ReservationStatus::CheckedOut, ReservationStatus::Cancelled};
    for (size_t index = 0; index < 4; ++index) {
        assert(records.at("RES_" + to_string(index + 1)).getStatus() == statuses[index]);
    }
    assert(FileHandler::saveReservations(files.path("saved_reservations.txt"), records));
    unordered_map<string, Reservation> reloaded;
    assert(FileHandler::loadReservations(files.path("saved_reservations.txt"), reloaded));
    assert(reloaded.size() == records.size());
    for (const auto& entry : records) {
        const Reservation& expected = entry.second;
        const Reservation& actual = reloaded.at(entry.first);
        assert(actual.getReservationID() == expected.getReservationID());
        assert(actual.getOwnerID() == expected.getOwnerID());
        assert(actual.getRoomID() == expected.getRoomID());
        assert(actual.getStartTime() == expected.getStartTime());
        assert(actual.getEndTime() == expected.getEndTime());
        assert(actual.getStatus() == expected.getStatus());
    }
    assert(!filesystem::exists(files.path("saved_reservations.txt.tmp")));
    cout << "Reservation fields, statuses, and save/reload passed.\n";
}

void testInvalidReservations(const TestFiles& files) {
    const string valid = "RES_1,S001,R101,1800000000,1800003600,Confirmed\n";
    files.write("valid.txt", valid);
    unordered_map<string, Reservation> records;
    assert(FileHandler::loadReservations(files.path("valid.txt"), records));
    const vector<string> invalid = {
        "RES_2,S001,R101,1800000000,1800003600\n", // Missing status.
        "RES_2,S001,R101,1800000000,1800003600,Confirmed,extra\n",
        "RES_2,S001,R101,abc,1800003600,Confirmed\n",
        "RES_2,S001,R101,1800003600,1800000000,Confirmed\n", // Reversed interval.
        "RES_2,S001,R101,1800000000,1800000000,Confirmed\n", // Zero duration.
        "RES_2,S001,R101,1800000000,1800003600,Unknown\n",
        "RES_2,,R101,1800000000,1800003600,Confirmed\n",
        valid // Duplicate ID after the valid first row.
    };
    for (const string& row : invalid) {
        files.write("invalid.txt", valid + row);
        assert(!FileHandler::loadReservations(files.path("invalid.txt"), records));
        assert(records.size() == 1);
        assert(records.at("RES_1").getOwnerID() == "S001");
        assert(records.at("RES_1").getStatus() == ReservationStatus::Confirmed);
    }
    cout << "Invalid reservation input preserves loaded records.\n";
}

void testMissingAndEmptyFiles(const TestFiles& files) {
    assert(FileHandler::loadRooms(files.path("missing_rooms.txt")).empty());
    UserManagementSystem users;
    assert(!FileHandler::loadUsers(files.path("missing_users.txt"), users.getUsers()));
    unordered_map<string, Reservation> reservations;
    assert(!FileHandler::loadReservations(files.path("missing_reservations.txt"), reservations));

    files.write("empty.txt", "");
    assert(FileHandler::loadRooms(files.path("empty.txt")).empty());
    assert(FileHandler::loadUsers(files.path("empty.txt"), users.getUsers()));
    assert(users.getUsers().empty());
    assert(FileHandler::loadReservations(files.path("empty.txt"), reservations));
    assert(reservations.empty());
    assert(FileHandler::saveRooms(files.path("empty_rooms.txt"), {}));
    assert(FileHandler::saveUsers(files.path("empty_users.txt"), {}));
    assert(FileHandler::saveReservations(files.path("empty_reservations.txt"), {}));
    assert(files.read("empty_rooms.txt").empty());
    assert(files.read("empty_users.txt").empty());
    assert(files.read("empty_reservations.txt").empty());
    cout << "Missing and empty file cases passed.\n";
}

void testSaveFailures(const TestFiles& files) {
    const string missingDirectory = files.path("not_created/records.txt");
    assert(!FileHandler::saveRooms(missingDirectory, {}));
    assert(!FileHandler::saveUsers(missingDirectory, {}));
    assert(!FileHandler::saveReservations(missingDirectory, {}));

    // A directory blocks opening the temporary file on all supported systems.
    files.write("protected_users.txt", "original users\n");
    files.write("protected_reservations.txt", "original reservations\n");
    assert(filesystem::create_directory(files.path("protected_users.txt.tmp")));
    assert(filesystem::create_directory(files.path("protected_reservations.txt.tmp")));
    assert(!FileHandler::saveUsers(files.path("protected_users.txt"), {}));
    assert(!FileHandler::saveReservations(files.path("protected_reservations.txt"), {}));
    assert(files.read("protected_users.txt") == "original users\n");
    assert(files.read("protected_reservations.txt") == "original reservations\n");
    cout << "Save failures preserve existing user/reservation files.\n";
}
} // namespace

int main() {
    const TestFiles files;
    testRoomLoadAndSave(files);
    testUserLoadAndSave(files);
    testReservationLoadAndSave(files);
    testInvalidReservations(files);
    testMissingAndEmptyFiles(files);
    testSaveFailures(files);
    cout << "\nAll FileHandler tests passed!\n";
}
