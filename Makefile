
CXX = g++

CFLAGS = -O0 -Wall -std=c++20
CFLAGS += -Ilib/glad/include -Ilib/glfw/include -Ilib/stb
LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

default: rhubarb run

run:
	./rhubarb

libs:
	cd lib/glad && $(CXX) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/glfw && cmake . && make

rhubarb: $(OBJ)
	$(CXX) -o rhubarb $^ $(LDFLAGS) 

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ) rhubarb

