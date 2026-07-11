# 2D Physics Engine & Simulation

A cross-platform 2D physics engine built with C++ and SDL2 that simulates realistic physics interactions including springs, gravity, drag forces, and collision response. The engine features an interactive simulation where users can apply impulse forces through mouse interaction and observe connected particles behave under various physical forces.

## Overview

This project demonstrates fundamental physics simulation concepts by implementing a particle-based physics engine with multiple force generators. The simulation visualizes four connected particles arranged in a square grid, connected by springs that respond to stretching and compression. Users can interact with the simulation by dragging particles to apply impulses and watching the system respond in real-time.

### Key Features

- **Particle Physics**: Mass-based particles with position, velocity, and acceleration tracking
- **Force Simulation**: Multiple force types including gravity, drag, spring forces, and user-applied impulses
- **Spring Constraints**: Elastic connections between particles with configurable stiffness and rest length
- **Interactive Control**: Mouse-based impulse application and keyboard input for external forces
- **Real-time Visualization**: Rendered using SDL2 with continuous frame updates at 60 FPS
- **Cross-platform Support**: Builds on macOS (Homebrew), Linux, and Windows (MSYS2 MinGW64)

## Stack

- **Language(s):** C++ (primary), with Python support scripts
- **Framework/Runtime:** C++17 with SDL2 (Simple DirectMedia Layer)
- **Graphics:** SDL2, SDL2_image, SDL2_gfx (primitive drawing library)
- **Build:** GNU Make with cross-platform compiler detection
- **Physics:** Custom vector math (Vec2) and force calculations

## Project Structure

```
src/
  Main.cpp              Application entry point, main loop
  Application.h/cpp     Core simulation logic, state management, setup/update/render cycle
  Graphics.h/cpp        SDL2 rendering abstraction, drawing primitives
  Physics/
    Body.h/cpp          Particle representation with position, velocity, mass
    Force.h/cpp         Force generators (gravity, drag, springs, friction)
    Vec2.h/cpp          2D vector math with operators and utilities
    Particle.h/cpp      Particle-specific data (thin wrapper)
    Shape.h/cpp         Shape definitions for drawing
    Constants.h         Simulation constants (FPS, pixel scaling)

assets/                 Texture and sprite assets for rendering
  basketball.png        Particle sprite
  bowlingball.png       Heavy particle sprite
  crate.png             Box sprite
  metal.png             Surface texture

Makefile               Cross-platform build configuration
.ycm_extra_conf.py     YouCompleteMe IDE configuration
```

### How It Fits Together

**Main Control Flow:**
1. `Main.cpp` initializes an `Application` object and runs the game loop
2. Each frame iteration:
   - **Input()** captures keyboard and mouse events to modify forces and handle user interaction
   - **Update()** applies forces (gravity, drag, springs, user impulses) to all bodies, integrates physics using delta-time, and checks boundary conditions
   - **Render()** draws all particles, constraint springs, and user input feedback to the SDL2 renderer

**Physics System:**
- The `Application` maintains a vector of `Body` pointers representing the four connected particles
- In the Setup phase, four particles are spawned in a square pattern and connected by springs
- During each Update, `Force` static methods generate forces between bodies (spring constraints connect all adjacent particles and diagonal pairs)
- Each `Body` accumulates forces in `sumForces`, then integrates acceleration and velocity using Euler method to produce new positions
- Boundary collision is handled with velocity reflection (90% damping) when particles hit window edges

**Interaction Model:**
- Mouse drag creates impulses: difference between start and end cursor positions scales a force applied to the last particle
- Arrow keys apply continuous directional forces to all particles
- Spring forces pull particles back toward rest positions, creating oscillation and damping effects

## How to Run It

### Prerequisites

**macOS (Homebrew):**
```bash
brew install sdl2 sdl2_image sdl2_gfx
```

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-gfx-dev
```

**Windows (MSYS2 MinGW64):**
```bash
pacman -S mingw-w64-x86_64-sdl2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_gfx
```

### Build

```bash
# Using the Makefile (automatic platform detection)
make clean
make build

# Or manual compilation
g++ -std=c++17 -Wall src/*.cpp src/Physics/*.cpp -o app $(pkg-config --cflags --libs sdl2 SDL2_image SDL2_gfx)
```

### Run

```bash
# macOS/Linux
./app

# Windows (MSYS2 MinGW64)
./app.exe
```

### Interactive Controls

| Control | Action |
|---------|--------|
| **Mouse Drag** | Click and drag a particle to apply an impulse force (red line shows direction) |
| **Arrow Keys** | Apply continuous force in the direction: ↑ Up, ↓ Down, ← Left, → Right |
| **ESC** | Exit the simulation |

### Configuration

Edit constants in `src/Physics/Constants.h`:
```cpp
const int FPS = 60;                    // Target frame rate
const int PIXELS_PER_METER = 20;       // Pixel-to-physics scale
```

Modify simulation parameters in `src/Application.cpp`:
```cpp
float k = 1000;                        // Spring stiffness constant
float restLength = 200;                // Spring rest length in pixels
anchor = Vec2(width/2, 30);            // Anchor point for springs
```

## Architecture Details

### Physics Components

**Vec2 (Vector Math)**
- Provides 2D vector operations: addition, subtraction, scaling, normalization, dot/cross products
- Supports operator overloading for intuitive math expressions

**Body (Particle)**
- Stores position, velocity, acceleration, mass, and radius
- `Integrate(dt)` updates position/velocity using acceleration and delta time
- `AddForce()` accumulates forces for the current frame; forces reset after integration

**Force Generators**
- `GenerateDragForce()` applies velocity-dependent damping (air resistance)
- `GenerateSpringForce()` produces Hooke's law restoring forces between particles or toward anchors
- `GenerateFrictionForce()` and `GenerateGravitationalForce()` provide additional physics options

### Graphics Abstraction

The `Graphics` struct encapsulates SDL2 initialization and drawing:
- `OpenWindow()` / `CloseWindow()` manage SDL lifecycle
- Drawing primitives: circles, lines, rectangles, polygons
- `RenderFrame()` flushes draw calls to the screen

### Simulation Loop

**Fixed Timestep:**
- Each frame waits to maintain 60 FPS using SDL_Delay
- Delta time is clamped to 16ms to prevent large physics jumps

**Force Application Order:**
1. User-applied forces (keyboard/mouse)
2. Global forces (gravity, drag)
3. Constraint forces (springs connecting all particle pairs)
4. Position update via numerical integration

**Boundary Handling:**
- Particles bounce off window edges with 90% velocity retention
- Prevents particles from leaving the visible area

## Try Asking

- **How do I adjust the spring stiffness or change particle behavior?** – Edit `k` (spring constant) and `restLength` in `Application.cpp`'s `Update()` function
- **Can I add more particles or change the particle arrangement?** – Modify `NUM_BODIES` and the spawn calls in `Application::Setup()`
- **What's the pixel-to-physics scale and why does it matter?** – `PIXELS_PER_METER` in `Constants.h` converts between simulation units and screen coordinates; adjust for different simulation speeds

## License

This project is open source. Feel free to use, modify, and distribute as needed.

## Author

Created by [rathod-dhruv](https://github.com/rathod-dhruv)
