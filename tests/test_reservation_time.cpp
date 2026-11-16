#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "ReservationManagementSystem.h"

using namespace std;
using Clock = chrono::system_clock;

int main() {
    ReservationManagementSystem system;
    system.addRoom(Room("R101", RoomType::CLASSROOM, 30, "Library", true));
    const auto now = Clock::now();

    // Entirely in the past.
    assert(!system.createReservation("R101", "S001", now - chrono::hours(2), now - chrono::hours(1)));
    // Boundary: started one second ago.
    assert(!system.createReservation("R101", "S001", now - chrono::seconds(1), now + chrono::hours(1)));
    // Already in progress (started in the past, ends in the future).
    assert(!system.createReservation("R101", "S001", now - chrono::minutes(30), now + chrono::minutes(30)));
    assert(system.getReservations().empty());

    // A future booking is accepted, and rejected requests did not use up an ID.
    const auto start = now + chrono::hours(1);
    assert(system.createReservation("R101", "S001", start, start + chrono::hours(1)));
    assert(system.getReservations().count("RES_1") == 1);

    // Past reservations already saved on disk still load (history is kept)
    // and do not block future bookings.
    const auto path = filesystem::temp_directory_path() /
        ("cs410-past-" + to_string(Clock::now().time_since_epoch().count()) + ".txt");
    { ofstream file(path); file << "RES_7,S001,R101,1000000000,1000003600,CheckedOut\n"; }
    ReservationManagementSystem loaded;
    loaded.addRoom(Room("R101", RoomType::CLASSROOM, 30, "Library", true));
    assert(loaded.loadReservations(path.string()));
    assert(loaded.getReservations().count("RES_7") == 1);
    assert(loaded.createReservation("R101", "S002", start, start + chrono::hours(1)));
    assert(loaded.getReservations().count("RES_8") == 1);
    filesystem::remove(path);

    cout << "Reservation time rules passed!\n";
}
