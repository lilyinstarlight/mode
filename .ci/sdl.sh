#!/bin/bash -e
get_release() {
  { set +x; } 2>/dev/null

  local project="$1"
  local url="$2"

  echo "++ curl -sL "'"'"$url"'"' >&2
  local details version release
  details="$(curl -sL "$url" | env LC_ALL=C tr -d '[:blank:]\r\n' | perl -pe 's#^.*?<ahref="([^"]*)">'"$project"'-(\d+\.\d+\.\d+)\.tar\.gz</a>.*$#\2\t\1#i')"
  version="$(echo "$details" | cut -f1)"
  release="$(echo "$details" | cut -f2)"

  case "$release" in
    *://*)
      ;;

    *)
      case "$url" in
        */)
          release="$url$release"
          ;;

        *)
          release="$(dirname "$url")/$release"
          ;;
      esac
      ;;
  esac

  echo "Downloading $project-$version" >&2

  set -x

  curl -sL "$release" | tar xz

  { set +x; } 2>/dev/null

  echo "$project-$version"

  set -x
}

SELF="$(cd "$(dirname "$0")" &>/dev/null && pwd -P && cd - &>/dev/null)"/"$(basename "$0")"

TARGET="$1"
PREFIX="$2"

if [ -z "$TARGET" ]; then
  case "$(uname -s)" in
    Linux)
      TARGET=linux
      ;;

    Darwin)
      TARGET=macos
      ;;

    MINGW64*)
      TARGET=windows
      ;;
  esac
fi

if [ -n "$PREFIX" ]; then
  PREFIX="--prefix='$PREFIX'"
fi

case "$TARGET" in
  linux)
    SDL2_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --enable-alsa
      --disable-alsatest
      --enable-alsa-shared
      --disable-jack
      --disable-esd
      --enable-pulseaudio
      --enable-pulseaudio-shared
      --disable-arts
      --disable-nas
      --disable-sndio
      --disable-fusionsound
      --enable-libsamplerate
      --disable-libsamplerate-shared
      --enable-video-wayland
      --enable-video-wayland-qt-touch
      --enable-wayland-shared
      --enable-video-x11
      --enable-x11-shared
      --enable-video-x11-xcursor
      --enable-video-x11-xdbe
      --enable-video-x11-xinerama
      --enable-video-x11-xinput
      --enable-video-x11-xrandr
      --enable-video-x11-scrnsaver
      --enable-video-x11-xshape
      --enable-video-x11-vm
      --disable-video-directfb
      --disable-video-kmsdrm
      --enable-libudev
      --enable-dbus
      --enable-ime
      --enable-ibus
      --enable-fcitx
EOF
    )"
    SDL2_IMAGE_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --enable-bmp
      --enable-gif
      --enable-jpg
      --disable-jpg-shared
      --enable-lbm
      --enable-pcx
      --enable-png
      --disable-png-shared
      --enable-pnm
      --enable-svg
      --enable-tga
      --disable-tif
      --enable-xcf
      --enable-xpm
      --enable-xv
      --disable-webp
EOF
    )"
    SDL2_MIXER_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-sdltest
      --disable-music-cmd
      --enable-music-wave
      --disable-music-mod
      --disable-music-midi
      --enable-music-ogg
      --disable-music-ogg-tremor
      --disable-music-ogg-shared
      --disable-music-flac
      --enable-music-mp3
      --disable-music-mp3-mad-gpl
      --enable-music-mp3-mpg123
      --disable-music-mp3-mpg123-shared
      --disable-music-opus
EOF
    )"
    SDL2_TTF_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-freetypetest
      --disable-sdltest
EOF
    )"
    SDL2_GFX_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-sdltest
EOF
    )"
    ;;

  macos)
    SDL2_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-jack
      --disable-arts
      --disable-nas
      --disable-sndio
      --disable-fusionsound
      --disable-libsamplerate
      --enable-video-cocoa
      --enable-video-metal
      --enable-render-metal
      --without-x
EOF
    )"
    SDL2_IMAGE_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --enable-imageio
      --enable-bmp
      --enable-gif
      --enable-jpg
      --disable-jpg-shared
      --enable-lbm
      --enable-pcx
      --enable-png
      --disable-png-shared
      --enable-pnm
      --enable-svg
      --enable-tga
      --disable-tif
      --enable-xcf
      --enable-xpm
      --enable-xv
      --disable-webp
EOF
    )"
    SDL2_MIXER_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-sdltest
      --disable-music-cmd
      --enable-music-wave
      --disable-music-mod
      --disable-music-midi
      --enable-music-ogg
      --disable-music-ogg-tremor
      --disable-music-ogg-shared
      --disable-music-flac
      --enable-music-mp3
      --disable-music-mp3-mad-gpl
      --enable-music-mp3-mpg123
      --disable-music-mp3-mpg123-shared
      --disable-music-opus
EOF
    )"
    SDL2_TTF_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-freetypetest
      --disable-sdltest
      --without-x
