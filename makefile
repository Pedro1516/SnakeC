# ==========================================================
# Snake Game - Makefile
# ==========================================================

# Compilador
CC := gcc

# Executável
TARGET := bin/snake

# Diretórios
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Ferramenta para localizar bibliotecas
PKG_CONFIG ?= pkg-config

# ----------------------------------------------------------
# Configuração de compilação
# ----------------------------------------------------------

# Modo de compilação:
# make            -> Debug
# make BUILD=release -> Release
BUILD ?= debug

# Flags do pré-processador
CPPFLAGS := -I$(INC_DIR)
CPPFLAGS += $(shell $(PKG_CONFIG) --cflags raylib)

# Flags comuns
CFLAGS := \
	-Wall \
	-Wextra \
	-Wpedantic \
	-std=c99

# Flags específicas
ifeq ($(BUILD),release)
	CFLAGS += -O2
else
	CFLAGS += -g
endif

# Bibliotecas
LDLIBS := $(shell $(PKG_CONFIG) --libs raylib)
LDLIBS += -lm

# ----------------------------------------------------------
# Arquivos
# ----------------------------------------------------------

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# ==========================================================
# Regras
# ==========================================================

all: $(TARGET)

# Linkagem
$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

# Compilação
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# ==========================================================
# Utilidades
# ==========================================================

run: all
	./$(TARGET)

debug: all
	gdb ./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) bin

rebuild: clean all

release:
	$(MAKE) BUILD=release rebuild

# ==========================================================

.PHONY: all run debug clean rebuild release