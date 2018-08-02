CC=clang
CXX=clang++

CXXFLAGS=-std=c++14 -fsanitize=address -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(LIBDIR) -isystem $(LIBDIR)/lua -isystem $(LIBDIR)/sol2 `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS=-std=c++14 -fsanitize=address
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

ROOT=..

SRCDIR=$(ROOT)/src
LIBDIR=$(ROOT)/lib

SRC!=find $(SRCDIR) -maxdepth 1 -name "*.cpp" -printf "%f "
DEP=$(SRC:.cpp=.d)
OBJ=$(SRC:.cpp=.o)

VPATH=$(SRCDIR)

%.d: %.cpp
	$(CXX) $(CXXFLAGS) -MF $@ -MM $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.a: %.o
	$(AR) rcs $@ $^

%: %.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

all: $(DEP) main

main: $(OBJ) $(LIBDIR)/lua/liblua.a

$(LIBDIR)/lua/liblua.a: $(LIBDIR)/lua/makefile
	+make -C $(LIBDIR)/lua CC=$(CC) a

clean:
	$(RM) $(LIBDIR)/lua/{*.d,*.o,*.a,lua}
	$(RM) *.d *.o *.a main

.PHONY: all clean

-include $(DEP)