EOF
    )"
    SDL2_GFX_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-sdltest
      --enable-imageio
EOF
    )"
    ;;

  windows)
    # TODO: remove the --host flag for all of these after SDL 2.0.16 is released (fix: https://github.com/libsdl-org/SDL/commit/a29fe29)
    SDL2_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --host=x86_64-w64-mingw32
      --enable-shared
      --disable-static
      --disable-jack
      --disable-arts
      --disable-nas
      --disable-sndio
      --disable-fusionsound
      --disable-libsamplerate
      --enable-directx
      --enable-wasapi
      --enable-render-d3d
EOF
    )"
    SDL2_IMAGE_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --host=x86_64-w64-mingw32
      --enable-shared
      --disable-static
      --enable-bmp
      --enable-gif
      --enable-jpg
      --disable-jpg-shared
      --enable-lbm
      --enable-pcx
      --enable-png
      --disable-png-shared
      --enable-pnm
      --enable-svg
      --enable-tga
      --disable-tif
      --enable-xcf
      --enable-xpm
      --enable-xv
      --disable-webp
EOF
    )"
    SDL2_MIXER_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --host=x86_64-w64-mingw32
      --enable-shared
      --disable-static
      --disable-sdltest
      --disable-music-cmd
      --enable-music-wave
      --disable-music-mod
      --disable-music-midi
      --enable-music-ogg
      --disable-music-ogg-tremor
      --disable-music-ogg-shared
      --disable-music-flac
      --enable-music-mp3
      --disable-music-mp3-mad-gpl
      --enable-music-mp3-mpg123
      --disable-music-mp3-mpg123-shared
      --disable-music-opus
EOF
    )"
    SDL2_TTF_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --host=x86_64-w64-mingw32
      --enable-shared
      --disable-static
      --disable-freetypetest
      --disable-sdltest
EOF
    )"
    SDL2_GFX_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --host=x86_64-w64-mingw32
      --enable-shared
      --disable-static
      --disable-sdltest
EOF
    )"
    ;;

  *)
    SDL2_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-jack
      --disable-esd
      --disable-arts
      --disable-nas
      --disable-sndio
      --disable-fusionsound
      --enable-libsamplerate
      --disable-libsamplerate-shared
EOF
    )"
    SDL2_IMAGE_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --enable-bmp
      --enable-gif
      --enable-jpg
      --disable-jpg-shared
      --enable-lbm
      --enable-pcx
      --enable-png
      --disable-png-shared
      --enable-pnm
      --enable-svg
      --enable-tga
      --disable-tif
      --enable-xcf
      --enable-xpm
      --enable-xv
      --disable-webp
EOF
    )"
    SDL2_MIXER_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-sdltest
      --disable-music-cmd
      --enable-music-wave
      --disable-music-mod
      --disable-music-midi
      --enable-music-ogg
      --disable-music-ogg-tremor
      --disable-music-ogg-shared
      --disable-music-flac
      --enable-music-mp3
      --disable-music-mp3-mad-gpl
      --enable-music-mp3-mpg123
      --disable-music-mp3-mpg123-shared
      --disable-music-opus
EOF
    )"
    SDL2_TTF_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-freetypetest
      --disable-sdltest
EOF
    )"
    SDL2_GFX_CONF_FLAGS="$(tr -d ' ' <<EOF | tr '\n' ' ' | xargs
      $PREFIX
      --enable-shared
      --disable-static
      --disable-sdltest
EOF
    )"
    ;;
esac

BUILD_DIR="$(dirname "$SELF")"/build

set -x

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

pushd "$BUILD_DIR" >/dev/null

pushd "$(get_release SDL2 'https://libsdl.org/download-2.0.php')" >/dev/null
./autogen.sh
# shellcheck disable=SC2086
./configure $SDL2_CONF_FLAGS
make
make install
popd >/dev/null

pushd "$(get_release SDL2_image 'https://libsdl.org/projects/SDL_image/')" >/dev/null
./autogen.sh
# shellcheck disable=SC2086
./configure $SDL2_IMAGE_CONF_FLAGS
make
make install
popd >/dev/null

pushd "$(get_release SDL2_mixer 'https://libsdl.org/projects/SDL_mixer/')" >/dev/null
./autogen.sh
# shellcheck disable=SC2086
./configure $SDL2_MIXER_CONF_FLAGS
make
make install
popd >/dev/null

pushd "$(get_release SDL2_ttf 'https://libsdl.org/projects/SDL_ttf/')" >/dev/null
./autogen.sh
# shellcheck disable=SC2086
./configure $SDL2_TTF_CONF_FLAGS
make
make install
popd >/dev/null

pushd "$(get_release SDL2_gfx 'https://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/')" >/dev/null
./autogen.sh
# shellcheck disable=SC2086
./configure $SDL2_GFX_CONF_FLAGS
make
make install
popd >/dev/null

popd >/dev/null

{ set +x; } 2>/dev/null
