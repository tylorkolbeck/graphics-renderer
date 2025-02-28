IMGUI_DIR = libs/imgui

IMGUI_CORE = $(IMGUI_DIR)/imgui.cpp \
             $(IMGUI_DIR)/imgui_draw.cpp \
             $(IMGUI_DIR)/imgui_widgets.cpp \
             $(IMGUI_DIR)/imgui_tables.cpp \
             $(IMGUI_DIR)/imgui_demo.cpp  # Optional

IMGUI_BACKEND = $(IMGUI_DIR)/backends/imgui_impl_sdl2.cpp \
                $(IMGUI_DIR)/backends/imgui_impl_sdlrenderer2.cpp			 

IMGUI_SRC = $(IMGUI_CORE) $(IMGUI_BACKEND)
CXX = g++
CXXFLAGS = -Wall -std=c++17 -g -Iinclude -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends $(shell sdl2-config --cflags)
LDFLAGS = -lSDL2
TARGET = 3drenderer

SRC_DIR = src
OBJ_DIR = bin/obj
BIN_DIR = bin


SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(IMGUI_SRC)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN_DIR)/$(TARGET)
	./$(BIN_DIR)/$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/$(TARGET)

.PHONY: all run clean