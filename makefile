EXE=mode

CXXFLAGS=-std=c++11 -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror `sdl2-config --cflags` -I `sdl2-config --prefix`/include/
LDLIBS=-lm -lexpat `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SRC!=ls *.cpp

all: $(EXE)

$(EXE): $(SRC:.cpp=.o)

clean:
	rm -rf *.o
	rm -f $(EXE)

.PHONY: all clean
