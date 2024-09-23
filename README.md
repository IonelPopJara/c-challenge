# c-challenge

TODO: Write a proper description and give credit to Tuhkis.
A group of developers will collaborate to create software without any form of communication.

## Setup

### Pre-requisites

Before setting up the project, ensure you have the following tools installed:
- C Compiler (e.g., GCC)
- CMake (for building the project)
- Make (for building dependencies)
  
#### Windows

Scoop is a command-line installer for Windows that allows you to install software from the command line. It is recommended use it to install the required tools.

To install scoop, run the following command in PowerShell as an administrator:

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
Invoke-RestMethod -Uri https://get.scoop.sh | Invoke-Expression
```

After installing scoop, you can install the required tools by running the following commands:

```powershell
scoop bucket add main # Add the main bucket
scoop install main/gcc # Install the GCC compiler
scoop install main/make # Install the Make build tool
scoop install main/cmake # Install the CMake build tool
scoop install main/mingw # Install the MinGW compiler
```

#### Linux

On Linux, you can install the required tools using the package manager of your distribution.

**Ubuntu**
```bash
# Common tools
sudo apt install gcc make cmake
# Raylib dependencies
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
```

**Fedora**
```bash
# Common tools
sudo dnf install gcc make cmake
# Raylib dependencies
sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
```

**Arch**
```bash
# Common tools
sudo pacman -S gcc make cmake
# Raylib dependencies
sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```

#### macOS

> NOTE: I don't have a macOS machine to test the instructions. If you encounter any issues, please let me know.

On macOS, you can install the required tools using Homebrew.

```bash
xcode-select --install
brew install gcc make cmake
```

### Building the project

To start clone this repository by running:
    
```bash
https://github.com/IonelPopJara/c-challenge.git
cd c-challenge
```

#### Building Raylib

The project uses the Raylib library for graphics. To build the project, you need to build the Raylib library first.

Then, navigate to the `external/raylib/src` directory and build the Raylib library according to your OS:

**Windows**
```powershell
cd external/raylib/src
mingw32-make PLATFORM=PLATFORM_DESKTOP
cd ../../.. # Go back to the project root
```

**Linux and macOS**
```bash
cd external/raylib/src
make PLATFORM=PLATFORM_DESKTOP
cd ../../.. # Go back to the project root
```

#### Generating the build files

After building the Raylib library, you can generate the build files using CMake.

**Linux and macOS**
```bash
# From the project root
mkdir build # Create a build directory
cd build
cmake ..
make
```

**Windows**
```powershell
# From the project root
mkdir build # Create a build directory
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make
```

### Running the project

After building the project, you can run the executable generated in the `build` directory.

**Linux and macOS**
```bash
# From the build directory
cd build
./hacktober.out
```

**Windows**
```powershell
# From the build directory
cd build
./hacktober.exe
```
