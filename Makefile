# ==============================================================================
# Cross-Compilation Makefile (Fedora Linux -> Win32 x86_64)
# ==============================================================================

CC         := x86_64-w64-mingw32-gcc
CFLAGS     := -Wall -Wextra -std=c11 -DUNICODE -D_UNICODE -O2
INCLUDES   := -Isrc/include
LDFLAGS    := -ladvapi32

BUILD_DIR  := build
SRC_DIR    := src
TEST_DIR   := tests
UNITY_DIR  := $(TEST_DIR)/unity

TARGET     := $(BUILD_DIR)/PersistenceSim.exe
TEST_TARGET:= $(BUILD_DIR)/test_runner.exe

# Coleta de fontes excluindo testes da compilação principal
SRCS       := $(shell find $(SRC_DIR) -maxdepth 1 -name '*.c')
OBJS       := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/obj/%.o, $(SRCS))

# Fontes dos testes unitários
TEST_SRCS  := $(wildcard $(TEST_DIR)/*.c) $(UNITY_DIR)/unity.c
TEST_OBJS  := $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/test_obj/%.o, $(wildcard $(TEST_DIR)/*.c)) \
              $(BUILD_DIR)/test_obj/unity.o
# Objetos do src necessários nos testes (excluindo main.o)
SRC_NO_MAIN:= $(filter-out $(BUILD_DIR)/obj/main.o, $(OBJS))

.PHONY: all clean test doc run-test directories

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)/obj $(BUILD_DIR)/test_obj $(BUILD_DIR)/doc

# Linkagem do binário final
$(TARGET): $(OBJS)
	@echo "==> Linking Target: $@"
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compilação dos objetos do src
$(BUILD_DIR)/obj/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "==> Compiling: $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Regra de geração e execução de testes com Unity
test: directories $(TEST_TARGET) run-test

$(TEST_TARGET): $(SRC_NO_MAIN) $(TEST_OBJS)
	@echo "==> Linking Test Runner: $@"
	$(CC) $(CFLAGS) $(INCLUDES) -I$(UNITY_DIR) $^ -o $@ $(LDFLAGS)

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
	@which wine > /dev/null 2>&1 || (echo "ERRO: 'wine' não encontrado no PATH para executar o binário Win32." && exit 1)
	wine $(TEST_TARGET)

# Regra para gerar documentação
doc: directories
	@which doxygen > /dev/null 2>&1 || (echo "ERRO: 'doxygen' não instalado." && exit 1)
	@if [ -f Doxyfile ]; then \
		echo "==> Running Doxygen..."; \
		doxygen Doxyfile; \
	else \
		echo "Aviso: Doxyfile não encontrado. Gerando template padrão em $(BUILD_DIR)/doc..."; \
		doxygen -g $(BUILD_DIR)/doc/Doxyfile && doxygen $(BUILD_DIR)/doc/Doxyfile; \
	fi

clean:
	@echo "==> Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)
