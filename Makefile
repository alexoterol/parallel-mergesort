# ====================================
# Variables de configuración
# ====================================

# Compilador C
CC = gcc

# Banderas de compilación
CFLAGS = -Wall -Wextra -g -pthread -std=c99 -D_POSIX_C_SOURCE=200112L


# Librerías a enlazar
LDLIBS = -lm -pthread

# Nombre del ejecutable
TARGET = concurrent_mergesort

# Archivos fuente
SOURCES = main.c sort_utils.c

# Archivos objeto (creados a partir de los fuentes)
OBJECTS = $(SOURCES:.c=.o)

# ====================================
# Reglas Principales
# ====================================

# Regla por defecto: construye el ejecutable
all: $(TARGET)

# Regla para enlazar el ejecutable
$(TARGET): $(OBJECTS)
	@echo "Enlazando $^ para crear $@..."
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	@echo "¡Compilación completa! Ejecutable: ./$(TARGET)"

# Regla para compilar archivos .c a .o
%.o: %.c sort_utils.h
	@echo "Compilando $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para ejecutar el programa: depende de 'all'
run: all
	@echo "--- Ejecutando $(TARGET) ---"
	./$(TARGET)

# Regla de alias para 'run'
test: run

# Regla de limpieza: remueve el ejecutable y los archivos objeto
clean:
	@echo "Limpiando archivos objeto y el ejecutable..."
	rm -f $(OBJECTS) $(TARGET)

# ====================================
# Reglas Falsas (Phony Targets)
# ====================================
.PHONY: all clean run test