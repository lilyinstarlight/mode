$env:PATH = "C:\msys64\mingw64\bin;C:\msys64\usr\bin;$env:PATH"
$env:PKG_CONFIG_PATH = "C:\msys64\mingw64\lib\pkgconfig"

pacman.exe -Syyuu --noconfirm --noprogressbar
taskkill /f /fi "MODULES eq msys-2.0.dll"
pacman.exe -Syyuu --noconfirm --noprogressbar
taskkill /f /fi "MODULES eq msys-2.0.dll"

bash.exe -c "exec pacman.exe -S --needed --noconfirm --noprogressbar base-devel zip mingw-w64-x86_64-gcc mingw-w64-x86_64-smpeg2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_gfx mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2 mingw-w64-x86_64-yaml-cpp 2>&1"

bash.exe -c "exec pkg/windows/build.sh 2>&1"
