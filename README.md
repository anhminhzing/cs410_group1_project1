# Campus Room Reservation System

CS410 Group 1 project: a C++17 command-line application for university room reservations. Students and staff can find and reserve rooms; administrators can also manage user accounts.

## Milestone 3: Working Vertical Slice + CI

The Week 7.2 slides ("Milestone 3 Submission," slide 35) require one vertical slice tied to a user story, a successful CMake build, 2–3 meaningful automated tests, an issue → branch → pull request workflow, working CI, and updated README instructions.

### Sprint goal and selected user story

**Sprint goal:** Deliver an administrator account-management workflow from login through validation, file storage, and visible results.

**User story:** As a campus administrator, I want to view, add, edit, and delete user accounts so that I can manage access to the room reservation system.

The vertical slice follows this path:

```text
Enter an existing Admin ID
  → Open Manage user accounts
  → View, add, edit, or delete an account
  → Validate the request
  → Build the proposed Student, Staff, or Admin records
  → Save changes to data/users.txt
  → Update live accounts and display the result
  → Restart and log in with the saved account
```

### Acceptance criteria

| Scenario | Expected result |
| --- | --- |
| An existing admin logs in | The admin menu includes Manage user accounts. |
| A student or staff member logs in | The account-management option is unavailable. |
| An admin views users | IDs, names, emails, roles, and major/department/office are displayed. |
| An admin adds a unique ID with valid fields | The account is saved and can log in after a restart. |
| An admin adds an existing ID | The request is rejected without replacing the existing account. |
| An admin edits an existing account | Name, email, role, and role-specific details are saved; the ID remains unchanged. |
| An admin deletes another account | The account is removed from memory and storage and can no longer log in. |
| An admin edits or deletes a nonexistent ID | The request is rejected. |
| An admin tries to delete their logged-in account or remove its Admin role | The request is rejected. |
| A required field is blank or contains a comma | The application requests a valid field. |
| Saving fails | The application reports the failure and leaves live accounts unchanged. |

### Milestone evidence and remaining work

This table describes evidence available in this checkout. It does not establish the status of remote issues, reviews, or CI runs.

| Required deliverable | Local evidence / submission status |
| --- | --- |
| One working vertical slice | Admin user management in [main.cpp](src/main.cpp), account classes, and [FileHandler.cpp](src/FileHandler.cpp). See the demo below. |
| Code builds with CMake | **Pending:** this checkout uses [Makefile](Makefile); no `CMakeLists.txt` is present. A Make build does not satisfy the CMake requirement. |
| At least 2–3 meaningful automated tests | Five C++ test files exist (see below). **Pending for the selected slice:** automated account-management success, failure/boundary, and business-rule tests. Existing class/file assertions do not verify the complete admin workflow. |
| Issue → branch → PR workflow | **Evidence needed:** record the actual user-story issue, implementation branch, and reviewed PR links below. |
| Working CI pipeline | **Pending locally:** no `.github/workflows/` configuration is present. Add CI that configures with CMake, builds, and runs the tests; attach a successful run. |
| Updated README instructions | Build/run commands, login instructions, demo, tests, architecture, and limitations are documented here. |

Before submission, complete this evidence record with real links:

- User-story issue: **Not recorded**
- Implementation branch: **Not recorded**
- Pull request and teammate review: **Not recorded**
- Successful CI run for the submitted revision: **Not recorded**

## Build and run the current application

### Prerequisites

- A C++ compiler with C++17 support (`g++` or a compatible compiler).
- GNU Make.

Run commands from the repository root because the application loads `data/rooms.txt` and `data/users.txt` relative to the working directory.

```sh
make all
make run
```

To use another compiler:

```sh
make CXX=clang++ all
```

The executable is `build/main`. The current project has no CMake build command; add and verify CMake configuration before claiming that Milestone 3 requirement is complete.

### Login and role permissions

The application displays available account IDs, names, and roles before login. Enter an existing ID; an unknown ID prompts another attempt. Enter `0` at login to exit or `0` in a role menu to log out.

