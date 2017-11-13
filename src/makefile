EXE=mode

CXX=g++

CXXFLAGS=-std=c++11 -ggdb -fsanitize=address -Wall -Wpedantic -Wextra -Weffc++ -Werror `sdl2-config --cflags` -isystem lib -I `sdl2-config --prefix`/include/
LDLIBS=-lm -lexpat `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer lib/lua/liblua.a

SRC!=ls *.cpp

all: $(EXE)

$(EXE): lib/lua/liblua.a $(SRC:.cpp=.o)

lib/lua/liblua.a:
	make -C lib/lua a

clean:
	rm -rf lib/lua/{*.o,*.a}
	rm -rf *.o
	rm -f $(EXE)

.PHONY: all clean
