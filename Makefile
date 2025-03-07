CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2 -fopenmp

SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests

SOURCES = $(wildcard $(SRC_DIR)/lib/*.cpp)
TESTS = $(wildcard $(TESTS_DIR)/lib/*.hpp)

TARGET_NAMES = data_gen mettu_plaxton facility_set solution_cost k_median
TARGETS = $(patsubst %,$(BUILD_DIR)/%,$(TARGET_NAMES))

all: $(TARGETS)

$(BUILD_DIR)/unittest: $(TESTS_DIR)/unittest.cpp $(TESTS) $(SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SOURCES) -lgtest -lpthread

$(BUILD_DIR)/%: $(SRC_DIR)/%.cpp $(SOURCES) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< $(SOURCES)

test: $(BUILD_DIR)/unittest
	./$(BUILD_DIR)/unittest

clean:
	rm -rf $(BUILD_DIR)



.PHONY: all clean test
