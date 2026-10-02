CXX = g++
CXXFLAGS = -Iinclude -Wall -std=c++17

SRC = src/main.cpp src/Room.cpp
OUT = build/main

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
