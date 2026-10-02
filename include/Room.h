//
//  Room.h
//  
//
//  Created by Anh Minh Duong on 10/2/26.
//
#ifndef ROOM_H
#define ROOM_H

#include <string>
using namespace std;

class Room {
private:
    int roomID;
    int capacity;
    int maxCapacity
    string location;
    bool available;

public:
    // Constructor
    Room();

    Room(int roomID, int capacity, string location, bool available);

    // Getters
    int getRoomID() const;
    int getCapacity() const;
    std::string getLocation() const;
    bool isAvailable() const;

    // Setters
    void setRoomID(int roomID);
    void setCapacity(int capacity);
    void setLocation(std::string location);
    void setAvailable(bool available);

    // Display room information
    void displayRoom() const;
};

#endif
