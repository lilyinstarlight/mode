#!/bin/bash -e
start_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::group::$(echo -e "$*" | sed -e '1h;2,$H;$!d;g' -e 's/%/%25/g' -e 's/\r/%0D/g' -e 's/\n/%0A/g')" >&2
  fi
}

end_group() {
  if [ "$GITHUB_ACTIONS" == "true" ]; then
    echo "::endgroup::" >&2
  fi
}


SELF="$(readlink -f "$0")"
SRC_DIR="$(dirname "$(dirname "$(dirname "$SELF")")")"

start_group 'Packaging metadata'
set -x

source "$SRC_DIR"/pkg/metadata.sh

{ set +x; } 2>/dev/null
end_group

BUILD_DIR="$(dirname "$SELF")"/build
IMAGE_DIR="$(dirname "$SELF")"/image


# prepare work directories
start_group 'Make work directories'
set -x

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
rm -rf "$IMAGE_DIR"
mkdir -p "$IMAGE_DIR"

{ set +x; } 2>/dev/null
end_group


# copy source
start_group 'Copy source'
set -x

mkdir -p "$BUILD_DIR"/"$NAME"

{ set +x; } 2>/dev/null

for SRC_FILE in "$SRC_DIR"/*; do
  if [ "$(basename "$SRC_FILE")" == pkg ] || [ "$(basename "$SRC_FILE")" == build ] || [ "$(basename "$SRC_FILE")" == data ] || [ "$(basename "$SRC_FILE")" == dist ]; then
    continue
  fi

  set -x

  cp -r "$SRC_FILE" "$BUILD_DIR"/"$NAME"/

  { set +x; } 2>/dev/null
done

end_group


# build windows res file
start_group 'Create executable resource information'
set -x

mkdir -p "$BUILD_DIR"/"$NAME".res

magick convert "$BUILD_DIR"/"$NAME"/"$ICON" -define icon:auto-resize -compress zip "$BUILD_DIR"/"$NAME".res/"$NAME".ico

{ set +x; } 2>/dev/null

echo "+ cat >'$BUILD_DIR/$NAME.res/$NAME.rc'" >&2
cat >"$BUILD_DIR"/"$NAME".res/"$NAME".rc <<EOF
icon ICON "$(cygpath -ma "$BUILD_DIR"/"$NAME".res/"$NAME".ico)"

1 VERSIONINFO
FILEVERSION     $(echo "$VERSION.0.0.0" | tr '.' '\n' | head -n4 | tr '\n' ',' | sed 's/,$/\n/')
PRODUCTVERSION  $(echo "$VERSION.0.0.0" | tr '.' '\n' | head -n4 | tr '\n' ',' | sed 's/,$/\n/')
BEGIN
  BLOCK "StringFileInfo"
  BEGIN
    BLOCK "040904E4"
    BEGIN
      VALUE "CompanyName", "$CONTACT"
      VALUE "FileDescription", "$SUMMARY"
      VALUE "FileVersion", "$VERSION"
      VALUE "InternalName", "$NAME"
      VALUE "LegalCopyright", "$CONTACT"
      VALUE "OriginalFilename", "$NAME.exe"
      VALUE "ProductName", "$PRETTY"
      VALUE "ProductVersion", "$VERSION"
    END
  END
  BLOCK "VarFileInfo"
  BEGIN
    VALUE "Translation", 0x409, 1252
  END
END
EOF

set -x

windres "$BUILD_DIR"/"$NAME".res/"$NAME".rc "$BUILD_DIR"/"$NAME".res/"$NAME".res.o

{ set +x; } 2>/dev/null
end_group


# build dist directory without debug and defined relative resource
start_group 'Run make'
set -x

pushd "$BUILD_DIR"/"$NAME" >/dev/null

make distclean
make dist DEBUG=0 RESOURCE=.. MYOBJ="$BUILD_DIR"/"$NAME".res/"$NAME".res.o

popd >/dev/null

{ set +x; } 2>/dev/null
end_group


# prepare image for packaging
start_group 'Create distributable image'
set -x

mkdir -p "$IMAGE_DIR"/bin

{ set +x; } 2>/dev/null

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$DIST_FILE" == "$BUILD_DIR"/"$NAME"/dist/"$NAME".exe ]; then
    continue
  fi

  set -x

  cp -r "$DIST_FILE" "$IMAGE_DIR"/

  { set +x; } 2>/dev/null
done

set -x

cp "$BUILD_DIR"/"$NAME"/dist/"$NAME".exe "$IMAGE_DIR"/bin/"$NAME".exe

{ set +x; } 2>/dev/null

for lib in $(ldd "$IMAGE_DIR"/bin/"$NAME".exe | grep -E '=>\s*/' | awk '{ print $3 }' | sed -e 's#^/##' | grep '^mingw64'); do
  libbase="$(basename "$lib")"

  set -x

  cp /"$lib" "$IMAGE_DIR"/bin/"$libbase"

  { set +x; } 2>/dev/null
done

{ set +x; } 2>/dev/null

echo "+ cat >'$IMAGE_DIR/$NAME.bat'" >&2
cat >"$IMAGE_DIR"/"$NAME".bat <<EOF
@start ".:mode:." /D "%~dp0" "%~dp0\\bin\\$NAME.exe"
EOF

end_group


# package image
start_group 'Package into zip file'
set -x

rm -f "$NAME".zip

pushd "$IMAGE_DIR" >/dev/null

zip -r "$(dirs -l +1)"/"$NAME".zip ./*

popd >/dev/null

{ set +x; } 2>/dev/null
end_group
