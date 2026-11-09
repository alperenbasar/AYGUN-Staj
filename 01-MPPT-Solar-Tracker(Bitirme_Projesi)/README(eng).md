# MPPT-Analyzed Dual-Axis Solar Tracking System

This project is an Arduino-based electromechanical system designed to optimize the orientation of solar panels using LDR sensors and FT5325M servo motors. The system performs Maximum Power Point Tracking (MPPT) analysis by processing real-time data acquired through INA226 power sensors.

This study aims to autonomously track the Sun's horizontal and vertical movements across the sky, minimizing cosine losses and optimizing electrical load performance under laboratory conditions.

## Project Overview and Objectives
- **Dual-Axis Mechanical Design and Actuation System:** The panel's movement capability is designed to cover an azimuth range of 0°–180° and a tilt range of 0°–45°. A rotary turntable bearing is incorporated into the base to optimize weight distribution and reduce axial loading on the motor. Movement is provided by high-torque Feetech FT5320M digital metal-gear servo motors.
- **Autonomous Sensor and Algorithm Architecture:** Four LDR sensors mounted on the panel surface measure ambient light intensity in real time through the Analog-to-Digital Converter (ADC). The software operates using a differential control algorithm, with a 40-unit hysteresis threshold (deadband) to prevent oscillations and minimize mechanical wear. Movement commands are processed through stabilized control cycles with a 1000 ms delay to ensure smooth and stable operation.
- **MPPT and Power Analysis:** Current and voltage values are precisely monitored using an INA226 digital power monitor communicating via the I2C protocol. During laboratory experiments conducted under a constant light source, 20 different resistive load configurations were tested. Experimental results confirmed that the solar panel reached a Maximum Power Point (MPP) of 1.8296 W under a 73 Ω aluminum-housed power resistor load.

## Hardware and Components
- **Microcontroller:** Arduino Uno R3 (ATmega328, AVR Architecture)
- **Sensors:** LDR Light Sensors (4 units), INA226 Digital Current and Power Monitor
- **Actuators:** 2x Feetech FT5320M High-Torque Digital Servo Motors (Dual-Axis)
- **Solar Panel:** SolarMarket 25W 12V Monocrystalline Solar Panel
- **Power Management:** 3 Great Power INR18650 Rechargeable Lithium-Ion Batteries Connected in Series, XL4016 DC-DC Step-Down Voltage Regulator
- **Interface and Passive Components:** Arduino Uno Sensor Shield V5.0, 50W Aluminum-Housed Power Resistor

## Repository Structure
- `yazilim/`: Contains the Arduino embedded software source code (`.ino` files).
- `tez/`: Contains the undergraduate thesis (`.pdf`), detailing the mechanical design, theoretical calculations (MPPT, P-V and I-V curves), and hardware integration processes.
- `fotograflar/`: Contains SolidWorks 3D modeling images, manufacturing and assembly photographs, circuit diagrams, and laboratory test result graphs.
- `donanim/`: Contains the overall system architecture and Fritzing wiring diagram.

## Firmware Overview

The firmware reads ambient light intensity through four LDR sensors using the Analog-to-Digital Converter (ADC). The sensors are divided into two independent pairs according to the motor axes (A0-A1 for the first motor and A3-A2 for the second motor), and the panel's orientation is calculated directly based on the difference in light intensity between each sensor pair.

A differential control algorithm is implemented with a deadband threshold (`esikDeger = 40`) to prevent motor oscillations and minimize mechanical wear.

The panel's mechanical safety is ensured through conditional `if-else` statements, restricting movement within the defined angular limits of the first axis (0°–45°) and the second axis (0°–180°). Movement commands are executed in precise 1-degree increments, with a 1000 ms delay incorporated into the control loop to minimize mechanical vibrations and ensure stable operation.

Voltage and power measurements acquired from the INA226 sensor are transferred to the Arduino via the I2C communication protocol, enabling electrical performance monitoring and MPPT analysis.
