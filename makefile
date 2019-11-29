EXE=mode

SRCDIR=src
LIBDIR=vendor
BUILDDIR=build
DATADIR=data
DISTDIR=dist

DIST:=behaviours data fonts libs sounds specs textures $(EXE)

CC=gcc
CXX=g++

CXXFLAGS=-std=c++14 -fsanitize=address -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(LIBDIR) -isystem $(LIBDIR)/lua -isystem $(LIBDIR)/sol2 `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS=-std=c++14 -fsanitize=address
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

SRC!=find $(SRCDIR) -maxdepth 1 -name "*.cpp" -printf "%f "
DEP!=echo $(SRC) | sed -e "s/\([^ ]*\).cpp/$(BUILDDIR)\/\1.d /g"
OBJ!=echo $(SRC) | sed -e "s/\([^ ]*\).cpp/$(BUILDDIR)\/\1.o /g"

$(BUILDDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MQ $(@:.d=.o) -MF $@ -MM $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(DEP) $(EXE) $(DATADIR)

$(EXE): $(OBJ) $(LIBDIR)/lua/liblua.a
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(LIBDIR)/lua/liblua.a: $(LIBDIR)/lua/makefile
	+make -C $(LIBDIR)/lua CC=$(CC) a

$(DATADIR):
	mkdir -p $@

clean:
	$(RM) $(LIBDIR)/lua/{*.d,*.o,*.a,lua}
	$(RM) $(BUILDDIR)/{*.d,*.o,*.a} $(EXE)

dist: all
	mkdir -p $(DISTDIR)
	cp -a $(DIST) $(DISTDIR)/
	cd $(DISTDIR) && tar czf ../$(EXE).tar.gz $(DIST)

.PHONY: all clean dist

-include $(DEP)
