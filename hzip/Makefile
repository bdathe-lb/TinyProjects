SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin
DEP_DIR := $(BUILD_DIR)/dep

CC := gcc
CFLAGS := -Wall -Werror -I$(INC_DIR)
DEBUG_FLAGS := -g -O0
RELEASE_FLAGS := -O2

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS := $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRCS))
TARGET := $(BIN_DIR)/hzip

all: release

release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)
 
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET) 

$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

$(DEP_DIR):
	@mkdir -p $(DEP_DIR)

-include $(DEPS)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all release debug run clean