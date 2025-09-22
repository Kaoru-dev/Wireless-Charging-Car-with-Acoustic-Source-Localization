# Wireless Charging Car with Acoustic Source Localization

## Project Overview

This project is a smart car that can autonomously locate an acoustic source, navigate to it, and perform high-efficiency wireless charging. The system's core is a custom-designed main control board based on the **Infineon AURIX TC377** microcontroller.

A key technical highlight is the implementation of a **constant-power charging system** for a supercapacitor. The system uses a composite control strategy combining **Feedforward and PID control** to maintain a stable charging power, achieving up to **55W**. This project demonstrates a full development cycle, from theoretical analysis and hardware design to embedded software development and system integration.

![A GIF demonstration of the wireless charging car in action.](./assets/test.gif)
## Key Features

  * **Acoustic Source Localization**: Uses a microphone array and a **cross-correlation algorithm** to accurately determine the direction of a sound source.
  * **Autonomous Navigation**: The car can autonomously navigate and dock at the identified sound source for charging.
  * **High-Efficiency Wireless Charging**: Features a robust constant-power charging algorithm.
  * **Integrated Hardware Design**: Developed a custom all-in-one main control board featuring the Infineon TC377 MCU and TI DRV8701 motor driver.

## Hardware & Tech Stack

### Hardware

  * **MCU**: Infineon AURIX TC377
  * **Motor Driver**: TI DRV8701
  * **Wireless Charging**: Custom-designed LCC resonant topology boards
  * **Power Electronics**: Buck half-bridge circuit for voltage regulation
  * **Sensors**: IM68A130A

### Software & Tools

  * **Language**: C
  * **Core Algorithms**: PID Control, Cross-Correlation, Feedforward Control
  * **IDE**: AURIX Development Studio
  * **PCB Design**: Altium Designer


## Technical Deep Dive: Constant-Power Charging System

To ensure efficient and safe charging of the supercapacitor, a constant-power charging strategy was implemented, based on the energy conservation formula $P\\cdot t=\\frac{1}{2}C\\cdot V^{2}$.

### Control Strategy

The system employs a composite control scheme for robust performance:

  * **Feedforward Control**: Based on the energy formula, the system pre-calculates the theoretical output voltage required at any given time to maintain constant power, providing a fast initial response.
  * **PID Closed-Loop Control**: The system continuously samples the actual output voltage and current using an ADC. The real-time power is calculated and compared against the target power. The resulting error is fed into a PID controller, which finely tunes the PWM output to compensate for external disturbances and system non-linearities.

### Hardware Implementation

  * **Sensing**: A resistor divider network samples the voltage, while a high-precision current sensing chip on a 20mΩ shunt resistor measures the current.
  * **Actuation**: The output of the control algorithm is a PWM signal that drives a **Buck half-bridge circuit**. By modulating the PWM duty cycle, the system precisely regulates the voltage delivered to the supercapacitor, thus controlling the charging power.

## Repository Structure

```
├── Circuit/                            # Contains all hardware-related design and planning files
│   ├── Circuit Design/                 # Altium Designer schematic (.SchDoc) and PCB (.PcbDoc) files
│   ├── Overall Plan.png                # High-level system architecture diagram
│   ├── Power Tree.jpg                  # Diagram of the power distribution system
│   └── Shopping List.xlsx              # Bill of Materials (BOM) for all components
├── Code/                               # Source code for the Infineon TC377 microcontroller (Eclipse-based project)
│   ├── src/AppSw/                      # Main application software source files (motor control, PID, etc.)
│   ├── Cpu0_Main.c                     # Main program loop for Core 0
│   ├── Cpu1_Main.c                     # Main program loop for Core 1
│   ├── Cpu2_Main.c                     # Main program loop for Core 2
│   ├── .project                        # Eclipse project configuration file
│   └── LCF_Tasking_Tricore_Tc.lsl      # Linker script file for memory mapping
└── README.md                           # This file
```



