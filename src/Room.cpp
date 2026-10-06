#include <iostream>
#include "../include/Room.h"
using namespace std;


//Room(int roomID, int capacity,int maxCapacity ,string location, bool available);
// Default constructor
Room::Room() {
    roomID = "";
    capacity = 0;
    type = RoomType::CLASSROOM;
    maxcapacity = MAX_CAPACITY;
    location = "";
    available = true;
    
}


// Constructor with parameters
Room::Room(string roomID, RoomType type ,int capacity,int maxcapacity ,string location, bool available) {
    this->roomID = roomID;
    this->type = type;
    this->capacity = capacity;
    this->maxcapacity = maxcapacity;
    this->location = location;
    this->available = available;
}

// Getters
string Room::getRoomID() const {
    return roomID;
}

RoomType Room::getType() const {
    return type;
}

string Room::getTypeString() const {
    switch (type) {
        case RoomType::CLASSROOM:
            return "Classroom";

        case RoomType::MEETING_ROOM:
            return "Meeting room";

        case RoomType::STUDY_ROOM:
            return "Study room";

    }

    return "Unknown";
}

int Room::getCapacity() const {
    return capacity;
}

int Room::getMaxcapacity() const {
    return maxcapacity;
}

string Room::getLocation() const {
    return location;
}

bool Room::isAvailable() const {
    return available;
}

// Setters
void Room::setRoomID(string roomID) {
    this->roomID = roomID;
}

void Room::setCapacity(int capacity) {
    if (capacity < 1 || capacity >  this->maxcapacity){
        cout << "capacity is unvalid, please double check" << endl;
    } else{
        this->capacity = capacity;
    }
}
void Room::setMaxcapacity(int maxcapacity) {
    this->maxcapacity = maxcapacity;

}

void Room::setLocation(std::string location) {
    this->location = location;
}

void Room::setAvailable(bool available) {
    this->available = available;
}

// Display room information
void Room::displayRoom() const {
    cout << "Room ID: " << roomID << endl;
    cout << "type: " << getTypeString() << endl;
    cout << "Capacity: " << capacity << endl;
    cout << "Max capacity: " << maxcapacity << endl;
    cout << "Location: " << location << endl;
    cout << "Available: "
         << (available ? "Yes" : "No") << endl;
}

bool Room::canAccommodate(int requestedCapacity) const {
    return  1 <= requestedCapacity && requestedCapacity <= maxcapacity;
}