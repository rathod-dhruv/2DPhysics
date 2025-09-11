# Cross-platform Makefile for SDL2 (macOS, Linux, Windows via MSYS2 MinGW64)

CXX      := g++
CXXFLAGS := -std=c++17 -Wall

# Expand source files with make's wildcard (works cross-platform)
SRC := $(wildcard src/*.cpp) $(wildcard src/Physics/*.cpp)

# Detect platform
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Darwin)  # macOS
    # Use sdl2-config on macOS (Homebrew)
    CXXFLAGS += $(shell sdl2-config --cflags)
    LDLIBS   := $(shell sdl2-config --libs) \
                -lSDL2_image -lSDL2_gfx
else                        # Linux or MSYS2 MinGW64
    # Use pkg-config everywhere else
    CXXFLAGS += $(shell pkg-config --cflags sdl2 SDL2_image SDL2_gfx)
    LDLIBS   := $(shell pkg-config --libs   sdl2 SDL2_image SDL2_gfx)
endif

# Targets
all: app
build: app

app: $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: clean
clean:
	rm -f app
