#!/bin/bash
set -e

echo "=== CxxGRAV dependency installer ==="

if command -v apt >/dev/null; then
    echo "Detected Debian/Ubuntu. Installing packages..."
    sudo apt update
    sudo apt install -y build-essential git cmake pkg-config \
        libasound2-dev libx11-dev libxrandr-dev libxi-dev \
        libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev \
        libwayland-dev libxkbcommon-dev
elif command -v dnf >/dev/null; then
    echo "Detected Fedora. Installing packages..."
    sudo dnf install -y gcc make git cmake \
        alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel \
        libXi-devel libXcursor-devel libXinerama-devel libatomic
elif command -v pacman >/dev/null; then
    echo "Detected Arch Linux. Installing packages..."
    sudo pacman -S --noconfirm gcc make git cmake \
        libx11 libxrandr libxi libxcursor libxinerama mesa
else
    echo "Unsupported distro. Please install dependencies manually."
    exit 1
fi

echo "=== Dependencies installed successfully ==="
