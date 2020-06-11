#!/bin/bash -xe
SELF="$(cd "$(dirname "$0")" &>/dev/null && pwd -P && cd - &>/dev/null)"/"$(basename "$0")"
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
make dist DEBUG=0 RESOURCE=../Resources
EXE="$(find dist -mindepth 1 -maxdepth 1 -type f -perm -u+x -printf '%f\n' | head -n1)"
popd


# prepare image for packaging
mkdir -p "$IMAGE_DIR"/Contents
mkdir -p "$IMAGE_DIR"/Contents/MacOS
mkdir -p "$IMAGE_DIR"/Contents/Frameworks
mkdir -p "$IMAGE_DIR"/Contents/Resources

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$(basename "$DIST_FILE")" == "$EXE" ]; then
    continue
  fi

  cp -r "$DIST_FILE" "$IMAGE_DIR"/Contents/Resources/
done

cp "$BUILD_DIR"/"$NAME"/dist/"$EXE" "$IMAGE_DIR"/Contents/MacOS/"$NAME"

mkdir -p "$BUILD_DIR"/"$NAME".iconset
for size in 16 32 128 256 512; do
  sips -z "$size" "$size" "$SRC_DIR"/"$ICON" --out "$BUILD_DIR"/"$NAME".iconset/icon_"$size"x"$size".png
  sips -z "$(expr "$size" '*' 2)" "$(expr "$size" '*' 2)" "$SRC_DIR"/"$ICON" --out "$BUILD_DIR"/"$NAME".iconset/icon_"$size"x"$size"@2x.png
done
iconutil --convert icns --output "$IMAGE_DIR"/Contents/Resources/"$NAME".icns "$BUILD_DIR"/"$NAME".iconset

cat >"$IMAGE_DIR"/Contents/PkgInfo <<EOF
APPL????
EOF

cat >"$IMAGE_DIR"/Contents/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>en</string>
	<key>CFBundleExecutable</key>
	<string>$NAME</string>
	<key>CFBundleIconFile</key>
	<string>$NAME.icns</string>
	<key>CFBundleIdentifier</key>
	<string>$DOMAIN.$NAME</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>6.0</string>
	<key>CFBundleName</key>
	<string>$PRETTY</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>$VERSION</string>
	<key>CFBundleVersion</key>
	<string>$VERSION</string>
	<key>LSMinimumSystemVersion</key>
	<string>10.8.0</string>
	<key>LSUIElement</key>
	<true/>
	<key>NSHumanReadableCopyright</key>
	<string>Copyright $(date '+%Y') $CONTACT</string>
	<key>NSPrincipalClass</key>
	<string>NSApplication</string>
	<key>NSHighResolutionCapable</key>
	<true/>
</dict>
</plist>
EOF

for dylib in $(otool -L "$IMAGE_DIR"/Contents/MacOS/"$NAME" | grep '^\t/' | awk '{ print $1 }' | sed -e 's#^/##' | grep '^usr/local/'); do
  cp /"$dylib" "$IMAGE_DIR"/Contents/Frameworks/"$(basename "$dylib")"
  install_name_tool -change /"$dylib" @rpath/"$(basename "$dylib")" "$IMAGE_DIR"/Contents/MacOS/"$NAME"
done


# package image
rm -rf "$NAME".app
rm -f "$NAME".zip

cp -a "$IMAGE_DIR" "$NAME".app
zip -r "$NAME".zip "$NAME".app
