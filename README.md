# Campus Room Reservation System

CS410 Group 1 project: a C++17 console application for finding and reserving university rooms. Students, staff, and administrators log in using an existing user ID. All three roles can browse rooms, search availability, make reservations, and cancel permitted reservations. Administrators can also view, add, edit, and delete user accounts.

For a step-by-step explanation of startup, login, reservations, account management, and file saving, see the [Program Workflow Guide](docs/PROGRAM_WORKFLOW.md).

## Current features

| Feature | Current behavior |
| --- | --- |
| ID-based login | Displays available account IDs, names, and roles; retries unknown IDs and opens the appropriate role menu. |
| View rooms | Displays each room's ID, type, maximum capacity, location, and availability flag. |
| Search available rooms | Filters by date, start/end time, required attendees, room availability flag, and existing bookings. |
| Make a reservation | Books one of the displayed rooms for the logged-in user and prints a generated ID such as `RES_1`. |
| Prevent booking conflicts | Rejects overlapping bookings for the same room. Adjacent bookings are allowed. |
| Cancel a reservation | Lists permitted non-cancelled bookings and marks the selected booking as cancelled, freeing its time slot. |
| Manage users | Admins can inspect full account details, create accounts, replace account details/roles, and delete other accounts. |
| Save reservations | Loads `data/reservations.txt` at startup and saves creation, cancellation, and deletion immediately. |
| Delete reservations | Admin menu option **5** permanently removes a selected reservation from memory and disk. |
| Save account changes | Writes user changes to `data/users.txt`; failed saves leave the live accounts unchanged. |
| Switch accounts | Logging out returns to login without discarding reservations created during the current process. |

### Roles and permissions

| Function | Student | Staff | Admin |
| --- | --- | --- | --- |
| View rooms | Yes | Yes | Yes |
| Search and reserve rooms | Yes | Yes | Yes |
| Cancel reservations | Own bookings | Own bookings | Any booking |
| View full user details | No | No | Yes |
| Permanently delete reservations | No | No | Yes |
| Add, edit, and delete accounts | No | No | Yes |

The login screen shows a basic account list for demonstration. Login uses an ID only; passwords and university identity-provider authentication are not implemented.

## Build and run

### Prerequisites

- A C++17 compiler (`g++` or a compatible compiler).
- GNU Make.
- A terminal for the console interface.

Run commands from the repository root because data paths are relative to the working directory:


#### Makefile build and run


```sh
make all
make run
```

`make all` builds `build/main`; `make run` builds it if needed and starts it. You can also run `./build/main` from the repository root. To select another compiler, use `make CXX=clang++ all`.


#### CMake build and run

```sh
cmake -S . -B build-cmake
cmake --build build-cmake --parallel
.\build-cmake\campus_reservation.exe
```

`cmake --build build-cmake` builds `build-cmake/campus_reservation.exe`; you can run it using .`\build-cmake\campus_reservation.exe` from the repository root. To select another compiler, configure CMake with `-DCMAKE_CXX_COMPILER=clang++`

This project supports both Makefile and CMake builds. The GitHub Actions workflow in `.github/workflows/ci.yml` is used to build and test the project on pushes and pull requests, and supports manual runs. Check the workflow configuration and GitHub Actions tab to confirm that the latest revision has passed.

### Login and menus

Sample accounts in `data/users.txt`:

| ID | Name | Role |
| --- | --- | --- |
| `S001` | Anh Duong | Student |
| `S002` | John Smith | Student |
| `T001` | Sarah Lee | Staff |
| `T002` | Michael Brown | Staff |
| `A001` | Alice Johnson | Admin |

Account changes may alter this list. Use an ID displayed by the application.

The main role menu provides:

```text
1. View rooms
2. Search available rooms and make a reservation
3. Cancel my reservation / Cancel any reservation
4. Manage user accounts  (Admin only)
5. Delete reservation permanently  (Admin only)
0. Log out
```

Enter `0` at login to exit. In the cancellation or room-selection prompt, `0` returns without making a change.

## Reservation workflow

### Search and make a reservation

1. Log in using an existing ID, such as `S001`.
2. Select **1. View rooms** to inspect room information, or select **2** to search and book.
3. Enter the date as `MM/DD/YYYY` and start/end times as 24-hour `HH:MM`.
4. Enter the required number of attendees as a positive whole number.
5. Review the available rooms and enter one of their IDs.
6. Record the generated reservation ID shown in the success message.

