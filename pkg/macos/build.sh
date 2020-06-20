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


SELF="$(cd "$(dirname "$0")" &>/dev/null && pwd -P && cd - &>/dev/null)"/"$(basename "$0")"
SRC_DIR="$(dirname $(dirname "$(dirname "$SELF")"))"

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


# build dist directory without debug and defined relative resource
start_group 'Run gmake'
set -x

pushd "$BUILD_DIR"/"$NAME" >/dev/null

gmake distclean
gmake dist DEBUG=0 RESOURCE=../Resources

popd >/dev/null

{ set +x; } 2>/dev/null
end_group


# prepare image for packaging
start_group 'Create distributable image'
set -x

mkdir -p "$IMAGE_DIR"/Contents
mkdir -p "$IMAGE_DIR"/Contents/MacOS
mkdir -p "$IMAGE_DIR"/Contents/Frameworks
mkdir -p "$IMAGE_DIR"/Contents/Resources

{ set +x; } 2>/dev/null

for DIST_FILE in "$BUILD_DIR"/"$NAME"/dist/*; do
  if [ "$DIST_FILE" == "$BUILD_DIR"/"$NAME"/dist/"$NAME" ]; then
    continue
  fi

  set -x

  cp -r "$DIST_FILE" "$IMAGE_DIR"/Contents/Resources/

  { set +x; } 2>/dev/null
done

set -x

cp "$BUILD_DIR"/"$NAME"/dist/"$NAME" "$IMAGE_DIR"/Contents/MacOS/"$NAME"

{ set +x; } 2>/dev/null

DYLIBS_ADDED=0

set -x

install_name_tool -add_rpath @executable_path/../Frameworks "$IMAGE_DIR"/Contents/MacOS/"$NAME"

{ set +x; } 2>/dev/null

for dylib in $(otool -L "$IMAGE_DIR"/Contents/MacOS/"$NAME" | grep '^\t/' | awk '{ print $1 }' | sed -e 's#^/##' | grep '^usr/local/'); do
  dylibbase="$(basename "$dylib")"

  set -x

  cp /"$dylib" "$IMAGE_DIR"/Contents/Frameworks/"$dylibbase"
  install_name_tool -change /"$dylib" @rpath/"$dylibbase" "$IMAGE_DIR"/Contents/MacOS/"$NAME"

  { set +x; } 2>/dev/null

  DYLIBS_ADDED="$(expr "$DYLIBS_ADDED" + 1)"
done

while [ "$DYLIBS_ADDED" -gt 0 ]; do
  DYLIBS_ADDED=0

  for dylib in "$IMAGE_DIR"/Contents/Frameworks/*; do
    dylibbase="$(basename "$dylib")"

    set -x

    chmod 644 "$dylib"

    { set +x; } 2>/dev/null

    for ddylib in $(otool -L "$dylib" | grep '^\t/' | awk '{ print $1 }' | sed -e 's#^/##' | grep '^usr/local/'); do
      ddylibbase="$(basename "$ddylib")"

      if [ "$dylibbase" == "$ddylibbase" ]; then
	set -x

        install_name_tool -id @rpath/"$ddylibbase" "$dylib"

	{ set +x; } 2>/dev/null
      else
        if ! [ -e "$IMAGE_DIR"/Contents/Frameworks/"$ddylibbase" ]; then
	  set -x

          cp /"$ddylib" "$IMAGE_DIR"/Contents/Frameworks/"$ddylibbase"

	  { set +x; } 2>/dev/null

          DYLIBS_ADDED="$(expr "$DYLIBS_ADDED" + 1)"
        fi

	set -x

        install_name_tool -change /"$ddylib" @rpath/"$ddylibbase" "$dylib"

	{ set +x; } 2>/dev/null
      fi
    done
  done
done

set -x

mkdir -p "$BUILD_DIR"/"$NAME".iconset

{ set +x; } 2>/dev/null

for size in 16 32 128 256 512; do
  size2x="$(expr "$size" '*' 2)"

  set -x

  sips -z "$size" "$size" "$SRC_DIR"/"$ICON" --out "$BUILD_DIR"/"$NAME".iconset/icon_"$size"x"$size".png
  sips -z "$size2x" "$size2x" "$SRC_DIR"/"$ICON" --out "$BUILD_DIR"/"$NAME".iconset/icon_"$size"x"$size"@2x.png

  { set +x; } 2>/dev/null
done

set -x

iconutil --convert icns --output "$IMAGE_DIR"/Contents/Resources/"$NAME".icns "$BUILD_DIR"/"$NAME".iconset

{ set +x; } 2>/dev/null

echo "+ cat >'$IMAGE_DIR/Contents/PkgInfo'" >&2
cat >"$IMAGE_DIR"/Contents/PkgInfo <<EOF
APPL????
EOF

echo "+ cat >'$IMAGE_DIR/Contents/Info.plist'" >&2
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

end_group


# package image
start_group 'Package into app directory and zip file'
set -x

rm -rf "$NAME".app
rm -f "$NAME".zip

cp -a "$IMAGE_DIR" "$NAME".app
zip -r "$NAME".zip "$NAME".app

{ set +x; } 2>/dev/null
end_group
