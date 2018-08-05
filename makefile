EXE=mode

CC=gcc
CXX=g++

CXXFLAGS=-std=c++14 -fsanitize=address -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(LIBDIR) -isystem $(LIBDIR)/lua -isystem $(LIBDIR)/sol2 `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS=-std=c++14 -fsanitize=address
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

SRCDIR=src
LIBDIR=lib
BUILDDIR=build

SRC!=find $(SRCDIR) -maxdepth 1 -name "*.cpp" -printf "%f "
DEP!=echo $(SRC) | sed -e "s/\([^ ]*\).cpp/$(BUILDDIR)\/\1.d /g"
OBJ!=echo $(SRC) | sed -e "s/\([^ ]*\).cpp/$(BUILDDIR)\/\1.o /g"

$(BUILDDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MQ $(@:.d=.o) -MF $@ -MM $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(EXE): $(OBJ) $(LIBDIR)/lua/liblua.a
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(LIBDIR)/lua/liblua.a: $(LIBDIR)/lua/makefile
	+make -C $(LIBDIR)/lua CC=$(CC) a

all: $(DEP) $(EXE)

clean:
	$(RM) $(LIBDIR)/lua/{*.d,*.o,*.a,lua}
	$(RM) $(BUILDDIR)/{*.d,*.o,*.a} $(EXE)

.PHONY: all clean

-include $(DEP)
