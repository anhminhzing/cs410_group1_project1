#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "FileHandler.h"
#include "ReservationManagementSystem.h"
using namespace std;

int main() {
    const auto directory = filesystem::temp_directory_path() /
        ("cs410-reservations-" + to_string(chrono::steady_clock::now().time_since_epoch().count()));
    filesystem::create_directory(directory);
    const string path = (directory / "reservations.txt").string();
    const Room room("TEST", RoomType::CLASSROOM, 10, 20, "Library", true);
    const auto start = chrono::system_clock::time_point(chrono::seconds(1800000000));
    const auto end = start + chrono::hours(1);

    ReservationManagementSystem original;
    original.addRoom(room);
    assert(original.loadReservations(path)); // Missing file is initialized.
    assert(original.createReservation("TEST", "S001", start, end));
    ReservationManagementSystem restarted;
    restarted.addRoom(room);
    assert(restarted.loadReservations(path));
    assert(restarted.getReservations().at("RES_1").getOwnerID() == "S001");
    assert(restarted.getReservations().at("RES_1").getStartTime() == start);
    assert(!restarted.createReservation("TEST", "S002", start, end));
    assert(restarted.createReservation("TEST", "S002", end, end + chrono::hours(1)));
    assert(restarted.getReservations().count("RES_2") == 1);

    assert(restarted.cancelReservation("RES_1"));
    ReservationManagementSystem cancelled;
    cancelled.addRoom(room);
    assert(cancelled.loadReservations(path));
    assert(cancelled.isRoomAvailable("TEST", start, end));
    assert(cancelled.getReservations().at("RES_1").getStatus() == ReservationStatus::Cancelled);
    assert(cancelled.deleteReservation("RES_2"));
    unordered_map<string, Reservation> saved;
    assert(FileHandler::loadReservations(path, saved));
    assert(saved.count("RES_2") == 0);

    // Force atomic-save failure without relying on platform-specific permissions.
    filesystem::create_directory(path + ".tmp");
    assert(!cancelled.createReservation("TEST", "S003", start, end));
    assert(!cancelled.deleteReservation("RES_1"));
    assert(!cancelled.cancelReservation("RES_1"));
    assert(cancelled.getReservations().size() == 1);
    assert(FileHandler::loadReservations(path, saved));
    assert(saved.size() == 1);
    filesystem::remove(path + ".tmp");

    // Bad input must not replace already loaded live records or enable overwrite.
    const string badPath = (directory / "invalid.txt").string();
    { ofstream bad(badPath); bad << "RES_1,S001,TEST,not-a-time,123,Confirmed\n"; }
    assert(!cancelled.loadReservations(badPath));
    assert(cancelled.getReservations().size() == 1);
    { ofstream bad(badPath); bad << "RES_4,S001,UNKNOWN,1800000000,1800003600,Confirmed\n"; }
    assert(!cancelled.loadReservations(badPath));
    assert(cancelled.getReservations().size() == 1);
    filesystem::remove_all(directory);
    cout << "Reservation persistence tests passed!\n";
}
