#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include "Room.h"
using namespace std;

class FileHandler {
public:
    static std::vector<Room> loadRooms(const string& filename);

    static bool saveRooms(
        const string& filename,
        const vector<Room>& rooms);
};

#endif
