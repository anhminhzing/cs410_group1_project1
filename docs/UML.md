# UML Class Diagram

Generated from the headers in `include/` and the data files in `data/`.

```mermaid
classDiagram
    direction TB

    %% ===== Users =====
    class User {
        <<abstract>>
        #string userID
        #string name
        #string email
        +User(userID, name, email)
        +~User()
        +getUserID() string
        +getName() string
        +getEmail() string
        +setUserID(userID) void
        +setName(name) void
        +setEmail(email) void
        +getUserType()* string
    }

    class Student {
        -string major
        +Student(userID, name, email, major)
        +getMajor() string
        +getUserType() string
        +displayStd() void
    }

    class Staff {
        -string department
        +Staff(userID, name, email, department)
        +getDepartment() string
        +getUserType() string
        +displayStaff() void
    }

    class Admin {
        -string office
        +Admin(userID, name, email, office)
        +getOffice() string
        +getUserType() string
        +displayAdmin() void
        +deactivateAccount() void
    }

    User <|-- Student
    User <|-- Staff
    User <|-- Admin

    %% ===== Rooms =====
    class RoomType {
        <<enumeration>>
        CLASSROOM
        MEETING_ROOM
        STUDY_ROOM
    }

    class Room {
        -string roomID
        -RoomType type
        -int maxCapacity
        -string location
        -bool available
        +MAX_CAPACITY$ int = 20
        +Room()
        +Room(roomID, type, maxCapacity, location, available)
        +getRoomID() string
        +getType() RoomType
        +getMaxCapacity() int
        +getLocation() string
        +isAvailable() bool
        +setRoomID(roomID) void
        +setType(type) void
        +setMaxCapacity(maxCapacity) void
        +setLocation(location) void
        +setAvailable(available) void
        +getTypeString() string
        +displayRoom() void
        +canAccommodate(requestedCapacity) bool
    }

    Room --> RoomType : type

    %% ===== Reservations =====
    class ReservationStatus {
        <<enumeration>>
        Confirmed
        CheckedIn
        CheckedOut
        Cancelled
    }

    class Reservation {
        -string ownerID
        -string roomID
        -string reservationID
        -time_point startTime
        -time_point endTime
        -ReservationStatus status
        +Reservation(ownerID, roomID, reservationID, startTime, endTime)
        +getOwnerID() string
        +getRoomID() string
        +getReservationID() string
        +getStartTime() time_point
        +getEndTime() time_point
        +getStatus() ReservationStatus
        +setStatus(status) void
        +cancelReservation() void
        +checkedInReservation() void
        +checkedOutReservation() void
        +conflicts(reqStart, reqEnd) bool
    }

    Reservation --> ReservationStatus : status
    Reservation "0..*" --> "1" User : ownerID
    Reservation "0..*" --> "1" Room : roomID

    %% ===== Managers =====
    class ReservationManagementSystem {
        -unordered_map~string, Room~ rooms
        -unordered_map~string, Reservation~ reservations
        -unordered_map~string, vector~string~~ roomReservations
        -size_t nextReservationId
        -string reservationFile
        -timeFormat(tp) string
        -saveChanges(updated) bool
        +ReservationManagementSystem()
        +loadReservations(filename) bool
        +isRoomAvailable(roomID, reqStart, reqEnd) bool
        +createReservation(roomID, userID, startTime, endTime) bool
        +cancelReservation(reservationID) bool
        +deleteReservation(reservationID) bool
        +getReservations() unordered_map
        +addRoom(room) void
        +removeRoom(roomID) bool
        +getAvailableRooms(requiredCapacity, reqStart, reqEnd) vector~Room~
        +viewReservation(resID) void
        +viewAllReservations() void
    }

    class UserManagementSystem {
        -unordered_map~string, User*~ users
        +UserManagementSystem()
        +~UserManagementSystem()
        +getUser(userID) User*
        +getUsers() unordered_map
        +addUser(user) bool
        +deleteUser(userID) bool
        +userExists(userID) bool
        +isAdmin(userID) bool
    }

    ReservationManagementSystem "1" *-- "0..*" Room : rooms
    ReservationManagementSystem "1" *-- "0..*" Reservation : reservations
    UserManagementSystem "1" o-- "0..*" User : users (owns pointers)

    %% ===== Persistence =====
    class FileHandler {
        <<utility>>
        +loadReservations(filename, reservations)$ bool
        +saveReservations(filename, reservations)$ bool
        +loadRooms(filename)$ vector~Room~
        +saveRooms(filename, rooms)$ bool
        +loadUsers(filename, users)$ bool
        +saveUsers(filename, users)$ bool
    }

    ReservationManagementSystem ..> FileHandler : uses
    FileHandler ..> Reservation : reads/writes
    FileHandler ..> Room : reads/writes
    FileHandler ..> User : creates Student/Staff/Admin
```

## Data files

| File | Record format | Maps to |
|---|---|---|
| `data/reservations.txt` | `reservationID,ownerID,roomID,startEpoch,endEpoch,status` | `Reservation` |
| `data/rooms.txt` | `roomID,type,maxCapacity,location,available(0/1)` | `Room` |
| `data/users.txt` | `userType,userID,name,email,extraInfo` (major / department / office) | `Student` / `Staff` / `Admin` |
