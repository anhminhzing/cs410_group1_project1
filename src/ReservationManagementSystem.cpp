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
    reservations.emplace(generatedID, newRes);
    roomReservations[roomID].push_back(generatedID);
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

bool ReservationManagementSystem::deleteReservation(const string& reservationID) {
    auto it = reservations.find(reservationID);
    if (it == reservations.end()) {
        cout << "Reservation: " << reservationID << " not found" << endl;
        return false;
    }

    string roomID = it->second.getRoomID();
    auto roomIt = roomReservations.find(roomID);
    if (roomIt != roomReservations.end()) {
        auto& resList = roomIt->second;
        for (auto vecIt = resList.begin(); vecIt != resList.end(); ++vecIt) {
            if (*vecIt == reservationID) {
                resList.erase(vecIt);
                break;
            }
        }

        if (resList.empty()) {
            roomReservations.erase(roomID);
        }
    }

    reservations.erase(it);
    cout << "Successfully deleted reservation: " << reservationID << endl;
    return true;
}

vector<Room> ReservationManagementSystem::getAvailableRooms(
        int requiredCapacity,
        chrono::system_clock::time_point reqStart,
        chrono::system_clock::time_point reqEnd
        ) {

    vector<Room> availableRooms;
    availableRooms.reserve(rooms.size());

    for (const auto& [roomID, room] : rooms) {
        if (room.getCapacity() < requiredCapacity) {
            continue;
        }

        auto roomIt = roomReservations.find(roomID);
        if (roomIt == roomReservations.end()) {
            bool hasConflict = false;

            for (const string& resID : roomIt->second) {
                auto resIt = reservations.find(resID);
                if (resIt != reservations.end()) {
                    const auto& reservation = resIt->second;
                    if (reservation.getStatus() != ReservationStatus::Cancelled && reservation.conflicts(reqStart, reqEnd)) {
                        hasConflict = true;
                        break;
                    }
                }
            }

            if (hasConflict) {
                continue;
            }
        }

        availableRooms.push_back(room);
    }

    return availableRooms;
}

