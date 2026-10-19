//
// Created by ernie on 10/13/26.
//

#ifndef CS410_GROUP1_RESERVATIONMANAGEMENTSYSTEM_H
#define CS410_GROUP1_RESERVATIONMANAGEMENTSYSTEM_H
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>

#include "Reservation.h"
#include "User.h"
#include "Room.h"
using namespace std;

class ReservationManagementSystem {
private:
    unordered_map<string, Room> rooms;
    unordered_map<string, Reservation> reservations;

    unordered_map<string, vector<string>> roomReservations;

    size_t nextReservationId;

public:
    ReservationManagementSystem();

    bool isRoomAvailable(const string &roomID, chrono::system_clock::time_point reqStart, chrono::system_clock::time_point
                         reqEnd);
    bool createReservation(const string roomID, const string userID, chrono::system_clock::time_point startTime, chrono::system_clock::time_point endTime);
    bool cancelReservation(const string& reservationID);

    void addRoom(const Room& room);

    vector<Room> get AvailableRooms(
        int requiredCapacity,
        chrono::system_clock::time_point reqStart,
        chrono::system_clock::time_point reqEnd
    );

};
#endif //CS410_GROUP1_RESERVATIONMANAGEMENTSYSTEM_H
