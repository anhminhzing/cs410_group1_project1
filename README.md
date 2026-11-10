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

```sh
make all
make run
```

`make all` builds `build/main`; `make run` builds it if needed and starts it. You can also run `./build/main` from the repository root. To select another compiler, use `make CXX=clang++ all`.

This checkout currently uses a Makefile. The GitHub Actions workflow in `.github/workflows/ci.yml` builds and tests with Make on pushes and pull requests, and supports manual runs. Commit and push the workflow to activate it; inspect results in GitHub’s Actions tab. A successful remote run is not yet recorded. CMake configuration is still needed for Milestone 3; CMake commands are not yet available.

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

## Milestone 3: Working Vertical Slice + CI

The Week 7.2 slides (slide 35) require a vertical slice tied to a user story, a successful CMake build, 2–3 meaningful automated tests, issue → branch → PR evidence, a working CI pipeline, and updated README instructions.

**Sprint goal:** Demonstrate a room reservation from login through search, validation, booking, conflict handling, and cancellation. Admin account management is an additional end-to-end workflow in the same application.

### User stories and vertical slices

| Story | User-visible value | Path through the implementation |
| --- | --- | --- |
| As a student or staff member, I want to find and reserve a room for a time and group size so that I have a suitable space. | A confirmed booking with a reservation ID. | Console input → account lookup → date/capacity validation → Room filtering → ReservationManagementSystem → Reservation → success/failure output. |
| As a reservation owner, I want to cancel my booking so that the room becomes available again. | The booking is cancelled and its time slot is released. | Role menu → permitted reservation list → ownership check → cancellation status update → availability check. |
| As a campus administrator, I want to manage user accounts so that people have the correct access to the system. | Accounts can be viewed, created, edited, and removed with persistent changes. | Admin menu → validation → Student/Staff/Admin objects → FileHandler save → live account update → result output. |

### Acceptance criteria

| Area | Scenario | Expected result |
| --- | --- | --- |
| Login | An existing ID is entered | The matching user's role menu opens. |
| Login | An unknown ID is entered | Login is rejected and can be retried. |
| Room search | A room is available and meets the requested maximum capacity | The room appears in the search results. |
| Reservation | A valid available room and time interval are selected | A confirmed reservation is created for the logged-in user. |
| Reservation | Date/time is invalid, end is not after start, or attendees are not a positive integer | The console rejects the request. |
| Reservation | Requested attendees exceed the room's maximum capacity | That room is excluded from results. |
| Reservation | The same room has an overlapping non-cancelled booking | That room is excluded; a conflicting direct creation request is also rejected. |
| Reservation | A booking starts when another ends | The time intervals do not conflict. |
| Cancellation | An owner cancels a listed booking | Its status becomes Cancelled and the time slot becomes available. |
| Cancellation | A student/staff member supplies another user's booking ID | The console rejects the request. |
| User management | An admin adds a unique account or edits/deletes an existing account | The change is saved to disk and reflected in live accounts. |
| User management | A duplicate ID or nonexistent edit/delete ID is entered | The request is rejected. |
| User management | A student or staff member selects account management | The option is unavailable and the choice is rejected. |
| User management | The admin attempts self-deletion or self-demotion | The request is rejected. |
| Persistence | Saving an account change fails | The error is reported and live accounts remain unchanged. |

### Sprint-review demonstration

**Reservation and cancellation:**

1. Start `make run`, log in as `S001`, and view the rooms.
2. Select **2**. Enter `11/10/2026`, `10:00`, `11:00`, and `5` attendees, then select `R101`.
3. Verify the generated ID (for example, `RES_1` when the saved file was empty).
4. Search again for the same date, `10:30`–`11:30`, and `5` attendees. Verify that `R101` is unavailable.
5. Log out and log in as `T001`. Select **3** and verify that the student's booking is not listed.
6. Log out and log in as `A001`. Select **3** and cancel the student's reservation using its ID.
7. Repeat the search and verify that `R101` is available again. Enter `0` at room selection to return without booking.

Reservations persist across restarts; choose a different time if an earlier demo booking still occupies the slot. Both `R101` and `R102` are enabled in the bundled data.

**Account management and persistence:**

1. As `A001`, open **4**, then view users.
2. Add an unused ID such as `S900`, role `1` (Student), name `Demo Student`, email `demo@student.edu`, and major `Computer Science`.
3. Return, log out, and log in as `S900` to verify its Student menu.
4. Log back in as `A001`, edit `S900`, and inspect the updated details.
5. Exit and restart the application; verify that the edited account still exists.
6. Delete `S900` as admin and verify that it can no longer log in. Also demonstrate rejection of an existing ID such as `S001` when adding an account.

This second demo modifies `data/users.txt`. Remove the demo account afterward. FileHandler tests use isolated temporary records and do not depend on this demo data.

