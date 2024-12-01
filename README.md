# Timer Project using ATmega32 Microcontroller

This project implements a **timer** with two modes: **Count-Up** and **Count-Down**, using the ATmega32 microcontroller from the AVR family. It demonstrates efficient use of hardware peripherals and embedded systems concepts. The project is fully modular, includes a Proteus simulation for visualization, and features well-documented code with detailed comments.

---

## Features
- **Modes**: Count-Up and Count-Down.
- **Peripherals**:
  - **6 Seven-Segment Displays** working in multiplexed mode.
  - **10 Buttons** for:
    - Resetting, pausing, resuming, and switching modes.
    - Incrementing and decrementing seconds, minutes, and hours.
  - **Buzzer** that rings when the set time is reached.
- **Code Structure**:
  - Fully modular and scalable.
  - Clean and well-commented for better readability and maintenance.
- **Simulation**:
  - Includes a **Proteus simulation** for testing and visualization.

---

## Key Learnings
By exploring this project, you can gain insights into:
- **Microcontroller Programming**:
  - Developing timer applications using the ATmega32 microcontroller.
- **Modular Code Design**:
  - Structuring code into reusable and maintainable modules.
- **Hardware Peripherals**:
  - Managing displays, buttons, and buzzers in embedded systems.
- **Embedded Systems Concepts**:
  - Configuring and using hardware timers.
  - Handling real-time events with interrupts.
  - Implementing debouncing techniques for buttons.
  - Multiplexing seven-segment displays to save on the number of pins used
- **Simulation and Testing**:
  - Visualizing and verifying functionality using Proteus.

---

## Hardware and Software Requirements
### **Hardware**:
- ATmega32 Microcontroller
- 6 Seven-Segment Displays
- 10 Push Buttons
- Buzzer
- Necessary resistors, and connecting wires
- Breadboard or PCB

### **Software**:
- **AVR-GCC Compiler** or any compatible IDE for coding.
- **Proteus Design Suite** for simulation.

---

## Getting Started
1. Clone this repository:
   ```bash
   git clone https://github.com/OmarAshraf118/stopWatch_timer_project_using_atmega32.git
