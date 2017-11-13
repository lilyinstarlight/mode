CC=g++
CXX=g++

CXXFLAGS=-std=c++11 -ggdb -fsanitize=address -Wall -Wpedantic -Wextra -Weffc++ -Werror `sdl2-config --cflags` -isystem lib -I `sdl2-config --prefix`/include/
LDFLAGS=-fsanitize=address
LDLIBS=-lm -lexpat `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRC!=ls *.cpp

all: main

main: $(SRC:.cpp=.o) lib/lua/liblua.a

lib/lua/liblua.a: lib/lua/makefile
	make -C lib/lua a

clean:
	rm -rf lib/lua/{*.o,*.a,lua}
	rm -rf *.o
	rm -f main

.PHONY: all clean
