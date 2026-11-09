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

    // Attendee boundaries use the single room-size property.
    assert(room.canAccommodate(1));
    assert(room.canAccommodate(30));
    assert(!room.canAccommodate(31));
    assert(!room.canAccommodate(0));
    assert(!room.canAccommodate(-1));
    room.setMaxCapacity(40);
    assert(room.getMaxCapacity() == 40);
    assert(room.canAccommodate(40));
    assert(!room.canAccommodate(41));

    Room defaultRoom;
    assert(defaultRoom.getMaxCapacity() == Room::MAX_CAPACITY);

    // Search must apply the same maximum, availability, and conflict rules.
    ReservationManagementSystem system;
    system.addRoom(room);
    system.addRoom(Room("R102", RoomType::STUDY_ROOM, 10, "Campus Center", true));
    system.addRoom(Room("R103", RoomType::MEETING_ROOM, 50, "Science Building", false));
    const auto start = chrono::system_clock::time_point(chrono::seconds(1800000000));
    const auto end = start + chrono::hours(1);
    auto available = system.getAvailableRooms(40, start, end);
    assert(available.size() == 1);
    assert(available.front().getRoomID() == "R101");
    assert(system.getAvailableRooms(41, start, end).empty());
    assert(system.getAvailableRooms(0, start, end).empty());
    assert(system.getAvailableRooms(-1, start, end).empty());
    assert(system.createReservation("R101", "S001", start, end));
    assert(system.getAvailableRooms(40, start, end).empty());
    assert(system.getAvailableRooms(40, end, end + chrono::hours(1)).size() == 1);
    assert(system.cancelReservation("RES_1"));
    assert(system.getAvailableRooms(40, start, end).size() == 1);

    cout << "All Room tests passed!\n";
}