The console rejects invalid dates/times, end times that are not after start times, nonpositive or nonnumeric attendee counts, and IDs outside the available results. A booking uses one date for both start and end; overnight bookings cannot be entered through this menu.

The capacity filter uses `Room::canAccommodate`, which compares the requested attendee count with the room's single size property, **`maxCapacity`**. `getMaxCapacity()` reads this limit; `setMaxCapacity()` updates it.

### Conflict detection

Two reservations for the same room overlap when:

```text
existing.start < requested.end AND requested.start < existing.end
```

An existing booking from `10:00` to `11:00` prevents another booking from `10:30` to `11:30`. A booking starting at `11:00` is allowed. Cancelled reservations do not block availability.

### Cancel a reservation

Select **3** from the role menu. Students and staff see only their own non-cancelled reservations; admins see all non-cancelled reservations. Enter a listed reservation ID to cancel it. The cancellation list includes the reservation ID, room ID, and owner ID.

Cancellation changes the reservation's status to `Cancelled` and saves that status; it does not delete its record. Admins can use **5. Delete reservation permanently** to remove any listed reservation, including cancelled ones. Both operations update `data/reservations.txt`. Reservations and conflict checks survive restarts. New IDs continue beyond the largest ID in the loaded records; deleted IDs may be reused after a restart if they were above every remaining ID.

## Admin user-management workflow

Log in as `A001` and select **4. Manage user accounts**:

```text
1. View users
2. Add user
3. Edit user
4. Delete user
0. Back
```

- **View:** displays ID, name, email, role, and major (Student), department (Staff), or office (Admin).
- **Add:** enter a unique ID, select a role, and supply name, email, and the role-specific field.
- **Edit:** enter an existing ID and all replacement details. The role can change, while the ID stays fixed.
- **Delete:** enter another existing account's ID to remove it from live accounts and the user file.

Duplicate IDs, unknown edit/delete IDs, blank fields, comma-containing fields, and IDs containing spaces/tabs are rejected. The logged-in admin cannot delete their own account or change their own role away from Admin. Changes are saved before the live accounts are updated, and remain available after a restart. The application checks that email is a nonempty field; it does not validate email syntax.

## Milestone 4: Tested MVP (Lecture 10_2) 

### Goal:

Milestone 4 moves the project from demonstrating one working vertical slice to delivering a usable core product. The goal is a stable, tested Minimum Viable Product (MVP): the major behaviors should work together through coherent end-to-end workflows, be covered by meaningful automated tests, build reliably, and pass CI.

The MVP does not need every planned feature. It should provide the core functionality needed for the application to be meaningfully usable.

### Integrated user stories

The current application brings these core user stories together:
| UserStory | Intergrated | User-visible result |
| --- | --- | --- |
| As a student or staff member, I want to find and reserve a room for a time and group size so that I have a suitable space. | Login → room search → date/time and capacity validation → conflict checking → reservation creation → persistence. | A confirmed reservation with an ID, saved for later sessions. |
| As a reservation owner, I want to cancel my booking so that the room becomes available again. | Role-specific reservation list → ownership check → cancellation → persistence → availability refresh. | The reservation is marked Cancelled and no longer blocks its time slot. |
| As a campus administrator, I want to manage user accounts so that people have the correct access to the system. | Admin menu → input validation → account creation/edit/deletion → file save → live account update. | Account changes persist and are reflected after restarting the application. |

### Core Rule

| Area | Rule |
| --- | --- |
| Login | Only an existing user ID is accepted; the user's role determines the available menu actions. |
| Room search | A room must be enabled, meet the requested maximum capacity, and have no overlapping active reservation. |
| Reservation conflicts | Overlapping intervals for the same room are rejected; adjacent intervals are allowed. |
| Cancellation | Students and staff may cancel their own reservations; admins may cancel any reservation. Cancelled reservations do not block availability. |
| Permanent deletion | Only admins may permanently delete reservation records. |
| Account management | Only admins may add, edit, and delete accounts. Duplicate IDs and invalid fields are rejected; an admin cannot delete or demote their own account. |
| Persistence | Reservation and account changes are saved before the in-memory state is updated, so a failed save does not apply the change. |

### Acceptance criteria

