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

which magick &>/dev/null || magick() { convert "$@"; }


start_group 'Packaging metadata'
set -x

source "$SRC_DIR"/pkg/metadata.sh

{ set +x; } 2>/dev/null
end_group

BIN_DIR="$(dirname "$SELF")"/bin
BUILD_DIR="$(dirname "$SELF")"/build
IMAGE_DIR="$(dirname "$SELF")"/image


# prepare work directories
start_group 'Make work directories'
set -x

rm -rf "$BIN_DIR"
mkdir -p "$BIN_DIR"
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

for src_file in "$SRC_DIR"/*; do
  if [ "$(basename "$src_file")" == pkg ] || [ "$(basename "$src_file")" == build ] || [ "$(basename "$src_file")" == data ] || [ "$(basename "$src_file")" == dist ]; then
    continue
  fi

  set -x

  cp -r "$src_file" "$BUILD_DIR"/"$NAME"/

  { set +x; } 2>/dev/null
done

end_group


# build dist directory without debug and defined relative resource
start_group 'Run make'
set -x

pushd "$BUILD_DIR"/"$NAME" >/dev/null

make distclean
make dist DEBUG=0 RESOURCE=../share/"$NAME"

popd >/dev/null

{ set +x; } 2>/dev/null
end_group


# prepare image for packaging
start_group 'Create distributable image'
set -x

mkdir -p "$IMAGE_DIR"/usr/bin
mkdir -p "$IMAGE_DIR"/usr/share
mkdir -p "$IMAGE_DIR"/usr/share/applications
mkdir -p "$IMAGE_DIR"/usr/share/icons/hicolor
mkdir -p "$IMAGE_DIR"/usr/share/metainfo
mkdir -p "$IMAGE_DIR"/usr/share/"$NAME"

{ set +x; } 2>/dev/null

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$DIST_FILE" == "$BUILD_DIR"/"$NAME"/dist/"$NAME" ]; then
    continue
  fi

  set -x

  cp -r "$DIST_FILE" "$IMAGE_DIR"/usr/share/"$NAME"/

  { set +x; } 2>/dev/null
done

set -x

cp "$BUILD_DIR"/"$NAME"/dist/"$NAME" "$IMAGE_DIR"/usr/bin/"$NAME"

{ set +x; } 2>/dev/null

for size in 16 32 64 128 256; do
  set -x

  mkdir -p "$IMAGE_DIR"/usr/share/icons/hicolor/"$size"x"$size"
  magick "$BUILD_DIR"/"$NAME"/"$ICON" -thumbnail "$size"x"$size" "$IMAGE_DIR"/usr/share/icons/hicolor/"$size"x"$size"/"$NAME".png

  { set +x; } 2>/dev/null
done

echo "+ cat >'$IMAGE_DIR/usr/share/applications/$NAME.desktop'" >&2
cat >"$IMAGE_DIR"/usr/share/applications/"$NAME".desktop <<EOF
[Desktop Entry]
Name=$PRETTY
Exec=$NAME
Icon=$NAME
Type=Application
Categories=Game;
EOF

echo "+ cat >'$IMAGE_DIR/usr/share/metainfo/$DOMAIN.$NAME.appdata.xml'" >&2
cat >"$IMAGE_DIR"/usr/share/metainfo/"$DOMAIN"."$NAME".appdata.xml <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<component type="desktop">
  <id>$DOMAIN.$NAME.desktop</id>
  <name>$NAME</name>
  <developer_name>$CONTACT</developer_name>
  <summary>$SUMMARY</summary>
  <description>
    $DESCRIPTION
  </description>
  <releases>
    <release version="$VERSION"/>
  </releases>
  <metadata_license>$LICENSE</metadata_license>
  <project_license>$LICENSE</project_license>
  <url type="homepage">$HOMEPAGE</url>
  <update_contact>$EMAIL</update_contact>
</component>
EOF

end_group


start_group 'Package into AppImage'
set -x

# get helper tools
pushd "$BIN_DIR" >/dev/null

rm -f linuxdeploy-x86_64.AppImage
rm -f linuxdeploy

wget 'https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage'
chmod +x linuxdeploy-x86_64.AppImage
ln -s linuxdeploy-x86_64.AppImage linuxdeploy

rm -f linuxdeploy-plugin-checkrt-x86_64.sh
rm -f linuxdeploy-plugin-checkrt

wget 'https://github.com/linuxdeploy/linuxdeploy-plugin-checkrt/releases/download/continuous/linuxdeploy-plugin-checkrt-x86_64.sh'
chmod +x linuxdeploy-plugin-checkrt-x86_64.sh
ln -s linuxdeploy-plugin-checkrt-x86_64.sh linuxdeploy-plugin-checkrt

popd >/dev/null


# package image
rm -f "$NAME".AppImage

env OUTPUT="$NAME".AppImage "$BIN_DIR"/linuxdeploy --appdir "$IMAGE_DIR" --desktop-file "$IMAGE_DIR"/usr/share/applications/"$NAME".desktop --plugin checkrt --output appimage

{ set +x; } 2>/dev/null
end_group
