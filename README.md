# CxxGRAV
### A Graphical Semi implicit Euler implementation in C++ to simulate interaction of N-Body objects

![CI](https://github.com/TimGoTheCreator/SemiEuler/actions/workflows/cmake-multi-platform.yml/badge.svg)
![Language](https://img.shields.io/badge/language-C++-blue.svg)
![License](https://img.shields.io/badge/license-Apache%202.0-green.svg)
[![Discord](https://img.shields.io/discord/1461315450632802409?label=Discord&logo=discord&logoColor=white&color=5865F2)](https://discord.gg/8kszVN667z)

### CxxGRAV is built on top of the [RayLib](https://github.com/raysan5/raylib) Library

![Three Body Problem](videos/three_body_stars.gif) 

## Roadmap
- **Priority 1:** Add particles by clicking
- **Priority 2:** Galaxy simulation
- **Priority 3:** Barnes–Hut
- **Priority 4:** macOS support 

## Roadmap (Finished) 
- **1** Camera, UI (DONE) 

# How to install Dependencies (Windows)
Install MSYS2 From [msys2.org](https://msys2.org)
then open the MinGW64 shell and type
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make
```

# MSVC Dependencies
For MSVC (CL Compiler) builds, Visual Studio 2022 with C++ Desktop Development workload is required.
Raylib and other system dependencies are auto-downloaded by CMake.


# Quick start (For all Operating Systems) 
```bash
git clone https://github.com/TimGoTheCreator/CxxGRAV.git
cd CxxGRAV
mkdir build
cd build
cmake ..
cmake --build . 
cd bin
./CxxGRAV
```

For this, Git is required
Note:
for MSVC the directory of the binary output might change to Debug

# How to install Dependencies (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential git cmake pkg-config \
    libasound2-dev libx11-dev libxrandr-dev libxi-dev \
    libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
    libwayland-dev libxkbcommon-dev
```

# How to install Dependencies (Fedora)
```bash
sudo dnf install gcc make git cmake \
    alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel \
    libXi-devel libXcursor-devel libXinerama-devel libatomic
```

# How to install Dependencies (Arch Linux)
```bash
sudo pacman -S gcc make git cmake \
    libx11 libxrandr libxi libxcursor libxinerama mesa
```
