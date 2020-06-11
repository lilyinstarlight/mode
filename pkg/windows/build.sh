#!/bin/bash -xe
SELF="$(readlink -f "$0")"
SRC_DIR="$(dirname $(dirname "$(dirname "$SELF")"))"

source "$(dirname "$(dirname "$SELF")")"/metadata.sh

BUILD_DIR="$(dirname "$SELF")"/build
IMAGE_DIR="$(dirname "$SELF")"/image


# prepare work directories
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
rm -rf "$IMAGE_DIR"
mkdir -p "$IMAGE_DIR"


# copy source
mkdir -p "$BUILD_DIR"/"$NAME"

for SRC_FILE in "$SRC_DIR"/*; do
  if [ "$(basename "$SRC_FILE")" == pkg ] || [ "$(basename "$SRC_FILE")" == build ] || [ "$(basename "$SRC_FILE")" == data ] || [ "$(basename "$SRC_FILE")" == dist ]; then
    continue
  fi

  cp -r "$SRC_FILE" "$BUILD_DIR"/"$NAME"/
done


# build dist directory without debug and defined relative resource
pushd "$BUILD_DIR"/"$NAME"
make distclean
make dist DEBUG=0 RESOURCE=..
EXE="$(find dist -mindepth 1 -maxdepth 1 -type f -name '*.exe' -printf '%f\n' | head -n1)"
popd


# prepare image for packaging
mkdir -p "$IMAGE_DIR"/bin

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$(basename "$DIST_FILE")" == "$EXE" ]; then
    continue
  fi

  cp -r "$DIST_FILE" "$IMAGE_DIR"/
done

cp "$BUILD_DIR"/"$NAME"/dist/"$EXE" "$IMAGE_DIR"/bin/"$EXE"

for lib in $(ldd "$IMAGE_DIR"/bin/"$EXE" | grep -E '=>\s*/' | awk '{ print $3 }' | sed -e 's#^/##' | grep '^mingw64/'); do
  cp /"$lib" "$IMAGE_DIR"/bin/"$(basename "$lib")"
done

cat >"$IMAGE_DIR"/"$NAME".bat <<EOF
@start ".:mode:." /D "%~dp0" "%~dp0\\bin\\$EXE"
EOF


# package image
rm -f "$NAME".zip

pushd "$IMAGE_DIR"
zip -r "$(dirs -l +1)"/"$NAME".zip ./*
popd
