#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include <unordered_map>

#include "Room.h"
#include "User.h"
#include "Student.h"
#include "Staff.h"
#include "Admin.h"


using namespace std;

class FileHandler {
public:
    static std::vector<Room> loadRooms(const string& filename);

    static bool saveRooms(
        const string& filename,
        const vector<Room>& rooms);
    
    // User file functions
    static bool loadUsers(
        const string& filename,
        unordered_map<std::string, User*>& users
    );

    static bool saveUsers(
        const string& filename,
        const unordered_map<std::string, User*>& users
    );
};

#endif
