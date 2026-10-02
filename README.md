# cs410_group1
Room reservation in university
Open terminal and run "make run" to compile and run application.

Stakeholders might include:
- Students
- Faculty
- Staff
- Student organization leaders
- Facilities staff
- Campus administrators
- IT support 
- People attending the events

Conflict:
- If the room is already reserved, then should reject any request after it.

Functional requirements:
- Search space: The system should allow the user to search room by some simple criteria like capacity or time slot.
- Check availability: it should allow the stakeholders to see if the room meets their capacity or time choice.
- Create the reservation: The stakeholder can create the reservation if the room is available.
- Modify Reservation: The system shall allow users to modify their existing reservations.
- Cancel Reservation: the stakeholder could be able to cacel their current reservation.
- Resolve conflict: it should prevent any overlap reservation in the system
- Manage Spaces: authorized campus administrators are allowed to add, edit, or remove campus spaces.
- View Space Information: The users can view information about a space, such as capacity, location, and available features. However, base on their depend on the user's account type/permissions, some could gain more right.


Nonfunctional requirements:
- User interface: should be simple and clean interface
- Security: each stakeholder should have an university account to be able to create the request
- Availability: since it a online system, it should be available all the time, but the maintain period


User Stories:
- As a student, I want easily search room by capacity so that I can find a space large enough for my study group.
- As a faculty/staff member, I want to search room by time and capacity so that I could find the one that not conflict with my other schedule.
- As a student organization leader, I want to search and request on big capacity so i could find a space big enough for our club activities and event.
- As a facilities staff, I want to access the room reservations and space requirements, so that I can prepare rooms and make sure they are ready to use.
- As a campus administrator, I want to manage spaces and reservations, so that campus resources are used for the right purpose and also to solve any schedule conflict.
- As an IT support teachnician, I want to have access to monitor and troubleshoot the system, so that it work smoothly for all stakeholders.
- As a person atteding the event, I want get in the system to check the location, time, and relevant details of an event, so that I can easily find and attend the event.

Acceptance Criteria:
- Given a room capacity should be at least 1 slot, when the stakeholders enter 0 or lesser, it may though error.
- Given an available room, when the student enters a valid date and time, then the reservation is created.
- Given an already reserved room, when another student requests an overlapping time, then the reservation is rejected.
- Given a room capacity of 15, when 16 attendees or more are requested, then the reservation is rejected