Default sample accounts include `S001` (Student), `T001` (Staff), and `A001` (Admin). The displayed list reflects changes made to the user file.

| Function | Student | Staff | Admin |
| --- | --- | --- | --- |
| View rooms | Yes | Yes | Yes |
| Search by time and required capacity; reserve a room | Yes | Yes | Yes |
| Cancel a reservation | Own reservations | Own reservations | Any reservation |
| View full user details; add, edit, or delete accounts | No | No | Yes |

Login uses an ID only; passwords and university identity-provider authentication are not implemented.

## Vertical-slice demonstration

Run `make run` from the repository root. This demo changes `data/users.txt`; use a unique test ID and delete it afterward.

1. Log in as `A001`.
2. Select **4. Manage user accounts**, then **1. View users**.
3. Select **2. Add user**. Enter `S900` (if unused), role `1` (Student), name `Demo Student`, email `demo@student.edu`, and major `Computer Science`.
4. Verify the success message. Select **0. Back**, then **0. Log out**. Log in as `S900` and verify the Student menu.
5. Log out, log in as `A001`, open user management, and select **3. Edit user**. Enter `S900`, then supply the replacement role, name, email, and role-specific field. Editing requires entering all these fields; the ID stays fixed.
6. Select **1. View users** to inspect the update. Return, log out, and exit. Restart the application and log in as `S900` to demonstrate persistence.
7. Log in as `A001` again, open user management, select **4. Delete user**, and enter `S900`. After logging out, verify that `S900` is rejected at login.
8. Demonstrate a business rule by attempting to add `S001` again or delete the currently logged-in `A001`; verify that the request is rejected.

To demonstrate reservations separately, log in, select **2**, enter a date as `MM/DD/YYYY`, start/end times as 24-hour `HH:MM`, and a positive required capacity. Choose a room ID from the available results. Overlapping reservations for the same room are rejected. Reservations remain available across logouts during the same process but are lost when the application exits.

## Automated tests (C++ only)

All application and test source files are C++. The tests use standard C++ `assert` statements; no external testing framework is required. Do not define `NDEBUG` when running them, because it disables assertions.

Build and run the current suite from the repository root:

```sh
make all
printf '5\n' | make test-all
```

The Makefile discovers `tests/test_*.cpp`, builds each file as a separate executable in `build/`, and runs them. `test_room` prompts for a positive capacity, so the command supplies `5` to allow the suite to finish without interaction. `make all` builds the application; it does not execute tests.

| C++ test file | Current coverage |
| --- | --- |
| [test_admin.cpp](tests/test_admin.cpp) | Admin identity, email, role, and office fields. |
| [test_student.cpp](tests/test_student.cpp) | Student identity, email, role, and major fields. |
| [test_staff.cpp](tests/test_staff.cpp) | Staff identity, email, role, and department fields. |
| [test_filehandler.cpp](tests/test_filehandler.cpp) | Loading the sample rooms and users, including role-specific fields. |
| [test_room.cpp](tests/test_room.cpp) | Room fields and capacity setters; includes an interactive capacity-search demonstration. |

Individual test commands are also available:

```sh
make test_admin
make test_student
make test_staff
make test_filehandler
printf '5\n' | make test_room
```

### Tests still needed for the selected vertical slice

The current tests check supporting classes and file loading. They do not yet establish the admin account-management acceptance criteria. To meet the Milestone 3 testing goal, add at least these three meaningful automated cases in C++:

1. **Successful operation:** add an account, save and reload it, and verify the ID, role, and account details.
2. **Failure/boundary case:** attempt to add a duplicate ID and verify that the original account and account count remain unchanged.
3. **Business rule:** verify that a non-admin cannot manage accounts or that the logged-in admin cannot delete or demote their own account.

Also cover editing/deleting accounts and failed-save behavior as the suite grows. Some rules currently live in console helpers in `main.cpp`; move them into testable application logic or use a C++ integration harness to test the real workflow. The manual demo above is useful for sprint review but does not replace automated tests.

