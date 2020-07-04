#!/bin/bash -e
start_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::group::$(echo -e "$*" | sed -e '1h;2,$H;$!d;g' -e 's/%/%25/g' -e "s/$(printf '\r')/%0D/g" -e 's/\n/%0A/g')" >&2
  fi
}

end_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::endgroup::" >&2
  fi
}


start_group 'Update system'
set -x

brew upgrade

{ set +x; } 2>/dev/null
end_group

start_group 'Install dependencies'
set -x

brew install autoconf automake make libsamplerate libogg libvorbis mpg123 freetype

.ci/sdl.sh macos

{ set +x; } 2>/dev/null
end_group

exec pkg/macos/build.sh
