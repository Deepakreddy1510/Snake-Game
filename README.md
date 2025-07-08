# Terminal-Based Snake Game in C++

A real-time, terminal-based Snake game implemented in modern C++ using only standard libraries and low-level Linux system calls — no external libraries required.

---

## Features

- Real-time snake movement in the terminal
- Continuous gameplay with non-blocking keyboard input
- Snake grows by eating randomly placed fruit
- Wrap-around movement at screen borders
- Clean UI drawn using ASCII characters
- Linux-only, fully standalone (no dependencies)

---

## Technologies & Concepts Used

- **Language**: C++ (C++11 or later)
- **Data Structures**: `deque` for snake body, `set` for constant-time collision detection
- **Terminal Control**:  
  - `termios.h` for disabling canonical input (The terminal waits until you press Enter before sending your input to the program) and echo  (What you type appears on the screen)
  - `fcntl.h` for making keyboard input non-blocking

---

## What This Project Demonstrates

- Designed and implemented a **real-time Snake game** using C++ with efficient data structures like `deque` and `set` for smooth gameplay and O(1) collision checks.
- Integrated terminal control using **low-level Linux system calls** (`termios`, `fcntl`) to handle non-blocking keyboard input, enabling **continuous movement and responsive controls** without any external libraries.
- Reinforces understanding of **system programming**, **STL containers**, and **game loop logic** in a resource-constrained environment (terminal).

---

## Controls

| Key     | Action         |
|---------|----------------|
| `W`     | Move up        |
| `A`     | Move left      |
| `S`     | Move down      |
| `D`     | Move right     |
| `X`     | Exit game      |

---

##  How to Run

### Requirements
- Linux/Unix-based terminal
- g++ compiler

### Build & Run

```bash
g++ -o snake_game snake_game.cpp
./snake_game
