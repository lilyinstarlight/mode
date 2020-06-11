$env:PATH = "C:\msys64\mingw64\bin;C:\msys64\usr\bin;$env:PATH"

pacman.exe -Syuu --noconfirm --noprogressbar
pacman.exe -Syuu --noconfirm --noprogressbar

pacman.exe -S --needed --noconfirm --noprogressbar base-devel zip mingw-w64-x86_64-gcc mingw-w64-x86_64-smpeg2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_gfx mingw-w64-x86_64-SDL2_net mingw-w64-x86_64-SDL2 mingw-w64-x86_64-yaml-cpp

bash.exe pkg/windows/build.sh
