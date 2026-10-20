# Compiler and flags
CXX := g++
CXXFLAGS := -Iinclude -Wall -Wextra -Wpedantic -std=c++17
LDFLAGS :=

# Directories
SRC_DIR := src
TEST_DIR := tests
BUILD_DIR := build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Application sources and target
APP_SRC := $(wildcard $(SRC_DIR)/*.cpp)
APP_MAIN := $(SRC_DIR)/main.cpp
APP_SRC_NO_MAIN := $(filter-out $(APP_MAIN), $(APP_SRC))
APP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(APP_SRC))
APP_OUT := $(BUILD_DIR)/main

# Test discovery: each tests/test_*.cpp becomes a separate binary
TEST_SOURCES := $(wildcard $(TEST_DIR)/test_*.cpp)
# Derive test names (e.g., tests/test_room.cpp -> test_room)
TEST_NAMES := $(notdir $(basename $(TEST_SOURCES)))
TEST_BINS := $(addprefix $(BUILD_DIR)/,$(TEST_NAMES))

# Object files for tests: allow objects from src and per-test object
TEST_OBJS_COMMON := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(filter-out $(APP_MAIN),$(APP_SRC)))

# Default target
.PHONY: all
all: $(APP_OUT)

# Link application
$(APP_OUT): $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Compile sources to objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pattern rule to build each test binary from its test source + common objects
$(BUILD_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(TEST_OBJS_COMMON)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Convenience run target
.PHONY: run
run: $(APP_OUT)
	./$(APP_OUT)

# Individual test targets (phony)
.PHONY: $(TEST_NAMES)
$(TEST_NAMES): %: $(BUILD_DIR)/%
	./$<

# Aggregate test target
.PHONY: test-all
test-all: $(TEST_BINS)
	@set -e; \
	for t in $(TEST_BINS); do \
	  echo "Running $$t"; \
	  ./$$t; \
	done

# Clean
.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(APP_OUT) $(TEST_BINS)

# Helpful info
.PHONY: info
info:
	@echo "App sources:    $(APP_SRC)"
	@echo "App objects:    $(APP_OBJS)"
	@echo "Test sources:   $(TEST_SOURCES)"
	@echo "Test bins:      $(TEST_BINS)"

