#!/bin/sh -e
sudo apt-get update -y
sudo apt-get full-upgrade -y
sudo apt-get install -y libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev libyaml-cpp-dev
exec pkg/linux/build.sh
