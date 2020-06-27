MACHINE=$(shell $(CC) -dumpmachine)


ifneq ($(findstring linux,$(MACHINE)),)

#############
### Linux ###
#############

CC=gcc
CXX=g++

ifeq ($(DEBUG),1)

CXXFLAGS=-std=c++17 -fvisibility=hidden -fsanitize=address -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -fsanitize=address -ggdb
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden -ggdb

LUA_CFLAGS=-std=c11 -fvisibility=hidden -ggdb -DLUA_USE_LINUX -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -ggdb
LUA_LDLIBS=-ldl -lreadline -lhistory -lncurses

else

CXXFLAGS=-std=c++17 -fvisibility=hidden -flto -O2 -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -flto -O2
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden -flto -O2

LUA_CFLAGS=-std=c11 -fvisibility=hidden -flto -O2 -DLUA_USE_LINUX -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -flto -O2
LUA_LDLIBS=-ldl -lreadline -lhistory -lncurses

endif


else ifneq ($(findstring darwin,$(MACHINE)),)

#############
### macOS ###
#############

CC=clang
CXX=clang++

ifeq ($(DEBUG),1)

CXXFLAGS=-std=c++17 -fvisibility=hidden -fsanitize=address,undefined -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -fsanitize=address,undefined -ggdb
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden -ggdb

LUA_CFLAGS=-std=c11 -fvisibility=hidden -ggdb -DLUA_USE_MACOSX -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -ggdb
LUA_LDLIBS=-ldl -lreadline -lncurses

else

CXXFLAGS=-std=c++17 -fvisibility=hidden -flto -O2 -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -flto -O2
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden -flto -O2

LUA_CFLAGS=-std=c11 -fvisibility=hidden -flto -O2 -DLUA_USE_MACOSX -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -flto -O2
LUA_LDLIBS=-ldl -lreadline -lncurses

endif


else ifneq ($(findstring mingw,$(MACHINE)),)

###########################
### Windows (MinGW-w64) ###
###########################

CC=gcc
CXX=g++

ifeq ($(DEBUG),1)

CXXFLAGS=-std=c++17 -fvisibility=hidden -ggdb -mwindows -Wall -Wpedantic -Wextra -Weffc++ -Werror -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -ggdb -mwindows
LDLIBS=-lm `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-ggdb

LUA_CFLAGS=-std=c11 -fvisibility=hidden -ggdb -DLUA_USE_WINDOWS -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -ggdb
LUA_LDLIBS=-lreadline -lhistory -lncurses

else

CXXFLAGS=-std=c++17 -fvisibility=hidden -flto -O2 -mwindows -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -flto -O2 -mwindows
LDLIBS=-lm `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden -flto -O2

LUA_CFLAGS=-std=c11 -fvisibility=hidden -flto -O2 -DLUA_USE_WINDOWS -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -flto -O2
LUA_LDLIBS=-lreadline -lhistory -lncurses

endif


else

###############
### Unknown ###
###############

CC=cc
CXX=c++

ifeq ($(DEBUG),1)

CXXFLAGS=-std=c++17 -fvisibility=hidden -g -Wall -Wpedantic -Wextra -Weffc++ -Werror -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden -g
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden -g

LUA_CFLAGS=-std=c11 -fvisibility=hidden -g -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden -g
LUA_LDLIBS=-ldl -lreadline -lhistory -lncurses

else

CXXFLAGS=-std=c++17 -fvisibility=hidden -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -I $(VENDORDIR) -I $(VENDORDIR)/yaml-cpp/include -I $(VENDORDIR)/lua -I $(VENDORDIR)/sol3/single/include `sdl2-config --cflags`
LDFLAGS=-std=c++17 -fvisibility=hidden
LDLIBS=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx

YAML_CPP_CXX_STANDARD=17
YAML_CPP_CXX_FLAGS=-fvisibility=hidden

LUA_CFLAGS=-std=c11 -fvisibility=hidden -Wall
LUA_LDFLAGS=-std=c11 -fvisibility=hidden
LUA_LDLIBS=-ldl -lreadline -lhistory -lncurses

endif

endif
