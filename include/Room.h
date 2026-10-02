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
    string roomID;
    int capacity;
    int maxcapacity;
    string location;
    bool available;

public:
    static const int MAX_CAPACITY = 20;
    
    // Constructor
    Room();

    Room(string roomID, int capacity,int maxcapacity ,string location, bool available);

    // Getters
    string getRoomID() const;
    int getCapacity() const;
    int getMaxcapacity() const;
    string getLocation() const;
    bool isAvailable() const;

    // Setters
    void setRoomID(string roomID);
    void setCapacity(int capacity);
    void setMaxcapacity(int maxcapacity);
    void setLocation(string location);
    void setAvailable(bool available);

    // Display room information
    void displayRoom() const;
};

#endif
