#!/bin/sh -e
sudo apt install libsdl2{,-image,-mixer,-ttf,-gfx}-dev libyaml-cpp-dev
exec pkg/linux/build.sh
