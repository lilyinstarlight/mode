#!/bin/bash -xe
SELF="$0"
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

mkdir -p "$IMG_DIR"/dist


cp -r "$TMP_DIR"/"$NAME" "$BLD_DIR"/"$NAME"

pushd "$BLD_DIR"/"$NAME"
make clean
make dist DEBUG=0
EXE="$(find dist -mindepth 1 -maxdepth 1 -type f -perm -u+x -printf '%f\n' | head -n1)"
popd

cp -r "$BLD_DIR"/"$NAME"/dist/ "$IMG_DIR"/dist/

mkdir -p "$BLD_DIR"/icon/"$NAME".iconset
for size in 16 32 128 256 512; do
  sips -z "$size" "$size" "$SRC_DIR"/"$ICON" --out "$BLD_DIR"/icon/"$NAME".iconset/icon_"$size"x"$size".png
  sips -z "$(expr "$size" '*' 2)" "$(expr "$size" '*' 2)" "$SRC_DIR"/"$ICON" --out "$BLD_DIR"/icon/"$NAME".iconset/icon_"$size"x"$size"@2x.png
done
iconutil --convert icns --output "$IMG_DIR"/icon.icns "$BLD_DIR"/icon/"$NAME".iconset

cat >"$IMG_DIR"/run.sh <<EOF
#!/bin/sh
cd "\`dirname "\$0"\`/dist"
exec ./'$NAME'
EOF
chmod +x "$IMG_DIR"/run.sh

mkdir -p "$IMG_DIR"/dist/dylib
for dylib in $(otool -L "$IMG_DIR"/dist/"$NAME" | grep '^\t/' | awk '{ print $1 }' | sed -e 's#^/##' | grep '^usr/local/'); do
  cp /"$dylib" "$IMG_DIR"/dist/dylib/"$(basename "$dylib")"
  install_name_tool -change /"$dylib" @executable_path/dylib/"$(basename "$dylib")" "$IMG_DIR"/dist/"$NAME"
done

rm -rf "$NAME".app

platypus -a "$PRETTY" -o 'None' -p /bin/sh -i "$IMG_DIR"/icon.icns -u "$CONTACT" -V "$VERSION" -I "$DOMAIN.$NAME" -B -R -f "$IMG_DIR"/dist "$IMG_DIR"/run.sh "$NAME".app
