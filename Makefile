CXX = g++
CXXFLAGS = -Wall -std=c++20 -O2 -fopenmp

SRC_DIR = src
EXTERNAL_DIR = external_solutions
BUILD_DIR = build
TESTS_DIR = tests
OBJ_DIR = $(BUILD_DIR)/obj
LIB_OBJ_DIR_Z1 = $(OBJ_DIR)/lib_z1
LIB_OBJ_DIR_Z2 = $(OBJ_DIR)/lib_z2

LIB_SOURCES = $(wildcard $(SRC_DIR)/lib/*.cpp)
HEADERS = $(wildcard $(SRC_DIR)/lib/*.hpp)
TESTS = $(wildcard $(TESTS_DIR)/*.hpp)

LIB_OBJECTS_Z1 = $(patsubst $(SRC_DIR)/lib/%.cpp,$(LIB_OBJ_DIR_Z1)/%.o,$(LIB_SOURCES))
LIB_OBJECTS_Z2 = $(patsubst $(SRC_DIR)/lib/%.cpp,$(LIB_OBJ_DIR_Z2)/%.o,$(LIB_SOURCES))

TARGET_NAMES = data_gen mettu_plaxton facility_set facility_set_cost clustering clustering_cost
TARGETS_Z1 = $(patsubst %,$(BUILD_DIR)/%_z1,$(TARGET_NAMES))
TARGETS_Z2 = $(patsubst %,$(BUILD_DIR)/%_z2,$(TARGET_NAMES))

EXTERNAL_NAMES_Z2 = scikit
EXTERNAL_Z2 = $(patsubst %,$(BUILD_DIR)/%_z2,$(EXTERNAL_NAMES_Z2))

$(shell mkdir -p $(BUILD_DIR) $(LIB_OBJ_DIR_Z1) $(LIB_OBJ_DIR_Z2))

all: z1 z2
z1: $(TARGETS_Z1)
z2: $(TARGETS_Z2) $(EXTERNAL_Z2)

$(LIB_OBJ_DIR_Z1)/%.o: $(SRC_DIR)/lib/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB_OBJ_DIR_Z2)/%.o: $(SRC_DIR)/lib/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -D Z2 -c -o $@ $<

$(BUILD_DIR)/unittest: $(TESTS_DIR)/unittest.cpp $(TESTS) $(LIB_OBJECTS_Z1)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJECTS_Z1) -lgtest -lpthread

$(BUILD_DIR)/%_z1: $(SRC_DIR)/%.cpp $(LIB_OBJECTS_Z1)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJECTS_Z1)

$(BUILD_DIR)/%_z2: $(SRC_DIR)/%.cpp $(LIB_OBJECTS_Z2)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LIB_OBJECTS_Z2)

$(BUILD_DIR)/scikit_z2: $(EXTERNAL_DIR)/scikit.py
	cp $< $@
	chmod u+x $@

test: $(BUILD_DIR)/unittest
	./$(BUILD_DIR)/unittest

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all z1 z2 clean test
