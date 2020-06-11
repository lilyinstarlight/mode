#!/bin/sh -e
brew install sdl2{,_image,_mixer,_ttf,_gfx} yaml-cpp
exec pkg/macos/build.sh
