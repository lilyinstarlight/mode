MACHINE=$(shell $(CC) -dumpmachine)


ifneq ($(findstring linux,$(MACHINE)),)

#############
### Linux ###
#############

ifeq ($(DEBUG),1)

CXXFLAGS?=-std=c++17 -fsanitize=address -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17 -fsanitize=address
LDLIBS?=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

else

CXXFLAGS?=-std=c++17 -O2 -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17 -O2
LDLIBS?=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

endif

LUA_CFLAGS?=-std=c11 -DLUA_USE_LINUX
LUA_LDFLAGS?=
LUA_LDLIBS?=-ldl -lreadline -lhistory -lncurses


else ifneq ($(findstring darwin,$(MACHINE)),)

#############
### macOS ###
#############

ifeq ($(DEBUG),1)

CXXFLAGS?=-std=c++17 -fsanitize=address,undefined -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17 -fsanitize=address,undefined
LDLIBS?=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

else

CXXFLAGS?=-std=c++17 -O2 -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17 -O2
LDLIBS?=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

endif

LUA_CFLAGS?=-std=c11 -DLUA_USE_MACOSX
LUA_LDFLAGS?=
LUA_LDLIBS?=-ldl -lreadline -lncurses


else ifneq ($(findstring mingw,$(MACHINE)),)

###########################
### Windows (MinGW-w64) ###
###########################

ifeq ($(DEBUG),1)

CXXFLAGS?=-std=c++17 -O2 -mwindows -ggdb -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17 -O2 -mwindows
LDLIBS?=-lm `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

else

CXXFLAGS?=-std=c++17 -O2 -mwindows -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17 -O2 -mwindows
LDLIBS?=-lm `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

endif

LUA_CFLAGS?=-std=c11 -DLUA_USE_WINDOWS
LUA_LDFLAGS?=
LUA_LDLIBS?=-lreadline -lhistory -lncurses


else

###############
### Unknown ###
###############

ifeq ($(DEBUG),1)

CXXFLAGS?=-std=c++17 -Wall -Wpedantic -Wextra -Weffc++ -Werror -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17
LDLIBS?=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

else

CXXFLAGS?=-std=c++17 -DNDEBUG -Wall -Wpedantic -Wextra -Weffc++ -isystem $(VENDORDIR) -isystem $(VENDORDIR)/lua -isystem $(VENDORDIR)/sol3/single/include `sdl2-config --cflags` -I `sdl2-config --prefix`/include/ `pkg-config --cflags yaml-cpp`
LDFLAGS?=-std=c++17
LDLIBS?=-lm -ldl `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_gfx `pkg-config --libs yaml-cpp`

endif

LUA_CFLAGS?=-std=c11
LUA_LDFLAGS?=
LUA_LDLIBS?=-ldl -lreadline -lhistory -lncurses

endif
