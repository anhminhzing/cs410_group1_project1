#include <iostream>
#include "Room.h"
using namespace std;


// Default constructor
Room::Room() {
    roomID = 0;
    capacity = 0;
    location = "";
    available = true;
}

// Constructor with parameters
Room::Room(int roomID, int capacity, string location, bool available) {
    this->roomID = roomID;
    this->capacity = capacity;
    this->location = location;
    this->available = available;
}

// Getters
int Room::getRoomID() const {
    return roomID;
}

int Room::getCapacity() const {
    return capacity;
}

std::string Room::getLocation() const {
    return location;
}

bool Room::isAvailable() const {
    return available;
}

// Setters
void Room::setRoomID(int roomID) {
    this->roomID = roomID;
}

void Room::setCapacity(int capacity) {
    this->capacity = capacity;
}

void Room::setLocation(std::string location) {
    this->location = location;
}

void Room::setAvailable(bool available) {
    this->available = available;
}

// Display room information
void Room::displayRoom() const {
    cout << "Room ID: " << roomID << std::endl;
    cout << "Capacity: " << capacity << std::endl;
    cout << "Location: " << location << std::endl;
    cout << "Available: "
         << (available ? "Yes" : "No") << std::endl;
}
