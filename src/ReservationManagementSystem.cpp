//
// Created by ernie on 10/15/26.
//

#include "../include/ReservationManagementSystem.h"
#include <chrono>
#include <iostream>
#include <ostream>
#include <filesystem>
#include <algorithm>
#include <limits>
#include "FileHandler.h"
#include <ctime>
#include <sstream>

#include "../include/Room.h"
#include "../include/Reservation.h"
using namespace std;

ReservationManagementSystem::ReservationManagementSystem() : nextReservationId(1) {
}

bool ReservationManagementSystem::saveChanges(
    const unordered_map<string, Reservation>& updated) const {
    if (reservationFile.empty() || FileHandler::saveReservations(reservationFile, updated)) {
        return true;
    }
    cout << "Could not save reservations. No changes were applied." << endl;
    return false;
}

bool ReservationManagementSystem::loadReservations(const string& filename) {
    unordered_map<string, Reservation> loaded;
    error_code error;
    const bool exists = filesystem::exists(filename, error);
    if (error) return false;
    if (exists) {
        if (!FileHandler::loadReservations(filename, loaded)) return false;
    } else if (!FileHandler::saveReservations(filename, loaded)) {
        return false;
    }
    unordered_map<string, vector<string>> indexes;
    size_t nextID = 1;
    for (const auto& entry : loaded) {
        const Reservation& reservation = entry.second;
        if (rooms.find(reservation.getRoomID()) == rooms.end()) return false;
        // Validate generated IDs and continue beyond the largest saved number.
        const string& id = entry.first;
        if (id.size() <= 4 || id.substr(0, 4) != "RES_" ||
            id.find_first_not_of("0123456789", 4) != string::npos) return false;
        try {
            const auto number = stoull(id.substr(4));
            if (number == 0 || number >= numeric_limits<size_t>::max()) return false;
            nextID = max(nextID, static_cast<size_t>(number) + 1);
        } catch (const exception&) {
            return false;
        }
        indexes[reservation.getRoomID()].push_back(id);
    }
    reservations.swap(loaded);
    roomReservations.swap(indexes);
    nextReservationId = nextID;
    reservationFile = filename;
    return true;
}

void ReservationManagementSystem::addRoom(const Room& room) {
    rooms[room.getRoomID()] = room;
}

bool ReservationManagementSystem::removeRoom(const string& roomID) {
    auto roomIt = rooms.find(roomID);

    if (roomIt == rooms.end()) {
        return false;
    }
    // Do not delete a room if any reservation still references it.
    for (const auto& entry : reservations) {
        if (entry.second.getRoomID() == roomID) {
            return false;
        }
    }
    rooms.erase(roomIt);
    roomReservations.erase(roomID);

    return true;
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

    // Business rule: a reservation cannot start in the past.
    if (startTime <= chrono::system_clock::now()) {
        cout << "Reservations must start in the future." << endl;
        return false;
    }

    if (!isRoomAvailable(roomID, startTime, endTime)) {
        cout << "Room: " << roomID << " is unavailable at this time" << endl;
        return false;
    }
    if (endTime <= startTime || !rooms.at(roomID).isAvailable() || userID.empty() ||
        nextReservationId == numeric_limits<size_t>::max()) return false;
    string generatedID = "RES_" + to_string(nextReservationId);

    Reservation newRes(userID, roomID, generatedID, startTime, endTime);
    auto updated = reservations;
    updated.emplace(generatedID, newRes);
    if (!saveChanges(updated)) return false;
    reservations.swap(updated);
    ++nextReservationId;
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

    auto updated = reservations;
    updated.at(reservationID).setStatus(ReservationStatus::Cancelled);
    if (!saveChanges(updated)) return false;
    reservations.swap(updated);
    cout << "Successfully cancelled reservation: " << reservationID << endl;
    return true;
}

bool ReservationManagementSystem::deleteReservation(const string& reservationID) {
    auto it = reservations.find(reservationID);
    if (it == reservations.end()) {
        cout << "Reservation: " << reservationID << " not found" << endl;
        return false;
    }

    const string roomID = it->second.getRoomID();
    auto updated = reservations;
    updated.erase(reservationID);
    if (!saveChanges(updated)) return false;
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

    reservations.swap(updated);
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
        if (room.isAvailable() && room.canAccommodate(requiredCapacity) &&
            isRoomAvailable(roomID, reqStart, reqEnd)) {
            availableRooms.push_back(room);
        }
    }

    return availableRooms;
}

string ReservationManagementSystem::timeFormat(std::chrono::system_clock::time_point tp) const {
    time_t time = chrono::system_clock::to_time_t(tp);
    tm tmStruct{};
    #ifdef _WIN32
    localtime_s(&tmStruct, &time);
    #else
    localtime_r(&time, &tmStruct);
    #endif

    ostringstream oss;
    oss << put_time(&tmStruct, "%Y-%m-%d %H:%M");
    return oss.str();

}

void ReservationManagementSystem::viewReservation(const string &resID) const {
    auto it = reservations.find(resID);
    if (it == reservations.end()) {
        cout << "Reservation '" << resID << "' not found." << endl;
        return;
    }

    const Reservation& res = it->second;
    string statusStr = (res.getStatus() == ReservationStatus::Cancelled) ? "Cancelled" : "Active";

    cout << "\n========================================\n"
         << " Reservation ID : " << res.getReservationID() << "\n"
         << " Room ID        : " << res.getRoomID() << "\n"
         << " User ID        : " << res.getOwnerID() << "\n"
         << " Start Time     : " << timeFormat(res.getStartTime()) << "\n"
         << " End Time       : " << timeFormat(res.getEndTime()) << "\n"
         << " Status         : " << statusStr << "\n"
         << "========================================\n" << endl;
}

void ReservationManagementSystem::viewAllReservations() const {
    if (reservations.empty()) {
        cout << "\nNo reservations found in the system.\n" << endl;
        return;
    }

    cout << "\n" << left
         << setw(12) << "Res ID"
         << setw(10) << "Room ID"
         << setw(12) << "User ID"
         << setw(18) << "Start Time"
         << setw(18) << "End Time"
         << setw(10) << "Status" << "\n"
         << string(80, '-') << "\n";

    for (const auto& [id, res] : reservations) {
        string statusStr = (res.getStatus() == ReservationStatus::Cancelled) ? "Cancelled" : "Active";

        cout << left
             << setw(12) << id
             << setw(10) << res.getRoomID()
             << setw(12) << res.getOwnerID()
             << setw(18) << timeFormat(res.getStartTime())
             << setw(18) << timeFormat(res.getEndTime())
             << setw(10) << statusStr << "\n";
    }
    cout << string(80, '-') << "\n" << endl;
}
