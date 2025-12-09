### Build setup: two executables
# - `bin/00_Ventana.exe` built ONLY from `src/00_Ventana.cpp` (test)
# - `bin/CardMind.exe` built from all other sources (includes `main.cpp`)

SRC_DIR := src
BIN_DIR := bin
INC_DIR := include

SFML := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lbox2d -lsfml-main

CXX := g++
CXXFLAGS := -I$(INC_DIR) -std=c++17 -O2
MKDIR_P := mkdir -p

# All .cpp in src
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)

# CardMind: todos los fuentes en src
CARD_SOURCES := $(CPP_FILES)
CARD_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CARD_SOURCES))
CARD_EXE := $(BIN_DIR)/CardMind.exe

all: $(CARD_EXE)

$(BIN_DIR):
	$(MKDIR_P) $(BIN_DIR)

# Compile objects for CardMind
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link CardMind from its objects
$(CARD_EXE): $(CARD_OBJS)
	$(CXX) $^ -o $@ $(SFML)

# Run targets
run-cardmind: $(CARD_EXE)
	./$(CARD_EXE)

clean:
	rm -f $(BIN_DIR)/*.o $(CARD_EXE)


.PHONY: all clean run-cardmind runCardMind actualizar actualizar-force

# Aliases (conveniencia): soportan la notación que quizás intentaste usar
run: run-cardmind

# Actualizar: reconstruye el ejecutable principal si hay cambios
actualizar: $(CARD_EXE)

# Actualizar forzado: limpia y recompila todo
actualizar-force:
	$(MAKE) clean
	$(MAKE) all