#!/bin/sh -e
brew upgrade
brew install make pkg-config sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx yaml-cpp
exec pkg/macos/build.sh
