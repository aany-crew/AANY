CC := g++
CFLAGS = -std=c++17 -Wall -Wextra -Werror
CPPFLAGS := -Iinclude

ifeq ($(DEBUG), 1)
	CFLAGS += -g -DDEBUG
endif

SRC_DIR := src
OBJ_DIR := obj
INC_DIR := include
APP_DIR := apps
BIN_DIR := bin
TEST_DIR := test
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
INC := $(wildcard $(INC_DIR)/*.h)

.PHONY: all clean

all: app_client

app_client: $(OBJ_DIR)/app_client.o $(OBJ_DIR)/client.o | $(BIN_DIR)
	$(CC) $^ -o $(BIN_DIR)/$@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(APP_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR) $(BIN_DIR):
	mkdir $@

clean:
	rm $(OBJ_DIR)/* $(BIN_DIR)/*

# Tests
#scan_text: test/scanning_test.cpp $(OBJ_DIR)/scanning.o
#	$(CC) -std=c++17 -c $< -o test_scan_text.out

scan_text: test/scanning_test.cpp $(OBJ_DIR)/scanning.o | $(BIN_DIR)
	$(CC) $(CPPFLAGS) -std=c++14 -c $< -o $(BIN_DIR)/test_scan_text.out