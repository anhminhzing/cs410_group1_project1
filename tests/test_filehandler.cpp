#include <cassert>
#include <iostream>
#include <vector>

#include "Room.h"
#include "FileHandler.h"

int main() {
    std::vector<Room> rooms =
        FileHandler::loadRooms("data/rooms.txt");

    // Check number of rooms
    assert(rooms.size() == 3);

    // Check first room
    assert(rooms[0].getRoomID() == "R101");
    assert(rooms[0].getCapacity() == 10);
    assert(rooms[0].getMaxcapacity() == 30);
    assert(rooms[0].getLocation() == "Library");
    assert(rooms[0].isAvailable() == true);

    // Check second room
    assert(rooms[1].getRoomID() == "R102");
    assert(rooms[1].getCapacity() == 20);
    assert(rooms[1].getMaxcapacity() == 40);

    // Check third room
    assert(rooms[2].getRoomID() == "R103");
    assert(rooms[2].getCapacity() == 8);
    assert(rooms[2].getMaxcapacity() == 20);
    assert(rooms[2].isAvailable() == false);

    std::cout << "All FileHandler tests passed!" << std::endl;

    return 0;
}
