# Compiler
CC := gcc
# Compiler flags
CFLAGS := -Wall -Wextra -g
# Library flags
LFLAGS := -pthread
# Source directory
SRC_DIR := ./src
# Object directory
OBJ_DIR := ./obj
# Binary directory
BIN_DIR := ./bin
# Target executable name
TARGET := life

# Find all source files in the source directory
SRCS := $(wildcard $(SRC_DIR)/*.c)
# Generate object file names from source files
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: clean
# Default target - builds the binary file
all: $(BIN_DIR)/$(TARGET)

# Rule to build the target binary file
$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

# Rule to build object files from source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(LFLAGS)

# Rule to create the object directory
$(OBJ_DIR):
	mkdir -p $@

# Rule to create the binary directory
$(BIN_DIR):
	mkdir -p $@

# Clean rule - removes object and binary directories
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
