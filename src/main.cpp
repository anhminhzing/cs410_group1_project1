//
//  main.c
// Your First C++ Program

#include <iostream>
#include "../include/Room.h"
#include <vector>

using namespace std;
    vector<Room> rooms;

int main() {
    Room room1("W-100-003", RoomType::CLASSROOM ,6, 20 ,"Campus Center", true);
    rooms.push_back(room1);
    Room room2 ("R102", RoomType::STUDY_ROOM, 20, 40, "Campus Center", true);
    rooms.push_back(room2);
    room1.displayRoom();
    Room room3("R103", RoomType::MEETING_ROOM, 15, 25, "Campus Center", true);
    rooms.push_back(room3);

    cout << "---------------------------------------------------------------" << endl;
    cout << "Enter capacity: " ;
    int requestedCapacity;
    cin >> requestedCapacity;

    for (const Room& room : rooms) {
        if (room.canAccommodate(requestedCapacity)) {
            room.displayRoom();
        }
    }
    bool found = false;
    for (const Room& room : rooms) {
        if (room.canAccommodate(requestedCapacity)) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "There is no available room for capacity of " << requestedCapacity << "." << endl;
    }
    cout << "---------------------------------------------------------------" << endl;
    return 0;

}
