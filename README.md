# A* Pathfinding Visualization on LED Matrix and LCD using Arduino

## Project Overview

This project implements the A* pathfinding algorithm to find the shortest path between various landmarks in **Imphal, Manipur**, visualizing the path on an LED matrix and displaying the total cost of the path on an LCD. The project integrates real-time heuristic calculations using latitude and longitude data, serial communication between C++ and Arduino, and hardware control through shift registers and microcontrollers.

---

## Table of Contents

1. [Features](#features)
2. [Hardware Components](#hardware-components)
3. [Software Components](#software-components)
4. [System Design](#system-design)
5. [Graph and Landmarks in Imphal](#graph-and-landmarks-in-imphal)
6. [Installation](#installation)
7. [Usage](#usage)
8. [Serial Communication](#serial-communication)
9. [Contributing](#contributing)
10. [License](#license)

---

## Features

- **A* Algorithm Implementation:** Efficient pathfinding using the A* algorithm with real-time latitude and longitude for heuristic calculation.
- **Dual Arduino Setup:** Two Arduino boards communicate via serial ports, one controlling the LED matrix (COM7) and the other controlling the LCD (COM8).
- **LED Path Visualization:** LEDs light up sequentially along the path found by the A* algorithm, with a delay between each LED for visual effect.
- **LCD Cost Display:** The total cost of the path is shown on a 16x2 LCD display.
- **Real-Time Heuristic Calculation:** Straight-line distances between nodes (locations in Imphal) are calculated using actual geographical coordinates.

---

## Hardware Components

- **Arduino Uno (x2):** One for controlling the LED matrix and another for controlling the LCD.
- **LED Matrix:** Displays the nodes in the city and lights up to show the shortest path.
- **16x2 LCD Display:** Shows the total cost of the shortest path.
- **Shift Register:** Used to control the LED matrix with fewer GPIO pins.
- **Connecting Wires, Breadboard, Power Supply.**

---

## Software Components

- **C++ Program:** Implements the A* algorithm and communicates with the Arduino boards over serial communication (COM7 and COM8).
- **Arduino Code (x2):**
  - **COM7:** Controls the LED matrix based on the path data received from C++.
  - **COM8:** Displays the path cost on the LCD.

- **Libraries:**
  - Arduino `LiquidCrystal` library for LCD control.
  - Custom C++ code for serial communication using the Win32 API.

---

## System Design

The project follows a modular design to separate software and hardware components:

1. **A* Algorithm:**
   - Computes the shortest path between 20 key locations in Imphal, Manipur.
   - Uses real-time latitude and longitude to calculate heuristic values (`h(n)`).
   
2. **Serial Communication:**
   - C++ sends path data to Arduino via COM7 for LED matrix control.
   - Simultaneously, the path cost is sent to Arduino via COM8 for LCD display.

3. **LED Matrix:**
   - Controlled via a shift register connected to Arduino.
   - Lights up sequentially to show the nodes in the path.

4. **LCD Display:**
   - Displays the total path cost calculated by the A* algorithm.

---

## Graph and Landmarks in Imphal

The A* algorithm is applied to 20 key locations in **Imphal, Manipur**, which are represented as nodes in a graph. The locations include:

1. Mantripukhri
2. Imphal Airport
3. Khuman Lampak
4. Uripok
5. Lamlong
6. Singjamei
7. Porompat
8. Lilong
9. Thangmeiband
10. Moirangkhom
11. Langol
12. Nambol
13. Kakwa
14. Andro
15. Sekmai
16. Khurai
17. Thoubal
18. Bishnupur
19. Lamdan
20. Lamlong

The graph represents the geographical layout of the city and includes the **Euclidean distance** (straight-line distance) between each location based on real-time **latitude and longitude** data.

- The **heuristic function** used in the A* algorithm is based on this geographical data to calculate the shortest path more effectively.
- The project highlights local navigation challenges and provides a detailed visual representation of the A* algorithm's pathfinding capabilities.

---

## Installation

1. **Clone the repository:**
   ```bash
   git clone https://github.com/your-username/a-star-pathfinding-imphal.git
   cd a-star-pathfinding-imphal
