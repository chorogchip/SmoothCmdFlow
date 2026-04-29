CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -I./src

BUILD_DIR := build
APP := $(BUILD_DIR)/smoothcmdflow
TEST := $(BUILD_DIR)/testexamples

COMMON_SRCS := \
	src/SmoothCmdFlowDevice.cpp \
	src/SmoothCmdFlowSetup.cpp \
	src/StringPool.cpp \
	src/Token.cpp

.PHONY: all run test clean

all: $(APP)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(APP): src/main.cpp $(COMMON_SRCS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST): $(COMMON_SRCS) src/testexamples.h | $(BUILD_DIR)
	printf '#include "testexamples.h"\nint main(){ return scf::testexamples::run_device_tests() ? 0 : 1; }\n' | \
	$(CXX) $(CXXFLAGS) -x c++ - $(COMMON_SRCS) -o $@

run: $(APP)
	$(APP)

test: $(TEST)
	$(TEST)

clean:
	rm -rf $(BUILD_DIR)
