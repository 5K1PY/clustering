CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2 -fopenmp

SRC_DIR = src
BUILD_DIR = build
TESTS_DIR = tests

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
TESTS = $(wildcard $(TESTS_DIR)/*.h)

TARGET_NAMES = data_gen mettu_plaxton facility_set solution_cost k_median
TARGETS = $(patsubst %,$(BUILD_DIR)/%,$(TARGET_NAMES))

all: $(TARGETS)

$(BUILD_DIR)/unittest: $(TESTS_DIR)/unittest.cpp $(TESTS) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $< -lgtest -lpthread

$(BUILD_DIR)/%: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<

test: $(BUILD_DIR)/unittest
	./$(BUILD_DIR)/unittest

clean:
	rm -rf $(BUILD_DIR)



.PHONY: all clean test
