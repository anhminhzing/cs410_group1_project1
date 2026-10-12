CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17

APP_SRC = src/main.cpp \
          src/Room.cpp \
          src/User.cpp \
          src/Student.cpp \
          src/Staff.cpp \
          src/Admin.cpp \
          src/FileHandler.cpp \
          src/UserManagementSystem.cpp

APP_OUT = build/main


TEST_ROOM_SRC = tests/test_room.cpp \
                src/Room.cpp

TEST_ROOM_OUT = build/test_room


TEST_FILE_SRC = tests/test_filehandler.cpp \
                src/Room.cpp \
                src/User.cpp \
                src/Student.cpp \
                src/Staff.cpp \
                src/Admin.cpp \
                src/FileHandler.cpp

TEST_FILE_OUT = build/test_filehandler


TEST_ADMIN_SRC = tests/test_admin.cpp \
                 src/User.cpp \
                 src/Admin.cpp

TEST_ADMIN_OUT = build/test_admin


TEST_STUDENT_SRC = tests/test_student.cpp \
                   src/User.cpp \
                   src/Student.cpp

TEST_STUDENT_OUT = build/test_student


TEST_STAFF_SRC = tests/test_staff.cpp \
                 src/User.cpp \
                 src/Staff.cpp

TEST_STAFF_OUT = build/test_staff


all:
	$(CXX) $(CXXFLAGS) $(APP_SRC) -o $(APP_OUT)


run: all
	./$(APP_OUT)


test-room:
	$(CXX) $(CXXFLAGS) $(TEST_ROOM_SRC) -o $(TEST_ROOM_OUT)
	./$(TEST_ROOM_OUT)


test-file:
	$(CXX) $(CXXFLAGS) $(TEST_FILE_SRC) -o $(TEST_FILE_OUT)
	./$(TEST_FILE_OUT)


test-admin:
	$(CXX) $(CXXFLAGS) $(TEST_ADMIN_SRC) -o $(TEST_ADMIN_OUT)
	./$(TEST_ADMIN_OUT)


test-student:
	$(CXX) $(CXXFLAGS) $(TEST_STUDENT_SRC) -o $(TEST_STUDENT_OUT)
	./$(TEST_STUDENT_OUT)


test-staff:
	$(CXX) $(CXXFLAGS) $(TEST_STAFF_SRC) -o $(TEST_STAFF_OUT)
	./$(TEST_STAFF_OUT)


test-all: test-room test-file test-admin test-student test-staff


clean:
	rm -f $(APP_OUT) \
	      $(TEST_ROOM_OUT) \
	      $(TEST_FILE_OUT) \
	      $(TEST_ADMIN_OUT) \
	      $(TEST_STUDENT_OUT) \
	      $(TEST_STAFF_OUT)
