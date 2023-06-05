# Makefile
# Compiler options
CC = gcc
CFLAGS = -Wall -Werror
LDFLAGS = -L/usr/local/ethercat/lib -lethercat

# Output directory for object files
BUILD_DIR = build

# List of subfolders to include
SUBDIRS = \
	Modules/ethCatSpi \
	Modules/CanIdle

# Define the target name
TARGET = ighEthCatMaster

# List of source files
SRCS = main.c

# List of object files to build
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

# Libraries to link against
LIBS = -lethercat

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	for dir in $(SUBDIRS); do \
		mkdir -p $(BUILD_DIR)/$$dir; \
	done

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
