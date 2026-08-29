SRC_DIR ?= src
INCLUDE_DIR ?= include
BUILD_DIR ?= build

CC = cc
CPPFLAGS = $(addprefix -I,$(INCLUDE_DIR))
CFLAGS = -Wall -Wextra -fsanitize=address 
LDFLAGS = -fsanitize=address
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TARGET = $(BUILD_DIR)/cstow

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking to $(TARGET)"
	@$(CC) $(LDFLAGS) $^ -o $@ 

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@echo "Building $@ file"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(BUILD_DIR):
	@echo "Creating $(BUILD_DIR) directory..."
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "Removing $(BUILD_DIR) directory"
	@rm -rf $(BUILD_DIR)

run: $(TARGET)
	@./$(TARGET)

-include $(OBJ:.o=.d)

.PHONY: all clean run
