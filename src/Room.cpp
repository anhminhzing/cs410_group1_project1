#include <iostream>
#include "../include/Room.h"
using namespace std;


// Default constructor
Room::Room() {
    roomID = "";
    type = RoomType::CLASSROOM;
    maxCapacity = MAX_CAPACITY;
    location = "";
    available = true;
    
}


// Constructor with parameters
Room::Room(string roomID, RoomType type ,int maxCapacity ,string location, bool available) {
    this->roomID = roomID;
    this->type = type;
    this->maxCapacity = maxCapacity;
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

int Room::getMaxCapacity() const {
    return maxCapacity;
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

void Room::setMaxCapacity(int maxCapacity) {
    this->maxCapacity = maxCapacity;

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
    cout << "Max capacity: " << maxCapacity << endl;
    cout << "Location: " << location << endl;
    cout << "Available: "
         << (available ? "Yes" : "No") << endl;
}

bool Room::canAccommodate(int requestedCapacity) const {
    return  1 <= requestedCapacity && requestedCapacity <= maxCapacity;
}