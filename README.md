# 🧮 The Race to Pi

[![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat-square&logo=c%2B%2B)](https://isocpp.org/)
[![Raylib](https://img.shields.io/badge/Graphics-Raylib-orange.svg?style=flat-square)](https://www.raylib.com/)
[![Boost](https://img.shields.io/badge/Math-Boost.Multiprecision-red.svg?style=flat-square)](https://www.boost.org/)

A high-performance, visually stunning C++ simulation that pits classic and modern mathematical algorithms against each other in a race to calculate **Pi** with **1,000+ decimal places** of precision.

---

## 🎬 Live Demo

Experience the convergence in real-time. Watch how different mathematical approaches tackle the infinite complexity of Pi.

<video src="public/pi_race_20260509_051309.mp4" width="100%" controls autoplay loop muted></video>

---

## 🚀 Features

-   **High Precision Engine**: Powered by `Boost.Multiprecision` for arbitrary-precision arithmetic (1,050 bits).
-   **Real-time Visualization**: Hardware-accelerated rendering using `Raylib`.
-   **Algorithm Comparison**: Simultaneous execution of 4 legendary algorithms.
-   **Integrated Recorder**: Capture your simulations directly to MP4 using an FFmpeg pipe.
-   **Interactive Controls**: Toggle algorithms, adjust simulation speed, and reset state on the fly.
-   **Clean Architecture**: Fully SOLID-compliant codebase for easy extensibility.

---

## 🧬 The Competitors

| Algorithm | Type | Convergence Rate | Historical Context |
| :--- | :--- | :--- | :--- |
| **Gauss-Legendre** | Iterative | **Quadratic** (Doubles digits/step) | Used for world records in the late 20th century. |
| **Chudnovsky** | Series | **~14 digits/step** | The current gold standard for world record attempts. |
| **Ramanujan** | Series | **~8 digits/step** | A brilliant formula discovered by Srinivasa Ramanujan. |
| **Machin (arctan)** | Taylor | **Linear** (~1.4 digits/step) | The classic method used for centuries (1706). |

---

## 🛠️ Installation & Setup

### Prerequisites

You will need a C++17 compiler and the following libraries:

-   **Boost** (Multiprecision)
-   **Raylib** (Graphics)
-   **FFmpeg** (Optional: for video recording)

#### macOS (Homebrew)
```bash
brew install boost raylib ffmpeg
```

### Building the Project

The project includes a robust `Makefile` for easy compilation:

```bash
# Compile the visualizer
make visualize

# Run the simulation
./visualize
```

---

## 🎮 Controls

| Action | Control |
| :--- | :--- |
| **Play / Pause** | `SPACE` or Click **"RUN SIMULATION"** |
| **Reset** | `R` or Click **"RESET STATE"** |
| **Record Demo** | Click **"START RECORDING"** (Requires FFmpeg) |
| **Speed Up** | `UP ARROW` (Reduces step interval) |
| **Slow Down** | `DOWN ARROW` (Increases step interval) |
| **Toggle Algorithms** | Keys `1`, `2`, `3`, `4` or Sidebar Toggles |

---

## 🏗️ Architecture (SOLID)

This project is built with a modular, professional codebase following SOLID principles:

```text
.
├── Makefile
├── README.md
└── src
    ├── algorithms      # Convergence logic (Gauss-Legendre, Chudnovsky, etc.)
    ├── app             # Main application orchestrator
    ├── core            # Types, constants, and global layout settings
    ├── recording       # FFmpeg pipe management for MP4 export
    ├── rendering       # Raylib-based visualization logic
    ├── simulation      # Algorithm state and timing management
    └── utils           # High-precision formatting helpers
```

-   **S (Single Responsibility)**: Rendering, Recording, and Simulation logic are strictly decoupled.
-   **O (Open/Closed)**: Add new algorithms by implementing the `IPiAlgorithm` interface without touching the core engine.
-   **L (Liskov Substitution)**: All algorithm implementations are interchangeable.
-   **I (Interface Segregation)**: Minimal, focused interfaces for algorithm runners.
-   **D (Dependency Inversion)**: The Application orchestrator depends on abstractions, not concrete implementations.

---

## 📜 License

This project is open-source. Feel free to use it for educational purposes or to find even more digits of Pi!
