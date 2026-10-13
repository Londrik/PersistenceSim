# ==============================================================================
# Cross-Compilation Makefile (Fedora Linux -> Win32 x86_64)
# ==============================================================================

CC           := x86_64-w64-mingw32-gcc
CFLAGS       := -Wall -Wextra -std=c11 -DUNICODE -D_UNICODE -O2
INCLUDES     := -Isrc/include
LDFLAGS      := -municode -ladvapi32
TEST_LDFLAGS := -ladvapi32

BUILD_DIR    := build
SRC_DIR      := src
TEST_DIR     := tests
UNITY_DIR    := $(TEST_DIR)/unity

TARGET       := $(BUILD_DIR)/PersistenceSim.exe
TEST_TARGET  := $(BUILD_DIR)/test_runner.exe

SRCS         := $(shell find $(SRC_DIR) -maxdepth 1 -name '*.c')
OBJS         := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/obj/%.o, $(SRCS))

TEST_SRCS    := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS    := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test_obj/%.o, $(TEST_SRCS)) \
                $(BUILD_DIR)/test_obj/unity.o

SRC_NO_MAIN  := $(filter-out $(BUILD_DIR)/obj/main.o, $(OBJS))

.PHONY: all clean test doc run-test directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)/obj $(BUILD_DIR)/test_obj $(BUILD_DIR)/doc

$(TARGET): $(OBJS)
	@echo "==> Linking Target: $@"
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "==> Compiling: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

test: directories $(TEST_TARGET) run-test

$(TEST_TARGET): $(SRC_NO_MAIN) $(TEST_OBJS)
	@echo "==> Linking Test Runner: $@"
	$(CC) $^ -o $@ $(TEST_LDFLAGS)

$(BUILD_DIR)/test_obj/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "==> Compiling Test: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -I$(UNITY_DIR) -c $< -o $@

$(BUILD_DIR)/test_obj/unity.o: $(UNITY_DIR)/unity.c
	@mkdir -p $(dir $@)
	@echo "==> Compiling Unity Core: $<"
	$(CC) $(CFLAGS) -I$(UNITY_DIR) -c $< -o $@

run-test:
	@echo "==> Running Unit Tests via Wine..."
	@which wine > /dev/null 2>&1 || (echo "ERRO: wine nao encontrado no PATH." && exit 1)
	WINEDEBUG=-all wine $(TEST_TARGET)

doc: directories
	@which doxygen > /dev/null 2>&1 || (echo "ERRO: doxygen nao instalado." && exit 1)
	@echo "==> Running Doxygen..."
	doxygen Doxyfile

clean:
	@echo "==> Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
