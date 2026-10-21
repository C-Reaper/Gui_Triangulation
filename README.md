# Project README

## Overview
The Triangulation project is a C application designed to build and visualize a Delaunay triangulation of moving points inside the box.

## Features
- **Triangulation**: Points inside the box are connected using a Delaunay triangulation. Its triangles do not cross and favor nearby neighbors.
- **Real-time Rendering**: Uses a basic 2D rendering engine to display the triangles in a window.

## Project Structure
```
Gui_Triangulation/
├── build/              # .exe files produced by Main.c
├── src/                # Source code directory
│   └── Main.c          # Entry point of the application
├── Makefile.linux      # Linux Build configuration
├── Makefile.windows    # Windows Build configuration
├── Makefile.wine       # Wine Build configuration
├── Makefile.web        # Emscripten Build configuration
└── README.md           # This file
```

### Prerequisites
- C/C++ Compiler and Debugger (GCC, Clang)
- Make utility
- Standard development tools
- Libraries needed in specific projects (X11 for Linux, user32, gdi32, winmm for Windows)

## Build & Run
The project supports building on different platforms using the provided Makefiles.

### Building on Linux
```sh
cd Gui_Triangulation/
make -f Makefile.linux all

# To run the application:
./build/Main
```

### Building on Windows
```sh
cd Gui_Triangulation/
make -f Makefile.windows all

# To run the application:
.\build\Main.exe
```

### Building on Wine
```sh
cd Gui_Triangulation/
make -f Makefile.wine all

# To run the application:
wine build/Main.exe
```

### Building for WebAssembly
```sh
cd Gui_Triangulation/
make -f Makefile.web all

# To run the application in a web browser:
emrun --no_browser --port 8080 build/index.html
```