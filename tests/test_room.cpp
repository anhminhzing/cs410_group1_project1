#include <cassert>
#include <chrono>
#include <iostream>
#include "Room.h"
#include "ReservationManagementSystem.h"

using namespace std;

int main() {
    Room room("R101", RoomType::CLASSROOM, 30, "Library", true);

    assert(room.getRoomID() == "R101");
    assert(room.getType() == RoomType::CLASSROOM);
    assert(room.getMaxCapacity() == 30);
    assert(room.getLocation() == "Library");
    assert(room.isAvailable());

    // ------------------------------------------------------------
    // Room capacity boundaries
    // ------------------------------------------------------------
    assert(room.canAccommodate(1));
    assert(room.canAccommodate(30));   // Exact maximum
    assert(!room.canAccommodate(31));  // Above maximum
    assert(!room.canAccommodate(0));   // Invalid: zero attendees
    assert(!room.canAccommodate(-1));  // Invalid: negative attendees

    // Increase the room's maximum capacity.
    room.setMaxCapacity(40);
    assert(room.getMaxCapacity() == 40);
    assert(room.canAccommodate(40));
    assert(!room.canAccommodate(41));

    // Decrease the room's maximum capacity.
    room.setMaxCapacity(20);
    assert(room.getMaxCapacity() == 20);
    assert(room.canAccommodate(20));
    assert(!room.canAccommodate(21));

    // Restore capacity for the reservation-management tests below.
    room.setMaxCapacity(40);
    assert(room.getMaxCapacity() == 40);

    // ------------------------------------------------------------
    // Default room construction
    // ------------------------------------------------------------
    Room defaultRoom;
    assert(defaultRoom.getMaxCapacity() == Room::MAX_CAPACITY);
    assert(defaultRoom.getRoomID().empty());
    assert(defaultRoom.getLocation().empty());
    assert(defaultRoom.isAvailable());

    // ------------------------------------------------------------
    // Adding rooms and searching for available rooms
    // ------------------------------------------------------------
    ReservationManagementSystem system;
    system.addRoom(room);
    system.addRoom(Room("R102", RoomType::STUDY_ROOM, 10, "Campus Center", true));
    system.addRoom(Room("R103", RoomType::MEETING_ROOM, 50, "Science Building", false));
    // Relative to now so the test keeps working as real time passes.
    const chrono::system_clock::time_point start =
        chrono::time_point_cast<chrono::seconds>(chrono::system_clock::now()) + chrono::hours(24);
    const auto end = start + chrono::hours(1);

    // A 40-person search should return only R101.
    auto available = system.getAvailableRooms(40, start, end);
    assert(available.size() == 1);
    assert(available.front().getRoomID() == "R101");

    // R103 has enough capacity but is unavailable, so it must not be returned.
    assert(system.getAvailableRooms(50, start, end).empty());

    // Both available rooms can accommodate 10 attendees.
    available = system.getAvailableRooms(10, start, end);
    assert(available.size() == 2);

    // Invalid attendee counts should return no rooms.
    assert(system.getAvailableRooms(0, start, end).empty());
    assert(system.getAvailableRooms(-1, start, end).empty());

    // No room can accommodate more than R103's capacity.
    assert(system.getAvailableRooms(51, start, end).empty());

    // ------------------------------------------------------------
    // 5. Reservation creation and conflict handling
    // ------------------------------------------------------------

    // A valid reservation should be created.
    assert(system.createReservation("R101", "S001", start, end));

    // An overlapping reservation for the same room should be rejected.
    const auto overlapStart = start + chrono::minutes(30);
    const auto overlapEnd = end + chrono::minutes(30);
    assert(!system.createReservation(
        "R101", "S002", overlapStart, overlapEnd));

    // A reservation for a nonexistent room should be rejected.
    assert(!system.createReservation("R999", "S002", start, end));

    // Invalid time ranges should be rejected.
    assert(!system.createReservation(
        "R101", "S002", end, start));  // End is before start
    assert(!system.createReservation(
        "R101", "S002", start, start));  // Start and end are equal

    // The existing reservation should make R101 unavailable for that period.
    assert(system.getAvailableRooms(40, start, end).empty());

    // A reservation starting exactly when the previous one ends is allowed.
    assert(system.getAvailableRooms(
        40, end, end + chrono::hours(1)).size() == 1);

    // ------------------------------------------------------------
    // 6. Reservation cancellation
    // ------------------------------------------------------------
    assert(system.cancelReservation("RES_1"));

    // After cancellation, R101 should be available for the original period.
    assert(system.getAvailableRooms(40, start, end).size() == 1);

    cout << "------------------ALL ROOM AND RESERVATION MANAGEMENT TESTS PASSED!------------------\n" << endl;
}