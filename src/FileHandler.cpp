#include "../include/FileHandler.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

vector<Room> FileHandler::loadRooms(const string& filename) {
    vector<Room> rooms;

    ifstream file(filename);

    if (!file.is_open()) {
      cerr << "Error: could not open "
           << filename << std::endl;

        return rooms;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string roomID;
        string capacityText;
        string maxcapacityText;
        string location;
        string availableText;

        getline(ss, roomID, ',');
        getline(ss, capacityText, ',');
        getline(ss, maxcapacityText, ',');
        getline(ss, location, ',');
        getline(ss, availableText, ',');

        int capacity = stoi(capacityText);
        int maxcapacity = stoi(maxcapacityText);
        bool available = (availableText == "1");

        Room room(
            roomID,
            capacity,
            maxcapacity,
            location,
            available
        );

        rooms.push_back(room);
    }

    file.close();

    return rooms;
}

bool FileHandler::saveRooms(
    const string& filename,
    const vector<Room>& rooms
) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: could not open "
                  << filename << endl;

        return false;
    }

    for (const Room& room : rooms) {
        file << room.getRoomID() << ","
             << room.getCapacity() << ","
             << room.getMaxcapacity() << ","
             << room.getLocation() << ","
             << room.isAvailable()
             << endl;
    }

    file.close();

    return true;
}
