#!/bin/bash -xe
SELF="$(readlink -f "$0")"
SRC_DIR="$(dirname $(dirname "$(dirname "$SELF")"))"

source "$(dirname "$(dirname "$SELF")")"/metadata.sh

BIN_DIR='bin'
TMP_DIR='tmp'
BLD_DIR='build'
IMG_DIR='image'

rm -rf "$BIN_DIR"
mkdir -p "$BIN_DIR"
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR"
rm -rf "$BLD_DIR"
mkdir -p "$BLD_DIR"
rm -rf "$IMG_DIR"
mkdir -p "$IMG_DIR"

cp -r "$SRC_DIR" "$TMP_DIR"/"$NAME"

pushd "$TMP_DIR"
find . -type d -name .git -exec rm -rf '{}' ';' -prune
popd

mkdir -p "$IMG_DIR"/usr/bin
mkdir -p "$IMG_DIR"/usr/share
mkdir -p "$IMG_DIR"/usr/share/applications
mkdir -p "$IMG_DIR"/usr/share/icons/hicolor/"$(file "$SRC_DIR"/"$ICON" | grep -oE '\d+\s+x\s+\d+' | tr -d ' ')"
mkdir -p "$IMG_DIR"/usr/share/metainfo
mkdir -p "$IMG_DIR"/usr/share/"$NAME"


cp -r "$TMP_DIR"/"$NAME" "$BLD_DIR"/"$NAME"

pushd "$BLD_DIR"/"$NAME"
make clean
make dist DEBUG=0
EXE="$(find dist -mindepth 1 -maxdepth 1 -type f -perm -u+x -printf '%f\n' | head -n1)"
popd

for DIST_FILE in "$BLD_DIR"/"$NAME"/dist/*; do
  if [ "$(basename "$DIST_FILE")" == "$EXE" ]; then
    continue
  fi

  cp -r "$DIST_FILE" "$IMG_DIR"/usr/share/"$NAME"/
done

cp "$BLD_DIR"/"$NAME"/dist/"$EXE" "$IMG_DIR"/usr/bin/"$NAME"

cat >"$IMG_DIR"/AppRun <<EOF
#!/bin/sh
SELF="\`readlink -f "\$0"\`"
IMG="\`dirname "\$SELF"\`"

export PATH="\$IMG"/usr/bin/:"\$IMG"/usr/sbin/:"\$IMG"/usr/games/:"\$IMG"/bin/:"\$IMG"/sbin/:"\$PATH"
export LD_LIBRARY_PATH="\$IMG"/usr/lib/:"\$IMG"/usr/lib/i386-linux-gnu/:"\$IMG"/usr/lib/x86_64-linux-gnu/:"\$IMG"/usr/lib32/:"\$IMG"/usr/lib64/:"\$IMG"/lib/:"\$IMG"/lib/i386-linux-gnu/:"\$IMG"/lib/x86_64-linux-gnu/:"\$IMG"/lib32/:"\$IMG"/lib64/:"\$LD_LIBRARY_PATH"
export XDG_DATA_DIRS="\$IMG"/usr/share/:"\$XDG_DATA_DIRS"

cd "\$IMG"/usr/share/'$NAME'

exec "\$IMG"/usr/bin/'$NAME' "\$@"
EOF
chmod +x "$IMG_DIR"/AppRun

cp "$SRC_DIR"/"$ICON" "$IMG_DIR"/"$NAME".png
cp "$SRC_DIR"/"$ICON" "$IMG_DIR"/usr/share/icons/hicolor/"$(file "$SRC_DIR"/"$ICON" | grep -oE '\d+\s+x\s+\d+' | tr -d ' ')"/"$NAME".png

cat >"$IMG_DIR"/"$DOMAIN"."$NAME".desktop <<EOF
[Desktop Entry]
Name=$NAME
Exec=$NAME
Icon=$NAME
Type=Application
Categories=Game;
EOF
cp "$IMG_DIR"/"$DOMAIN"."$NAME".desktop "$IMG_DIR"/usr/share/applications/"$NAME".desktop

cat >"$IMG_DIR"/usr/share/metainfo/"$DOMAIN"."$NAME".appdata.xml <<EOF
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

for lib in $(ldd "$IMG_DIR"/usr/bin/"$NAME" | grep -E '=>\s*/' | awk '{ print $3 }' | sed -e 's#^/##'); do
  mkdir -p "$IMG_DIR"/"$(dirname "$lib")"
  cp /"$lib" "$IMG_DIR"/"$lib"
done

pushd "$BIN_DIR"
rm -f appimagetool-x86_64.AppImage
rm -f appimagetool
wget "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
chmod +x appimagetool-x86_64.AppImage
ln -s appimagetool-x86_64.AppImage appimagetool
popd

rm -f "$NAME-x86_64.AppImage"
rm -f "$NAME.AppImage"

"$BIN_DIR"/appimagetool "$IMG_DIR"

mv "$NAME-x86_64.AppImage" "$NAME.AppImage"
