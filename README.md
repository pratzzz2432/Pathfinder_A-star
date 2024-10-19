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

   # Arduino Setup

- Upload the provided `LED_matrix.ino` file to the Arduino controlling the LED matrix.
- Upload the `LCD_display.ino` file to the second Arduino controlling the LCD display.

## C++ Program Setup

Compile and run the C++ code using MinGW or your preferred compiler:

```bash
g++ astar.cpp -o astar.exe
The C++ program will communicate with the Arduinos using COM7 for the LED matrix and COM8 for the LCD.

##Circuit Configuration
Connect the LED matrix to the first Arduino using a shift register and follow the pin mappings specified in the LED_matrix.ino file.
Connect the LCD display to the second Arduino and follow the pin mappings specified in the LCD_display.ino file.
Usage
Run the C++ Program
After successful compilation, execute the program to calculate the shortest path between two locations in Imphal using the A* algorithm. You will be prompted to select a starting node and a destination node from the 20 locations in Imphal.

##Path Visualization on LED Matrix
The LEDs will light up sequentially, showing the shortest path calculated by the algorithm. Each LED represents a location on the graph. The LED path visualization will happen in real-time, with a delay between lighting up each LED, so users can easily follow the progression of the path.

##Display Path Cost on LCD
Simultaneously, the total path cost (g(n) + h(n)) will be displayed on the 16x2 LCD connected to the second Arduino. This gives users a real-time display of the total cost involved in reaching the destination.

##Verify Data Flow
Check the serial monitor for confirmation messages from both Arduino boards (COM7 for LEDs and COM8 for LCD), indicating successful reception of data.
Ensure that the LEDs light up in the correct sequence and the LCD displays the correct total cost.
Adjust Path Nodes
You can rerun the program to select different start and destination nodes from the graph of Imphal to observe the shortest path for different routes.

##System Architecture
A Pathfinding:* The heart of the system. The A* algorithm finds the most efficient route from a start point to an end point by considering various paths and choosing the one with the lowest cost. It's efficient and commonly used in applications like video games and robotics.

Send Path Data to COM 7 (LED Matrix): After the A* algorithm finds the path, this process sends the path data to the LED matrix via serial communication. COM 7 is the port used for this data transmission.

Send Total Cost to COM 8 (LCD): This process sends the total cost of the path (which is a measure of the path’s efficiency) to the LCD. COM 8 is the port used for this communication.

Display on LEDs: This process handles displaying the path data on the LED matrix. It ensures the visual representation of the path is accurate and user-friendly.

Display on LCD: This process handles displaying the total cost on the LCD screen. It ensures that the total cost is clear and readable.

Serial Communication (COM 7 & COM 8): These modules handle communication between the A* algorithm and the microcontrollers (Arduino). COM 7 is dedicated to the LED matrix, while COM 8 is for the LCD.

Microcontroller (Arduino for LED & LCD): Arduinos receive data from the serial communication modules and pass it on to the respective displays. One Arduino controls the LED matrix, and another controls the LCD.

LED Matrix: The hardware component where the path data is displayed. It lights up LEDs representing the path found by the A* algorithm.

LCD: The hardware component where the total cost is displayed. It shows a numerical value indicating the efficiency of the path found.

A Algorithm Module:* This module contains the logic for the A* algorithm. It computes the shortest path and determines the total cost.

Shift Register: A component that helps manage the multiple LEDs in the matrix. It’s essential for handling the high number of individual LED controls without requiring an equal number of output pins from the Arduino.

LED Control Module: This module interfaces between the Arduino and the LED matrix, ensuring the correct LEDs light up according to the path data.

LCD Control Module: This module interfaces between the Arduino and the LCD, ensuring the total cost is displayed correctly.

Together, these parts create a comprehensive system that efficiently finds a path, calculates its cost, and displays the information on both LED and LCD screens.

##Troubleshooting
Incorrect LED Sequence
Ensure that the Arduino controlling the LED matrix is correctly connected to the shift register and that the COM7 port is active.
Check if the C++ program is sending the correct path data.
No Output on LCD
Verify the connection between the second Arduino and the LCD (COM8).
Ensure the baud rate in the Arduino code matches the one set in the C++ program.
Path Data Not Displayed
Double-check the serial communication setup in the C++ code. Ensure the Win32 API is correctly configured to communicate with the respective COM ports (COM7 for LED, COM8 for LCD).
COM Port Issues
Make sure both Arduino boards are properly connected to your PC, and the COM ports are correctly assigned in the C++ code. You can use the Arduino IDE’s Serial Monitor to test if the correct COM ports are functioning.
Future Improvements
Expand Graph: The current project includes 20 key locations in Imphal, but more locations can be added to improve the map coverage.
Real-Time GPS Integration: In the future, integrating real-time GPS data can enhance the heuristic calculation, making the navigation system more dynamic and accurate.
Optimization: Explore improvements in data transfer speed, LED visualization effects, and the accuracy of the A* heuristic function by using advanced algorithms.
Smart City Integration: This project can serve as a foundation for integrating intelligent transportation systems into smart city projects, helping to visualize and solve real-time traffic issues in cities like Imphal.
