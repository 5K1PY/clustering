CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2 -fopenmp

SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests

SOURCES = $(wildcard $(SRC_DIR)/lib/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/lib/*.hpp)
TESTS = $(wildcard $(TESTS_DIR)/*.hpp)

TARGET_NAMES = data_gen mettu_plaxton facility_set solution_cost k_median
TARGETS_Z1 = $(patsubst %,$(BUILD_DIR)/%_z1,$(TARGET_NAMES))
TARGETS_Z2 = $(patsubst %,$(BUILD_DIR)/%_z2,$(TARGET_NAMES))

all: $(TARGETS_Z1)
z1: $(TARGETS_Z1)
z2: $(TARGETS_Z2)

$(BUILD_DIR)/unittest: $(TESTS_DIR)/unittest.cpp $(TESTS) $(SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SOURCES) -lgtest -lpthread

$(BUILD_DIR)/%_z1: $(SRC_DIR)/%.cpp $(SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SOURCES)

$(BUILD_DIR)/%_z2: $(SRC_DIR)/%.cpp $(SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SOURCES) -D Z2

test: $(BUILD_DIR)/unittest
	./$(BUILD_DIR)/unittest

clean:
	rm -rf $(BUILD_DIR)


.PHONY: all clean test
