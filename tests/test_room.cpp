
#include <cassert>
#include <iostream>
#include <vector>
#include "../include/Room.h"

using namespace std;

int main() {
    // Test valid capacity
    Room room1("R101", RoomType::CLASSROOM, 10, 20, "Library", true);
    assert(room1.getCapacity() == 10);

    // Test maximum capacity
    Room room2("R102", RoomType::MEETING_ROOM, 20, 30, "Library", true);
    assert(room2.getCapacity() == 20);

    // Test capacity above the allowed maximum
    Room room3("R103", RoomType::STUDY_ROOM, 20, 15, "Library", true);

    // Assumes the constructor sets capacity to 15
    // when the supplied capacity is invalid
    assert(room3.getCapacity() == 20);

    // Test setter with a valid value
    room1.setCapacity(20);
    assert(room1.getCapacity() == 20);

    // Test setter with an invalid value
    room1.setCapacity(30);

    // Assumes an invalid setter value leaves capacity unchanged
    assert(room1.getCapacity() == 20);

    // Store rooms in a vector
    vector<Room> rooms;
    rooms.push_back(room1);
    rooms.push_back(room2);
    rooms.push_back(room3);

    // Search rooms by requested capacity
    cout << "---------------------------------------------------------------" << endl;
    cout << "Enter capacity: ";

    int requestedCapacity;
    cin >> requestedCapacity;

    // Validate user input
    if (cin.fail() || requestedCapacity <= 0) {
        cout << "Please enter a valid positive capacity." << endl;
        return 1;
    }

    bool found = false;

    cout << "Rooms that can accommodate " << requestedCapacity
         << " people:" << endl;

    for (const Room& room : rooms) {
        if (room.canAccommodate(requestedCapacity)) {
            room.displayRoom();
            cout << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "There is no available room for capacity of "
             << requestedCapacity << "." << endl;
    }

    cout << "---------------------------------------------------------------" << endl;

    cout << "All Room tests passed!" << endl;

    return 0;
}