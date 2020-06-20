#!/bin/bash -e
start_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::group::$(echo -e "$*" | sed -e '1h;2,$H;$!d;g' -e 's/%/%25/g' -e 's/\r/%0D/g' -e 's/\n/%0A/g')"
  fi
}

end_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::endgroup"
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

sudo apt-get install -y make pkg-config gcc g++ libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev libsdl2-gfx-dev libyaml-cpp-dev

{ set +x; } 2>/dev/null
end_group

exec pkg/linux/build.sh
