# Variables con las rutas
SRC_DIR = ./src
INC_DIR = ./inc
OUT_DIR = ./build
OBJ_DIR = $(OUT_DIR)/obj
BIN_DIR = $(OUT_DIR)/bin
# Archivos fuente y objeto
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
# Incluir archivos de dependencia
-include $(OBJ_DIR)/*.d

# Evitar conflictos con directorios
.PHONY: all clean info

# Objetivo predeterminado
all: $(OBJ_FILES)
	@echo "Enlazando archivos objetos para crear el ejecutable"
	@mkdir -p $(BIN_DIR)
	@gcc $(OBJ_FILES) -o $(BIN_DIR)/app.out

# Compilar archivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilando $< to $@"
	@mkdir -p $(OBJ_DIR)
		gcc -o $@ -c $< $(foreach DIR, $(INC_DIR), -I $(DIR)) -MMD

# Limpiar
clean:
	@rm -rf $(OUT_DIR)

# Información
info:
	@echo "OBJ_FILES = $(OBJ_FILES)"

# Generar documentación con Doxygen
doc:
	doxygen doxyfile
