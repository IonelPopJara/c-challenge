# First Mults' Discord Hacktoberfest Challenge

~~I need an actual name for the project~~

In this challenge, a group of developers will collaborate to create software without any form of communication. Each developer will get 48 hours to make changes to the codebase, once the time is up, the next developer will continue from where the previous developer left off.

The project will be coded in C and will use the Raylib library for graphics. The software can be a game, a utility, or anything else that the developers can come up with. There are no restrictions on what it should do, as long as it follows basic regulations such as no harmful code, no hate speech, etc.

This idea was proposed by [Tuhkis](https://github.com/Tuhkis) on the Mults' Discord server.

## Rules

1. Each developer will have 48 hours to make changes to the codebase.

-   The time starts when the developer successfully builds the project.

2. The developer must not communicate with the previous developer.

-   Technical comments in the code **are allowed**. (e.g., `// This function calculates does something`)
-   Comments that give hints or instructions **are NOT allowed**. (e.g., `// You should do this`, `// Add this feature`)

3. The developer **must not** communicate with the next developer.
4. In case of technical issues, the developer can ask for help in the Discord server. Ideally, a memeber who already finished their turn should help.
5. Be mindful of other people's contributions. If you don't like something, try to work on it instead of completely removing it. Of course, this is a case by case basis. If there is something that breaks the code, or is harmful, it should be removed.
6. In order to properly contribute to the project every developer **must follow** the [CONTRIBUTING.md](CONTRIBUTING.md) file. Please read it carefully before starting your turn.

## Project Structure

```bash
.
├───.github
│   └───workflows
├───.vscode
├───assets
├───docs
├───external
│   └───raylib
│       └───src
└───src
```

-   **.github/workflows/**: Contains GitHub Actions workflows for building the project.
-  **.vscode/**: Contains settings for Visual Studio Code.
-   **assets/**: Contains program assets such as images and sounds.
-   **docs/**: Documentation related to the project.
-   **external/**: Contains external libraries used in the project. Currently, only Raylib is included.
-   **src/**: Contains all source files (`.c`) and header files (`.h`).

### Adding Files

-   When adding new source code, place them in the `src/` folder.
-   Assets like images or sounds should be placed in the `assets/` folder, under the appropriate subdirectory (e.g., `images/` or `sounds/`).

## Setup

### Pre-requisites

Before setting up the project, ensure you have the following tools installed:

-   C Compiler (e.g., GCC)
-   CMake (for building the project)
-   Make (for building dependencies)
-   MinGW (for Windows users)
-   SDL2 (for graphics) (Linux and macOS only)
-   A code editor or IDE of your choice. I recommend VSCode since this project has a folder already set up for it.
    -   You will need to install the C/C++ extension for VSCode.
    -   VSCode should detect the needed tools and prompt you to install them.

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

If you want everything in one command, you can run the following command after adding the main bucket:

```powershell
scoop install main/gcc main/make main/cmake main/mingw
```

#### Linux

On Linux, you can install the required tools using the package manager of your distribution.

**Ubuntu**

```bash
# Common tools
sudo apt install gcc make cmake
# Raylib dependencies
sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
# SDL2
sudo apt install libsdl2-dev
```

**Fedora**

```bash
# Common tools
sudo dnf install gcc make cmake
# Raylib dependencies
sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
# SDL2
sudo dnf install SDL2-devel
```

**Arch**

```bash
# Common tools
sudo pacman -S gcc make cmake
# Raylib dependencies
sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
# SDL2
sudo pacman -S sdl2
```

#### macOS

> NOTE: I don't have a macOS machine to test the instructions. If you encounter any issues, please let me know.

On macOS, you can install the required tools using Homebrew.

```bash
xcode-select --install
brew install gcc make cmake
brew install sdl2
```

#### Extra Notes

In case you are interested, SDL2 is used as a backend for Raylib on Linux due to Wayland compatibility. Windows users will use Raylib directly with no SDL2 dependencies. Since I don't have a macOS machine, I used SDL2 as the backend for macOS as well.

If you want to use GLFW instead, you can follow the [Linux instructions here](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux) or the [macOS instructions here](https://github.com/raysan5/raylib/wiki/Working-on-macOS).

### Building the project

Begin by [forking the repository](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/fork-a-repo) to your GitHub account.

Then clone your fork of the repo by running:

```bash
# DON'T FORGET TO REPLACE `your-username` WITH YOUR GITHUB USERNAME!!!!
git clone https://github.com/your-username/c-challenge.git # Clone your fork
cd c-challenge # Change directory to the project folder
```

For a more detailed guide on how to work on this repo you can check the [CONTRIBUTING.md](CONTRIBUTING.md) file.

#### Building Raylib

The project uses [Raylib](https://github.com/raysan5/raylib) for graphics. To build the project, you need to build the Raylib library first.

Navigate to the `external/raylib/src` directory and build the Raylib library according to your OS:

**Windows**

```powershell # From the project root
cd external\raylib\src
mingw32-make PLATFORM=PLATFORM_DESKTOP
cd ..\..\.. # Go back to the project root
```

**Linux and macOS**

```bash
cd external/raylib/src # From the project root
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

If you are running the project before any contributions, you should see a window like this pop up:

![Initial window](/docs/window.png)

## Common issues

If you encounter any issues, please write them down here with the solution you found.

## Contributors
Tuhkis

Marxy37

raskunet

wanders