| Area | Scenario | Expected result |
| --- | --- | --- |
| End-to-end workflow | A student or staff member logs in, searches, and books an available room. | A valid reservation is created and saved. |
| Integrated workflow | A reservation is cancelled and the same room/time is searched again. | The cancelled booking no longer blocks the room. |
| Permissions | A student or staff member attempts an admin-only action or tries to cancel another user's booking. | The operation is unavailable or rejected. |
| Business rules | A booking overlaps an existing booking for the same room. | The conflicting booking is rejected; an adjacent booking is accepted. |
| Boundary cases | Capacity equals the room limit, and capacity exceeds the room limit. | The room is included at the limit and excluded above it. |
| Invalid input | Invalid dates/times, nonpositive attendee counts, unknown IDs, or invalid account fields are entered. | The input is rejected without corrupting application state. |
| Persistence | The application is restarted after reservation or account changes. | Saved records are loaded and the changes remain available. |
| Failure handling | A save operation fails. | The failure is reported and the live state remains unchanged. |
| Build and tests | The project is built from a clean environment and the automated test suite is run. | The build succeeds and all registered tests pass. |
| CI | The current main branch is checked by GitHub Actions. | The workflow completes successfully with passing checks. |

### Automated testing expectations

Tests should go beyond the initial vertical slice and cover:
- Normal successful behavior.
- Important business rules and permissions.
- Boundary conditions, including capacity limits and adjacent reservation intervals.
- Invalid input and failure cases.
- Interactions between major components, including persistence across application restarts.

The existing C++ tests cover user-role classes, room behavior, file handling, and reservation persistence. Continue expanding coverage for console-level validation and permissions, as well as the complete account-management workflow. Manual demonstrations are useful, but they do not replace automated tests.

Run the full test suite using the commands in C++ tests. Do not define NDEBUG, because the tests use assert.

### Milestone 4 submission checklist

Before submission, verify each item:
- [ ] The core MVP workflows run successfully from start to finish.
- [ ] The major selected user stories are integrated and implemented.
- [ ] Core business rules, permissions, and failure cases behave correctly.
- [ ] The project builds from a clean environment, not only on one teammate's machine.
- [ ] Automated tests cover normal, invalid, boundary, and component-interaction cases.
- [ ] All automated tests pass.
- [ ] CI is green on the current main branch.
- [ ] No major known defect blocks the core workflow.
- [ ] The README explains how to build, run, and test the project.
- [ ] Work is merged through reviewed pull requests, with a clear issue/branch/PR history.
Milestone 4 goal: a usable core product whose major behaviors are integrated, tested, and continuously verified.
*Due date: November 17, 2026.*

## C++ tests

All test sources are C++. Tests use standard `assert` statements with no external testing framework. Do not define `NDEBUG`, which disables assertions.

### Makefile tests

```sh
make all
make test-all
```
The Makefile discovers `tests/test_*.cpp` and runs each as a separate executable. All tests run without interactive input. `make all` builds the application without running tests.
*Individual targets are available: `make test_admin`, `make test_student`, `make test_staff`, `make test_filehandler`, `make test_room`, and `make test_reservation_persistence`.*

### CMake tests

```sh
ctest --test-dir build-cmake --output-on-failure
```
*Individual targets are available: add flag `-R` following with name of test like `AdminTest`, `StudentTest`, `StaffTest`, `FileHandler Test`,`RoomTest`, and `ReservationPersistenceTest`.*

| Test file | Existing coverage |
| --- | --- |
| [test_admin.cpp](tests/test_admin.cpp) | Admin identity, role, email, and office. |
| [test_student.cpp](tests/test_student.cpp) | Student identity, role, email, and major. |
| [test_staff.cpp](tests/test_staff.cpp) | Staff identity, role, email, and department. |
| [test_filehandler.cpp](tests/test_filehandler.cpp) | Temporary-file loading and saving for rooms/users/reservations, all reservation statuses, malformed records, missing/empty files, and failed saves. |
| [test_reservation_persistence.cpp](tests/test_reservation_persistence.cpp) | Startup loading, saved bookings/status, overlap rejection, adjacent intervals, ID continuation, deletion, failed saves, and malformed records. |
| [test_room.cpp](tests/test_room.cpp) | Maximum-capacity boundaries and setter, enabled/disabled rooms, and availability search with conflicts, adjacent bookings, and cancellation. |

## Architecture and files

