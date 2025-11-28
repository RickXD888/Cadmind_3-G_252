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

# Test executable (single source)
TEST_SRC := $(SRC_DIR)/00_Ventana.cpp
TEST_EXE := $(BIN_DIR)/00_Ventana.exe

# CardMind: all sources except the test
CARD_SOURCES := $(filter-out $(TEST_SRC),$(CPP_FILES))
CARD_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CARD_SOURCES))
CARD_EXE := $(BIN_DIR)/CardMind.exe

all: $(TEST_EXE) $(CARD_EXE)

$(BIN_DIR):
	$(MKDIR_P) $(BIN_DIR)

# Build test exe from its single source (no object files)
$(TEST_EXE): $(TEST_SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ $(SFML)

# Compile objects for CardMind
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link CardMind from its objects
$(CARD_EXE): $(CARD_OBJS)
	$(CXX) $^ -o $@ $(SFML)

# Run targets
run-test: $(TEST_EXE)
	./$(TEST_EXE)

run-cardmind: $(CARD_EXE)
	./$(CARD_EXE)

clean:
	rm -f $(BIN_DIR)/*.o $(TEST_EXE) $(CARD_EXE)

.PHONY: all clean run-test run-cardmind runCardMind run00Ventana actualizar actualizar-force

# Aliases (conveniencia): soportan la notación que quizás intentaste usar
runCardMind: run-cardmind

run00Ventana: run-test

# Actualizar: reconstruye el ejecutable principal si hay cambios
actualizar: $(CARD_EXE)

# Actualizar forzado: limpia y recompila todo
actualizar-force:
	$(MAKE) clean
	$(MAKE) all