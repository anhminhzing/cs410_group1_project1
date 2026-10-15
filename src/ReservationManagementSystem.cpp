//
// Created by ernie on 10/15/26.
//

#include "../include/ReservationManagementSystem.h"
#include <chrono>
#include <iostream>
#include <ostream>

#include "../include/Room.h"
#include "../include/Reservation.h"
using namespace std;

ReservationManagementSystem::ReservationManagementSystem() : nextReservationId(1) {
}

void ReservationManagementSystem::addRoom(const Room& room) {
    rooms[room.getRoomID()] = room;
}

bool ReservationManagementSystem::isRoomAvailable(const string &roomID, chrono::system_clock::time_point reqStart, chrono::system_clock::time_point
                                                  reqEnd) {
    auto it = roomReservations.find(roomID);
    if (it == roomReservations.end()) {
        return true;
    }

    for (const string& id : it->second) {
        if (auto resIt = reservations.find(id); resIt != reservations.end()) {
            if (resIt->second.getStatus() == ReservationStatus::Cancelled) {
                continue;
            }
            if (resIt->second.conflicts(reqStart, reqEnd)) {
                return false;
            }
        }
    }

    return true;
}

bool ReservationManagementSystem::createReservation(const string roomID, const string userID, chrono::system_clock::time_point startTime, chrono::system_clock::time_point endTime) {
    if (rooms.find(roomID) == rooms.end()) {
        cout << "Room: " << roomID << " does not exist" << endl;
        return false;
    }

    if (!isRoomAvailable(roomID, startTime, endTime)) {
        cout << "Room: " << roomID << " is unavailable at this time" << endl;
        return false;
    }
    // can change ID generator later
    string generatedID = "RES_" + to_string(nextReservationId++);

    Reservation newRes(userID, roomID, generatedID, startTime, endTime);
    reservations.insert(make_pair(roomID, newRes));
    cout << "Successfully created reservation with ID: " << generatedID << endl;
    return true;
}

bool ReservationManagementSystem::cancelReservation(const string& reservationID) {
    auto it = reservations.find(reservationID);
    if (it == reservations.end()) {
        cout << "Reservation: " << reservationID << " not found" << endl;
        return false;
    }

    it->second.setStatus(ReservationStatus::Cancelled);
    cout << "Successfully cancelled reservation: " << reservationID << endl;
    return true;
}

