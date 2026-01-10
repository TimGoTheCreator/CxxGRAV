# SemiEuler
Semi implicit Euler implementation in C++ to simulate interaction of N-Body objects

How to install Dependencies (Windows)
Install MSYS2 From https://msys2.org
then open the MinGW64 shell and type
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make
```

How to compile SemiEuler (CMake Windows)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

How to install Dependencies (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential git cmake pkg-config \
    libasound2-dev libx11-dev libxrandr-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
    libwayland-dev libxkbcommon-dev
```

How to install Dependencies (Fedora)
```bash
sudo dnf install gcc make git cmake \
    alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel \
    libXi-devel libXcursor-devel libXinerama-devel libatomic
```

How to install Dependencies (Arch Linux)
```bash
sudo pacman -S gcc make git cmake \
    libx11 libxrandr libxi libxcursor libxinerama mesa
```

How to compile SemiEuler (CMake Linux)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
