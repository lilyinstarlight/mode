#!/bin/bash -e
start_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::group::$(echo -e "$*" | sed -e '1h;2,$H;$!d;g' -e 's/%/%25/g' -e 's/\r/%0D/g' -e 's/\n/%0A/g')"
  fi
}

end_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::endgroup::"
  fi
}


start_group 'Update system'
set -x

brew upgrade

{ set +x; } 2>/dev/null
end_group

start_group 'Install dependencies'
set -x

brew install make pkg-config sdl2 sdl2_image sdl2_mixer sdl2_ttf sdl2_gfx yaml-cpp

{ set +x; } 2>/dev/null
end_group

exec pkg/macos/build.sh
