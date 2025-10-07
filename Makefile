# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2

# Nombre del ejecutable
TARGET = Practica2

# Carpeta donde van los objetos
OBJDIR = build

# Todos los archivos .cc
SRCS = $(wildcard *.cc)

# Lista de objetos en la carpeta build
OBJS = $(patsubst %.cc,$(OBJDIR)/%.o,$(SRCS))

# Regla principal
all: $(TARGET)

# Cómo generar el ejecutable a partir de los objetos
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar cada .cc en build/*.o
$(OBJDIR)/%.o: %.cc | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear la carpeta build si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Limpiar todo
clean:
	rm -rf $(OBJDIR) $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild
