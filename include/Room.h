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

enum class RoomType {
    CLASSROOM,
    MEETING_ROOM,
    STUDY_ROOM,
};

class Room {
private:
    string roomID;
    RoomType type;
    int capacity;
    int maxcapacity;
    string location;
    bool available;

public:
    static const int MAX_CAPACITY = 20;
    
    // Constructor
    Room();

    Room(string roomID, RoomType type,
         int capacity,int maxcapacity ,string location, bool available);

    // Getters
    string getRoomID() const;
    RoomType getType() const;
    int getCapacity() const;
    int getMaxcapacity() const;
    string getLocation() const;
    bool isAvailable() const;

    // Setters
    void setRoomID(string roomID);
    void setType(RoomType type);
    void setCapacity(int capacity);
    void setMaxcapacity(int maxcapacity);
    void setLocation(string location);
    void setAvailable(bool available);

    // Display room information
    string getTypeString() const;
    void displayRoom() const;

    // Room capacity
    bool canAccommodate(int requestedCapacity) const;
};

#endif
