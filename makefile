# Compilador
CC = gcc

# Nome do executável
TARGET = bin/snake

# Pastas
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -I$(INC_DIR) -g


# Bibliotecas da raylib
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Caso o pkg-config não funcione, substitua a linha acima por:
# LIBS = -lraylib -lm -lpthread -ldl -lrt -lX11

# Todos os arquivos .c
SRC = $(wildcard $(SRC_DIR)/*.c)

# Todos os arquivos .o
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

# -------------------------

all: $(TARGET)

# Linkagem
$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Compilação
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Executar
run: all
	./$(TARGET)

# Limpar arquivos compilados
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Recompilar tudo
rebuild: clean all

.PHONY: all run clean rebuild

# Executar no GDB
debug: all
	gdb ./$(TARGET)
