#!/bin/bash -xe
SELF="$(readlink -f "$0")"
SRC_DIR="$(dirname $(dirname "$(dirname "$SELF")"))"

source "$(dirname "$(dirname "$SELF")")"/metadata.sh

BIN_DIR=bin
TMP_DIR=tmp
BUILD_DIR=build
IMG_DIR=img

rm -rf "$BIN_DIR"
mkdir -p "$BIN_DIR"
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
rm -rf "$IMG_DIR"
mkdir -p "$IMG_DIR"

cp -r "$SRC_DIR" "$TMP_DIR"/"$NAME"

pushd "$TMP_DIR"
find . -type d -name .git -exec rm -rf '{}' ';' -prune
popd

mkdir -p "$IMG_DIR"/bin


cp -r "$TMP_DIR"/"$NAME" "$BUILD_DIR"/"$NAME"

pushd "$BUILD_DIR"/"$NAME"
make clean
make dist DEBUG=0
EXE="$(find dist -mindepth 1 -maxdepth 1 -type f -name '*.exe' -printf '%f\n' | head -n1)"
popd

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$(basename "$DIST_FILE")" == "$EXE" ]; then
    continue
  fi

  cp -r "$DIST_FILE" "$IMG_DIR"/
done

cp "$BUILD_DIR"/"$NAME"/dist/"$EXE" "$IMG_DIR"/bin/"$EXE"

for lib in $(ldd "$IMG_DIR"/bin/"$EXE" | grep -E '=>\s*/' | awk '{ print $3 }' | sed -e 's#^/##' | grep '^mingw64/'); do
  cp /"$lib" "$IMG_DIR"/bin/"$(basename "$lib")"
done

cat >"$IMG_DIR"/"$NAME".bat <<EOF
@start ".:mode:." /D "%~dp0" "%~dp0\\bin\\$EXE"
EOF

pushd "$IMG_DIR"
zip -r "$(dirs -l +1)"/"$NAME".zip ./*
popd
