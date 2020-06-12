#!/bin/bash -xe
SELF="$(readlink -f "$0")"
SRC_DIR="$(dirname $(dirname "$(dirname "$SELF")"))"

source "$(dirname "$(dirname "$SELF")")"/metadata.sh

BIN_DIR="$(dirname "$SELF")"/bin
BUILD_DIR="$(dirname "$SELF")"/build
IMAGE_DIR="$(dirname "$SELF")"/image


# prepare work directories
rm -rf "$BIN_DIR"
mkdir -p "$BIN_DIR"
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
make dist DEBUG=0 RESOURCE=../share/"$NAME"
EXE="$(find dist -mindepth 1 -maxdepth 1 -type f -perm -u+x -exec basename '{}' ';' | head -n1)"
popd


# prepare image for packaging
mkdir -p "$IMAGE_DIR"/usr/bin
mkdir -p "$IMAGE_DIR"/usr/share
mkdir -p "$IMAGE_DIR"/usr/share/applications
mkdir -p "$IMAGE_DIR"/usr/share/icons/hicolor/"$(file "$SRC_DIR"/"$ICON" | grep -oE '\d+\s+x\s+\d+' | tr -d ' ')"
mkdir -p "$IMAGE_DIR"/usr/share/metainfo
mkdir -p "$IMAGE_DIR"/usr/share/"$NAME"

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$(basename "$DIST_FILE")" == "$EXE" ]; then
    continue
  fi

  cp -r "$DIST_FILE" "$IMAGE_DIR"/usr/share/"$NAME"/
done

cp "$BUILD_DIR"/"$NAME"/dist/"$EXE" "$IMAGE_DIR"/usr/bin/"$NAME"

cat >"$IMAGE_DIR"/AppRun <<EOF
#!/bin/sh
SELF="\`readlink -f "\$0"\`"
IMG="\`dirname "\$SELF"\`"

export PATH="\$IMG"/usr/bin/:"\$IMG"/usr/sbin/:"\$IMG"/usr/games/:"\$IMG"/bin/:"\$IMG"/sbin/:"\$PATH"
export LD_LIBRARY_PATH="\$IMG"/usr/lib/:"\$IMG"/usr/lib/i386-linux-gnu/:"\$IMG"/usr/lib/x86_64-linux-gnu/:"\$IMG"/usr/lib32/:"\$IMG"/usr/lib64/:"\$IMG"/lib/:"\$IMG"/lib/i386-linux-gnu/:"\$IMG"/lib/x86_64-linux-gnu/:"\$IMG"/lib32/:"\$IMG"/lib64/:"\$LD_LIBRARY_PATH"
export XDG_DATA_DIRS="\$IMG"/usr/share/:"\$XDG_DATA_DIRS"

cd "\$IMG"/usr/share/'$NAME'

exec "\$IMG"/usr/bin/'$NAME' "\$@"
EOF
chmod +x "$IMAGE_DIR"/AppRun

cp "$SRC_DIR"/"$ICON" "$IMAGE_DIR"/"$NAME".png
cp "$SRC_DIR"/"$ICON" "$IMAGE_DIR"/usr/share/icons/hicolor/"$(file "$SRC_DIR"/"$ICON" | grep -oE '\d+\s+x\s+\d+' | tr -d ' ')"/"$NAME".png

cat >"$IMAGE_DIR"/"$DOMAIN"."$NAME".desktop <<EOF
[Desktop Entry]
Name=$PRETTY
Exec=$NAME
Icon=$NAME
Type=Application
Categories=Game;
EOF
cp "$IMAGE_DIR"/"$DOMAIN"."$NAME".desktop "$IMAGE_DIR"/usr/share/applications/"$NAME".desktop

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

for lib in $(ldd "$IMAGE_DIR"/usr/bin/"$NAME" | grep -E '=>\s*/' | awk '{ print $3 }' | sed -e 's#^/##'); do
  mkdir -p "$IMAGE_DIR"/"$(dirname "$lib")"
  cp /"$lib" "$IMAGE_DIR"/"$lib"
done


# get helper tools
pushd "$BIN_DIR"
rm -f appimagetool-x86_64.AppImage
rm -f appimagetool
wget 'https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage'
chmod +x appimagetool-x86_64.AppImage
ln -s appimagetool-x86_64.AppImage appimagetool
popd


# package image
rm -f "$NAME".AppImage

"$BIN_DIR"/appimagetool "$IMAGE_DIR" "$NAME".AppImage
