# Makefile for the web server project

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = bin
BIN_DIR = .

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Target executable
TARGET = $(BIN_DIR)/server

# Default target
all: prepare $(TARGET)

# Prepare directories
prepare:
	@mkdir -p logs
	@mkdir -p www
	@mkdir -p $(OBJ_DIR)

# Compile source files to object files in /bin
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Clean everything including logs
distclean: clean
	rm -f logs/*

# Install target (copies executable to /usr/local/bin)
install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the server (default)"
	@echo "  clean      - Remove object files and executable"
	@echo "  distclean  - Remove object files, executable, and logs"
	@echo "  install    - Install the server to /usr/local/bin"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make"
	@echo "  ./server <port> <directory>"
	@echo ""
	@echo "Example:"
	@echo "  make"
	@echo "  ./server 8080 www"

.PHONY: all prepare clean distclean install help
