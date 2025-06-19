# EM Field Visualizer

A C++ interactive simulator for visualizing electric fields and equipotential lines created by user-placed point charges.

Built with:
- **SDL2**
- **SDL2_ttf**
- **SDL2_image**
- **Emscripten** (for WebAssembly build)

---

## Project Structure
```plaintext
├── local_gui
│   ├── assets
│   │   ├── arial.ttf
│   │   └── arrow.png
│   ├── build
│   │   ├── CMakeFiles
│   │   ├── cmake_install.cmake
│   │   ├── CMakeCache.txt
│   │   ├── EMFieldVisualizer
│   │   └── Makefile
│   ├── include
│   │   ├── Charge.hpp
│   │   ├── FieldCalculator.hpp
│   │   ├── GUI.hpp
│   │   └── Vector2D.hpp
│   ├── src
│   │   ├── Charge.cpp
│   │   ├── FieldCalculator.cpp
│   │   ├── GUI.cpp
│   │   └── main.cpp
│   └── CMakeLists.txt
└── public_gui
    ├── assets
    │   ├── arial.ttf
    │   └── arrow.png
    ├── include
    │   ├── Charge.hpp
    │   ├── FieldCalculator.hpp
    │   ├── GUI.hpp
    │   └── Vector2D.hpp
    ├── src
    │   ├── Charge.cpp
    │   ├── FieldCalculator.cpp
    │   ├── GUI.cpp
    │   └── main.cpp
    ├── wasm_build
    │   ├── EMFieldVisualizer.data
    │   ├── EMFieldVisualizer.html
    │   ├── EMFieldVisualizer.js
    │   └── EMFieldVisualizer.wasm
    └── CMakeLists.txt
```

## Features

- Add, drag, and delete positive and negative point charges
- Visualize:
  - Electric field vectors (N/C)
  - Equipotential lines
  - Charge coordinates (meters)
- Toggle grid and axis view
- Live readouts of field strength and potential
- Start screen with instructions and START button
- Real-world physical units with centered coordinate system

---

## Requirements

### For `local_gui/` (Native Build)

- C++17 compiler
- CMake ≥ 3.10
- SDL2, SDL2_ttf, SDL2_image

#### macOS (with Homebrew)

```bash
brew install sdl2 sdl2_image sdl2_ttf cmake
```

### For `public_gui/` (WebAssembly Build)

- Emscripten SDK

#### macOS (in your Terminal)

```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
```

## Building and Running 

### For `local_gui/` (Native Executable)

#### Navigate to the project folder:

```bash
cd local_gui
mkdir build && cd build
cmake ..
make
```

#### Run the executable
```bash
./EMFieldVisualizer
```

### For `public_gui/` (WebAssembly Version)

#### Navigate to your Emscripten SDK directory

```bash
cd ~/emsdk
```

#### Activate the SDK for this session

```bash
source ./emsdk_env.sh
```

#### Navigate to the project folder:

```bash
cd public_gui
mkdir wasm_build && cd wasm_build
```

#### Compile using Emscripten:

```bash
emcc ../src/main.cpp ../src/Charge.cpp ../src/FieldCalculator.cpp ../src/GUI.cpp \
  -I../include \
  -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 \
  -s SDL2_IMAGE_FORMATS='["png"]' \
  -s ALLOW_MEMORY_GROWTH=1 -s WASM=1 -s FULL_ES2=1 \
  --preload-file ../assets \
  -o EMFieldVisualizer.html
```

#### Serve locally

```bash
python3 -m http.server 8080
# Then visit: http://localhost:8080/EMFieldVisualizer.html
```
