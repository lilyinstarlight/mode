#!/bin/bash -e
start_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    # shellcheck disable=SC2016
    echo "::group::$(echo -e "$*" | sed -e '1h;2,$H;$!d;g' -e 's/%/%25/g' -e 's/\r/%0D/g' -e 's/\n/%0A/g')" >&2
  fi
}

end_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::endgroup::" >&2
  fi
}


start_group 'Update system'
set -x

sudo apt-get update -y
sudo apt-get full-upgrade -y

{ set +x; } 2>/dev/null
end_group

start_group 'Install dependencies'
set -x

sudo apt-get install -y autoconf automake make gcc g++ imagemagick fcitx-libs-dev libasound2-dev libdbus-1-dev libegl1-mesa-dev libgl1-mesa-dev libgles2-mesa-dev libglu1-mesa-dev libibus-1.0-dev libpulse-dev libsamplerate0-dev libudev-dev libvulkan-dev libwayland-dev libx11-dev libxcursor-dev libxext-dev libxi-dev libxinerama-dev libxkbcommon-dev libxrandr-dev libxss-dev libxt-dev libxv-dev libxxf86vm-dev libjpeg-turbo8-dev libpng-dev libogg-dev libvorbis-dev libmpg123-dev libfreetype-dev

sudo .ci/sdl.sh linux

{ set +x; } 2>/dev/null
end_group

exec pkg/linux/build.sh
