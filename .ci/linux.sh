#!/bin/sh -e
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev libyaml-cpp-dev
exec pkg/linux/build.sh
