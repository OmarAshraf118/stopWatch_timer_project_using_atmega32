# Timer Project using ATmega32 Microcontroller

This project implements a **timer** with two modes: **Count-Up** and **Count-Down**, using the ATmega32 microcontroller from the AVR family. It demonstrates efficient use of hardware peripherals and embedded systems concepts. The project is fully modular, includes a Proteus simulation for visualization, and features well-documented code with detailed comments.

---

## Features

- **Modes of Operation**:
  - **Count-Up Timer**.
  - **Count-Down Timer** with a buzzer alert when the timer reaches zero.
- **Peripherals**:
  - **6 Multiplexed Seven-Segment Displays** for time representation.
  - **10 Buttons**:
    - **Reset** the timer.
    - **Pause** and **Resume** the timer.
    - **Toggle Mode** (Count-Up or Count-Down).
    - Increment/Decrement **Hours**, **Minutes**, and **Seconds**.
  - **Buzzer** for audible alerts.
  - **Two LEDs** to indicate the active mode:
    - Red LED for Count-Up mode.
    - Yellow LED for Count-Down mode.
- **Code Design**:
  - Fully modular and scalable.
  - Clean and well-commented for better readability and maintainability.
- **Simulation**:
  - Includes a **Proteus simulation** for testing and visualization.

---

## Key Learnings

By exploring this project, you can gain insights into:

### **Microcontroller Programming**:
- Developing timer applications using the ATmega32 microcontroller.

### **Modular Code Design**:
- Structuring code into layers:
  - **MCAL (Microcontroller Abstraction Layer)**: Low-level hardware drivers.
  - **HAL (Hardware Abstraction Layer)**: Peripheral interfaces.
  - **Application Layer**: High-level program logic.
- Using **functions** and reusable code modules for maintainability.

### **Hardware Peripherals**:
- Efficiently managing displays, buttons, and buzzers.
- Implementing **multiplexing** for seven-segment displays to minimize pin usage.

### **Embedded Systems Concepts**:
- Configuring and utilizing hardware **timers** for precise time tracking.
- Implementing **button debouncing techniques** to avoid false triggers.
- Managing **real-time events** with interrupts.

### **Simulation and Testing**:
- Visualizing and verifying functionality using Proteus simulation tools.

---

## Hardware and Software Requirements

### **Hardware**:
- ATmega32 Microcontroller
- 6 Seven-Segment Displays
- 10 Push Buttons
- Buzzer
- Resistors, LEDs, and connecting wires
- Breadboard or PCB for assembly

### **Software**:
- **AVR-GCC Compiler** or any compatible IDE for coding.
- **Proteus Design Suite** for simulation.

---

## Getting Started

1. Clone this repository:
   ```bash
   git clone https://github.com/OmarAshraf118/stopWatch_timer_project_using_atmega32.git
