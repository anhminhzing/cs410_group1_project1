CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17

APP_SRC = src/main.cpp src/Room.cpp
APP_OUT = build/main

TEST_ROOM_SRC = tests/test_room.cpp src/Room.cpp
TEST_ROOM_OUT = build/test_room

TEST_FILE_SRC = tests/test_filehandler.cpp src/Room.cpp src/FileHandler.cpp
TEST_FILE_OUT = build/test_filehandler

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

clean:
	rm -f $(OUT)
