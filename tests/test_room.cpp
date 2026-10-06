
#include <cassert>
#include <iostream>
#include "../include/Room.h"


using namespace std;

int main() {
    // Valid capacity
    Room room1("R101", RoomType::CLASSROOM, ,10, 20 ,"Library", true);
    assert(room1.getCapacity() == 10);

    // Maximum capacity
    Room room2("R102", RoomType::MEETING_ROOM, ,20, 30 ,"Library", true);
    assert(room2.getCapacity() == 20);

    // Over maximum capacity
    Room room3("R103", RoomType::STUDY_ROOM ,20, 15 ,"Library", true);

    // If your constructor changes invalid capacity to MAX_CAPACITY:
    assert(room3.getCapacity() == 15);

    // Test setter with valid value
    room1.setCapacity(20);
    assert(room1.getCapacity() == 20);

    // Test setter with invalid value
    room1.setCapacity(30);

    // If invalid setter keeps the old value:
    assert(room1.getCapacity() == 20);

    cout << "All Room tests passed!" << endl;




    return 0;
}
