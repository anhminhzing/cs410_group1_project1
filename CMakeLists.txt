cmake_minimum_required(VERSION 3.20)

project(CampusRoomReservation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Core project code
add_library(campus_core
    src/Admin.cpp
    src/FileHandler.cpp
    src/Reservation.cpp
    src/ReservationManagementSystem.cpp
    src/Room.cpp
    src/Staff.cpp
    src/Student.cpp
    src/User.cpp
    src/UserManagementSystem.cpp
)

target_include_directories(campus_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# Main application
add_executable(campus_reservation
    src/main.cpp
)

target_link_libraries(campus_reservation
    PRIVATE campus_core
)

#[[
# Automated testing
enable_testing()

add_executable(test_room
    tests/test_room.cpp
)

target_link_libraries(test_room
    PRIVATE campus_core
)

add_test(
    NAME RoomTest
    COMMAND test_room
)
]]

# ==========================================
# Automated testing
# ==========================================

enable_testing()

add_executable(sprint0_smoke_test
    tests/test_smoke.cpp
)

add_test(
    NAME Sprint0SmokeTest
    COMMAND sprint0_smoke_test
)