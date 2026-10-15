//
//  Reservation.h
//  
//
//  Created by Anh Minh Duong on 10/6/26.
//

#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <chrono>

#include "Room.h"
using namespace std;

enum class ReservationStatus {
    Confirmed,
    CheckedIn,
    CheckedOut,
    Cancelled,
};

class Reservation {
private:
    string ownerID;
    string roomID;
    string reservationID;
    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> endTime;
    ReservationStatus status;

    

public:
    Reservation(const string &ownerID, const string &roomID, const string &reservationID, std::chrono::time_point<std::chrono::system_clock> startTime, std::chrono::time_point<std::chrono::system_clock> endTime);

    // get
    string getOwnerID() const;
    string getRoomID() const;
    string getReservationID() const;
    std::chrono::time_point<std::chrono::system_clock> getStartTime() const;
    std::chrono::time_point<std::chrono::system_clock> getEndTime() const;
    ReservationStatus getStatus() const;
    // set
    void setStatus(ReservationStatus status);
    // cancel
    void cancelReservation();
    // checkin
    void checkedInReservation();
    void checkedOutReservation();

    bool conflicts(chrono::system_clock::time_point reqStart, chrono::system_clock::time_point reqEnd) const;

    
};

#endif
