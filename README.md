# c-challenge
A set number of developers will work together without communicating to code something using C

## Setup

### Raylib

Ubuntu
```bash
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

Fedora
```bash
sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
```

Arch
```bash
sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```

Wayland
```bash
sudo dnf install wayland-devel libxkbcommon-devel wayland-protocols-devel
```

Windows
```powershell
# On powershell as admin
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
scoop bucket add main
scoop install main/gcc
scoop install main/make
scoop install main/cmake
scoop install main/mingw
# Inside ./external/raylib/src
mingw32-make PLATFORM=PLATFORM_DESKTOP
# Inside ./build/
cmake -G "MinGW Makefiles" ..
mingw32-make
```

