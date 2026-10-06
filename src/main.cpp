//
//  main.c
// Your First C++ Program

#include <iostream>
#include "../include/Room.h"

using namespace std;

int main() {
    Room room1("W-100-003", RoomType::CLASSROOM ,6, 20 ,"Campus Center", true);
    room1.displayRoom();
    return 0;
}
