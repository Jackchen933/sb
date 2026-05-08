# PC Simulator Makefile for OV_Watch

# Compiler
CC = gcc
SDL_CONFIG = sdl2-config

# Flags
CFLAGS = -Wall -O2 $(shell $(SDL_CONFIG) --cflags) -I../Middlewares/LVGL/GUI -I../Middlewares/LVGL/GUI/lvgl/src -I../Middlewares/LVGL/GUI/lvgl/examples -I../Middlewares/LVGL/GUI/lvgl/demos -I../User -I../User/GUI_App -I../User/GUI_App/Screens/Inc -I../User/Func/Inc -I../User/GUI_App/Fonts -I../BSP/BL24C02 -I../BSP/POWER -I../BSP/AHT21 -I../BSP/MPU6050 -DLV_CONF_INCLUDE_SIMPLE
LDFLAGS = $(shell $(SDL_CONFIG) --libs) -lm

# Source files
SRC = main_pc.c

# Output
TARGET = OV_Watch_Simulator.exe

all:
	@echo "OV_Watch PC Simulator Makefile"
	@echo ""
	@echo "Before building, please install SDL2:"
	@echo "  Windows: Download SDL2-devel from https://github.com/libsdl-org/SDL/releases"
	@echo "  Linux:   sudo apt install libsdl2-dev"
	@echo "  macOS:   brew install sdl2"
	@echo ""
	@echo "Then run: make build"

build: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all build clean run
