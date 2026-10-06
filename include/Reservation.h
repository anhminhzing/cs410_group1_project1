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
    Room room;
    string reservationID;
    std::chrono::time_point<std::chrono::system_clock> startTime;
    std::chrono::time_point<std::chrono::system_clock> endTime;
    ReservationStatus status;

    

public:
    Reservation(const string &ownerID, const Room &room, const string &reservationID, std::chrono::time_point<std::chrono::system_clock> startTime, std::chrono::time_point<std::chrono::system_clock> endTime);

    // get
    string getOwnerID() const;
    Room getRoom() const;
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

    
};

#endif
