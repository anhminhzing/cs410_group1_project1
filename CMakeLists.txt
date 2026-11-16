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

#Smoke test
enable_testing()

add_executable(sprint0_smoke_test
    tests/test_smoke.cpp

)

add_test(
    NAME Sprint0SmokeTest
    COMMAND sprint0_smoke_test
)

# Admin test
add_executable(test_admin
    tests/test_admin.cpp
)

target_link_libraries(test_admin
    PRIVATE campus_core
)

add_test(
    NAME AdminTest
    COMMAND test_admin
)

# Student test
add_executable(test_student
    tests/test_student.cpp
)

target_link_libraries(test_student
    PRIVATE campus_core
)

add_test(
    NAME StudentTest
    COMMAND test_student
)

# Staff test
add_executable(test_staff
    tests/test_staff.cpp
)

target_link_libraries(test_staff
    PRIVATE campus_core
)

add_test(
    NAME StaffTest
    COMMAND test_staff
)

# File Handler test
add_executable(test_filehandler
    tests/test_filehandler.cpp
)

target_link_libraries(test_filehandler
    PRIVATE campus_core
)

add_test(
    NAME FileHandlerTest
    COMMAND test_filehandler
)

# Reservation Persistence test
add_executable(test_reservation_persistence
    tests/test_reservation_persistence.cpp
)

target_link_libraries(test_reservation_persistence
    PRIVATE campus_core
)

add_test(
    NAME ReservationPersistenceTest
    COMMAND test_reservation_persistence
)

# Reservation time rules test
add_executable(test_reservation_time
    tests/test_reservation_time.cpp
)

target_link_libraries(test_reservation_time
    PRIVATE campus_core
)

add_test(
    NAME ReservationTimeTest
    COMMAND test_reservation_time
)
