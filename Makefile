CXX = g++
CXXFLAGS = -Wall -std=c++17 -O2

SRC_DIR = src
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/*.h)

TARGET_NAMES = data_gen mettu_plaxton facility_set solution_cost
TARGETS = $(patsubst %,$(BUILD_DIR)/%,$(TARGET_NAMES))

all: $(TARGETS)

$(BUILD_DIR)/%: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $<


clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