When CMake and CTest are added, register the C++ tests and ensure the data-dependent tests run from the correct working directory. Provide input for, or remove interaction from, the room test before running it in CI. Do not list CTest commands as working until this configuration has been implemented and verified.

## Architecture and repository layout

| Location | Responsibility |
| --- | --- |
| `src/main.cpp` | Console input, login, role menus, request validation, and workflow coordination. |
| `include/User.h`, `src/User.cpp` | Shared user identity fields and abstract role interface. |
| `Student`, `Staff`, `Admin` classes | Role-specific account information: major, department, and office. |
| `UserManagementSystem` | Account lookup, storage, ownership, and admin-role checks. |
| `FileHandler` | Loads room/user data and saves user-account changes through a temporary file followed by rename. |
| `Room`, `Reservation` | Room attributes, reservation ownership/status, and overlap logic. |
| `ReservationManagementSystem` | Reservation records, room indexes, availability checks, and cancellation. |
| `data/` | Initial room and user records. |
| `tests/` | C++ assertion-based class and file-loading tests. |
| `Makefile` | Current build and C++ test targets. |

User records have the format:

```text
Role,UserID,Name,Email,MajorOrDepartmentOrOffice
```

Room records have the expected format:

```text
RoomID,Type,Capacity,MaxCapacity,Location,Available
```

Roles are `Student`, `Staff`, and `Admin`. Room types are `Classroom`, `Meeting room`, and `Study room`; availability is `1` or `0`. Account input rejects commas because the file format does not support quoted CSV fields.

## Team workflow and CI completion

For the Milestone 3 submission:

1. Tie the selected user story and acceptance criteria to a GitHub issue.
2. Implement work on a focused feature branch and reference the issue in its PR.
3. Add the missing C++ vertical-slice tests, CMake configuration, and CTest registration. Preserve the application's required data working directory and remove or supply interactive test input.
4. Add a CI workflow triggered by pushes and pull requests that configures with CMake, builds, and runs all registered tests.
5. Request teammate review. Verify behavior, business rules, meaningful tests, coding conventions, and passing CI before merging.
6. Add the actual issue, branch, PR/review, and green CI links to the evidence record above.
7. At sprint review, show the user story, run the demo, explain the tests, and show the reviewed PR and successful CI result.

For the retrospective, record what helped delivery, what slowed integration, whether tests caught problems, and one specific improvement for Sprint 2. Team review and retrospective outcomes have not been recorded here.

## Remaining product backlog and limitations

The broader stakeholders include students, faculty/staff, student organization leaders, facilities staff, campus administrators, IT support, and event attendees. Only Student, Staff, and Admin are represented as account roles in the current application.

Future stories include:

- As a reservation owner, I want to modify a booking without cancelling and recreating it.
- As a campus administrator, I want to add, edit, and remove campus spaces.
- As facilities staff, I want to inspect upcoming reservations and preparation requirements.
- As an event attendee, I want to find event locations, times, and relevant details.
- As IT support, I want monitoring and troubleshooting information.

Current limitations relevant to evaluation:

- Reservations are stored in memory only; user-account changes persist to disk.
- This is a local console prototype. Online availability, production authentication, and separate permissions for the broader stakeholders are future requirements.
- Reservation date/time, capacity, and ownership checks are coordinated by the console workflow; they are not all enforced by the reservation-system API itself.
- The CLI capacity filter uses `Room::canAccommodate`, which compares requested attendees against `maxcapacity`.
- The separate `getAvailableRooms` API has an iterator-condition defect and is not used by the current console reservation workflow.
- The bundled `R102` room row contains an extra comma-separated field. The current loader therefore does not interpret its location and availability as intended; correct the fixture separately before using that room in a demo.

## Existing project diagram

The original project diagram is retained for reference; check it against the current code when updating the design.

![Existing campus room reservation project diagram](https://github.com/user-attachments/assets/9dde57a4-0c19-4f84-acb5-fd752ede688c)
