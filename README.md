# Analog Clock

A beautifully rendered, real-time analog clock written in C using **SDL3**. This project utilizes GPU-accelerated geometry rendering to achieve smooth, aesthetic thick lines and perfectly rounded clock faces.

## Demo

<video src="https://github.com/Abhishek-Verma0/Analog-Clock/raw/main/demo.mp4" controls autoplay loop muted style="max-width: 100%;"></video>



## Features

- **Real-Time Accuracy:** Continuously syncs with your system's local time.
- **GPU Rendering:** Uses `SDL_RenderGeometry` for drawing smooth, anti-aliased thick lines and circles, instead of relying on legacy CPU-based rendering.
- **Custom Aesthetics:** Features a carefully chosen color palette including a dark blue-gray background, elegant gold accents for the clock ring and hour hand, a silver minute hand, and a crimson second hand.
- **Dynamic Resizing:** Clean, math-driven layout based on trigonometry.

## Prerequisites

To compile and run this project, you will need:
- A C Compiler (e.g., `gcc`, `clang`, or MSVC)
- **SDL3 Library** installed and configured on your system.

## Build and Run

### Windows (MinGW/GCC)
Assuming you have SDL3 properly set up in your include and lib paths, compile the `analog-clock.c` file:

```bash
gcc analog-clock.c -o clock.exe -I./include -L./lib -lSDL3
```
Then run the executable:
```bash
./clock.exe
```

### Linux / macOS
If using a package manager to install SDL3, you can typically compile using:

```bash
gcc analog-clock.c -o clock -lSDL3 -lm
```
Then run it:
```bash
./clock
```

## How It Works

1. **Window setup:** A 600x600 window is initialized using `SDL_CreateWindow`.
2. **Time Tracking:** Employs the standard `<time.h>` library to continually grab the system's `localtime`.
3. **Geometry Calculation:** 
    - Hours, minutes, and seconds are converted into angles.
    - Uses `SDL_sinf` and `SDL_cosf` to calculate the exact coordinate endpoints for the hands.
    - Hands and clock rings are rendered using custom `DrawThickLine` and `DrawCircleOutline` functions, generating standard vertex buffers (via `SDL_Vertex`) pushed to the GPU.

## License
Feel free to use and modify the code as a starting point for learning SDL3 graphics programming or to integrate a clock into your own desktop applications!
