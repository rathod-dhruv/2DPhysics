CXX = g++
CXXFLAGS = -std=c++17 -Wall
INCLUDES = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_gfx

build:
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./src/*.cpp ./src/Physics/*.cpp $(LIBS) -o app

run:
	./app

clean:
	rm -f app