| Component | Responsibility |
| --- | --- |
| [main.cpp](src/main.cpp) | Startup, login, role menus, console validation, room filtering, booking/cancellation, and admin workflows. |
| [User](include/User.h) | Abstract identity interface with ID, name, email, setters, and virtual role lookup. |
| [Student](include/Student.h), [Staff](include/Staff.h), [Admin](include/Admin.h) | User subclasses with major, department, and office information. |
| [UserManagementSystem](include/UserManagementSystem.h) | Owns user pointers; provides lookup, addition, deletion, existence checks, and admin detection. The admin console coordinates edit/save operations. |
| [Room](include/Room.h) | Room identity, type, maximum capacity, location, availability flag, display, and attendee-limit check. |
| [Reservation](include/Reservation.h) | Booking identity, owner, room, time interval, status, and overlap calculation. |
| [ReservationManagementSystem](include/ReservationManagementSystem.h) | Stores rooms/reservations, indexes bookings by room, generates IDs, checks conflicts, and cancels/deletes reservation records. |
| [FileHandler](include/FileHandler.h) | Loads and saves room, user, and reservation records. User and reservation saves use a temporary file followed by rename. |
| `data/` | Sample room and user records. |
| `tests/` | C++ assertion-based test programs. |
| [Makefile](Makefile) | Application build, run, individual test, and aggregate test targets. |

### File formats and persistence

```text
# data/users.txt (five comma-separated fields)
Role,UserID,Name,Email,MajorOrDepartmentOrOffice

# data/rooms.txt (five comma-separated fields)
RoomID,Type,MaxCapacity,Location,Available
```

The headings above describe the formats; actual files contain records only, with no headers or comments. User roles are `Student`, `Staff`, and `Admin`. Room types are `Classroom`, `Meeting room`, and `Study room`; availability is `1` or `0`. Quoted CSV values are not supported. The room format now has five fields: remove the old `Capacity` column from any external room files before using them; keep the former `MaxCapacity` value.

User-account and reservation changes persist to disk. Rooms are loaded at startup; the current menu does not edit or save them.

`data/reservations.txt` contains six fields per record, without a header:

```text
ReservationID,OwnerID,RoomID,StartEpochSeconds,EndEpochSeconds,Status
```

Times are whole seconds since the Unix epoch, preserving the saved instant across restarts. Status is `Confirmed`, `CheckedIn`, `CheckedOut`, or `Cancelled`. The bundled file starts empty; bookings populate it automatically. A missing file is initialized empty. Invalid records or unknown room IDs stop startup with an error instead of silently overwriting data. Existing room records must be loaded before `ReservationManagementSystem::loadReservations` is called.

`FileHandler::loadReservations` and `saveReservations` handle text conversion and file I/O. `ReservationManagementSystem` rebuilds its indexes and calls the save function before committing creation, cancellation, or deletion. Standalone systems remain in-memory until `loadReservations(filename)` enables persistence. Direct changes to a `Reservation` object do not automatically save it.

### Implemented methods without menu actions

The repository also contains functionality below the console interface:

- `Reservation` provides CheckedIn and CheckedOut status methods; no menu invokes them or enforces a status-transition lifecycle.
- `FileHandler::saveRooms` can serialize room records, and `Room` has attribute setters; there is no admin room-management menu.
- `Admin::deactivateAccount` is a placeholder. `Room::setType` is declared but has no implementation in this checkout.

## Known limitations and future work

- ID-only login is a demonstration mechanism. Production authentication, network access, and online service availability are future work.
- Reservation modification, admin room creation/editing/removal, and event reporting are not available through the console.
- User deletion does not cancel or remove that user's saved reservations. Admin cancellation remains available.
- Date/time, attendee count, and cancellation ownership checks live in the console workflow; the reservation API does not independently enforce all those rules.
- Dates use the machine's local time zone. The console does not require bookings to be in the future.
- Room loading assumes valid numeric fields and does not provide robust malformed-file recovery.
- CMake build support and CTest targets are included. Before submission, verify a clean build, run the complete test suite, confirm CI is green on the current main branch, and expand automated coverage for console permissions/validation and account-management workflows.

The broader stakeholders include students, faculty/staff, student organization leaders, facilities staff, administrators, IT support, and event attendees. The current account model implements only Student, Staff, and Admin; facilities preparation views, event discovery, and monitoring remain backlog items.

## Existing project diagram

The original diagram is retained for reference and should be checked against the current implementation when the design is updated.

![Existing campus room reservation project diagram](https://github.com/user-attachments/assets/9dde57a4-0c19-4f84-acb5-fd752ede688c)