## C++ tests

All test sources are C++. Tests use standard `assert` statements with no external testing framework. Do not define `NDEBUG`, which disables assertions.

```sh
make all
make test-all
```

The Makefile discovers `tests/test_*.cpp` and runs each as a separate executable. All tests run without interactive input. `make all` builds the application without running tests.

| Test file | Existing coverage |
| --- | --- |
| [test_admin.cpp](tests/test_admin.cpp) | Admin identity, role, email, and office. |
| [test_student.cpp](tests/test_student.cpp) | Student identity, role, email, and major. |
| [test_staff.cpp](tests/test_staff.cpp) | Staff identity, role, email, and department. |
| [test_filehandler.cpp](tests/test_filehandler.cpp) | Temporary-file loading and saving for rooms/users/reservations, all reservation statuses, malformed records, missing/empty files, and failed saves. |
| [test_reservation_persistence.cpp](tests/test_reservation_persistence.cpp) | Startup loading, saved bookings/status, overlap rejection, adjacent intervals, ID continuation, deletion, failed saves, and malformed records. |
| [test_room.cpp](tests/test_room.cpp) | Maximum-capacity boundaries and setter, enabled/disabled rooms, and availability search with conflicts, adjacent bookings, and cancellation. |

Individual targets are available: `make test_admin`, `make test_student`, `make test_staff`, `make test_filehandler`, and `make test_room`.

### Missing automated workflow coverage

The C++ persistence test covers reservation creation, conflicts, cancellation, deletion, restart loading, and failed saves. Console permissions and the complete account-management workflow still need automated coverage. FileHandler tests use temporary data and clean up after themselves. Further coverage for the reservation story should include:

1. CLI date and attendee-count validation.
2. Owner-only cancellation and admin-only permanent deletion.
3. Restart behavior exercised through the console workflow.

Additional account tests should cover add/edit/delete persistence, duplicate IDs, self-protection, and failed saves. CLI date/capacity/permission rules need a C++ integration harness or extraction into testable application logic. Manual demonstrations do not replace automated tests.

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

## Milestone 3 evidence and remaining work

| Requirement | Current local evidence / remaining work |
| --- | --- |
| Working vertical slice tied to a story | Reservation creation/cancellation and admin account management are connected through the console. Stories, acceptance criteria, and demos are documented above. |
| Successful CMake build | Makefile is present; Makefile have the same purpose with CMake that is building project |
| 2–3 meaningful automated tests for the slice | Reservation persistence tests cover success, overlap rejection, cancellation, deletion, restart loading, and save failures. Console permissions and account workflow tests remain outstanding. |
| Issue → branch → PR workflow | Implement in resporitory |
| Working CI pipeline | [ci.yml](.github/workflows/ci.yml) builds and tests with Make on pushes/PRs and manual runs. **Pending:** verify a successful GitHub run, and migrate to CMake/CTest when configured. |
| Updated README | Full feature descriptions, build/run/test instructions, stories, demos, architecture, and known limitations are included, in docs folder, there is workflow.md that describe user work flow |

Record actual submission evidence here; remote issue, review, and CI status has not been verified:

- User-story issue: **Not recorded**
- Feature branch: **Not recorded**
- Reviewed pull request: **Not recorded**
- Successful CI run for the submitted revision: **Not recorded**

Use focused feature branches tied to issues, reference those issues in PRs, request teammate review, and merge after required checks pass. When CMake/CTest are added, register the C++ tests, set the data working directory, and keep tests noninteractive. At sprint review, show the story, running feature, automated tests, reviewed PR, and green CI. Record retrospective findings separately, including one improvement for the next sprint.

## Known limitations and future work

- ID-only login is a demonstration mechanism. Production authentication, network access, and online service availability are future work.
- Reservation modification, admin room creation/editing/removal, and event reporting are not available through the console.
- User deletion does not cancel or remove that user's saved reservations. Admin cancellation remains available.
- Date/time, attendee count, and cancellation ownership checks live in the console workflow; the reservation API does not independently enforce all those rules.
- Dates use the machine's local time zone. The console does not require bookings to be in the future.
- Room loading assumes valid numeric fields and does not provide robust malformed-file recovery.
- CMake, successful remote CI evidence, reservation workflow tests, and account workflow tests remain submission work as described above.

The broader stakeholders include students, faculty/staff, student organization leaders, facilities staff, administrators, IT support, and event attendees. The current account model implements only Student, Staff, and Admin; facilities preparation views, event discovery, and monitoring remain backlog items.

## Existing project diagram

The original diagram is retained for reference and should be checked against the current implementation when the design is updated.

![Existing campus room reservation project diagram](https://github.com/user-attachments/assets/9dde57a4-0c19-4f84-acb5-fd752ede688c)
