#!/bin/sh -e
brew install make pkg-config sdl2{,_image,_mixer,_ttf,_gfx} yaml-cpp
exec pkg/macos/build.sh
