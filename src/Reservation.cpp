//
//  Reservation.cpp
//  
//
//  Created by Anh Minh Duong on 10/6/26.
//

#include <iostream>
#include "../include/Reservation.h"
using namespace std;

Reservation::Reservation(const string &ownerID, const string &roomID, const string &reservationID, std::chrono::time_point<std::chrono::system_clock> startTime, std::chrono::time_point<std::chrono::system_clock> endTime) {
    this->ownerID = ownerID;
    this->roomID = roomID;
    this->reservationID = reservationID;
    this->startTime = startTime;
    this->endTime = endTime;
    this->status = ReservationStatus::Confirmed;
}

string Reservation::getOwnerID() const {
    return this->ownerID;
}

string Reservation::getRoomID() const {
    return this->roomID;
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

bool Reservation::conflicts(const chrono::system_clock::time_point reqStart, const chrono::system_clock::time_point reqEnd) const {
    return (this->startTime < reqStart && this->endTime < reqEnd);
}


