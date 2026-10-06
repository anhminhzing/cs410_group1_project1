//
//  Reservation.cpp
//  
//
//  Created by Anh Minh Duong on 10/6/26.
//

#include <iostream>
#include "../include/Reservation.h"
using namespace std;

Reservation::Reservation(const string &ownerID, const Room &room, const string &reservationID, std::chrono::time_point<std::chrono::system_clock> startTime, std::chrono::time_point<std::chrono::system_clock> endTime) {
    this->ownerID = ownerID;
    this->room = room;
    this->reservationID = reservationID;
    this->startTime = startTime;
    this->endTime = endTime;
    this->status = ReservationStatus::Confirmed;
}

string Reservation::getOwnerID() const {
    return this->ownerID;
}

Room Reservation::getRoom() const {
    return this->room;
}

string Reservation::getReservationID() const {
    return this->reservationID;
}

std::chrono::time_point<std::chrono::system_clock> Reservation::getStartTime() const {
    return this->startTime;
}

std::chrono::time_point<std::chrono::system_clock> Reservation::getEndTime() const {
    return this->endTime;
}

ReservationStatus Reservation::getStatus() const {
    return this->status;
}

void Reservation::setStatus(ReservationStatus status) {
    this->status = status;
}

void Reservation::cancelReservation() {
    setStatus(ReservationStatus::Cancelled);
}

void Reservation::checkedInReservation() {
    setStatus(ReservationStatus::CheckedIn);
}

void Reservation::checkedOutReservation() {
    setStatus(ReservationStatus::CheckedOut);
}


