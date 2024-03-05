CXX = g++
CXXFLAGS = -std=c++11 -Wall
LIBS = -lboost_system -pthread -lsqlite3

SRCS = src/main.cpp src/Account.cpp src/db.cpp
BUILD_DIR = build
OBJS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.cpp=.o)))
TARGET = rest_api
BUILD_TARGET = $(addprefix $(BUILD_DIR)/,$(TARGET))

.PHONY: all clean

all: $(BUILD_TARGET)

$(BUILD_TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)

