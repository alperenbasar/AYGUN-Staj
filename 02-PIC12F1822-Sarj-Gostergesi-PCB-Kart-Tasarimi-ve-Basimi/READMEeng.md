# 02-PIC12F1822-Charging-Status-Indicator-PCB-Design-and-Fabrication

This repository contains the end-to-end design, prototyping, hardware signal isolation, PCB layout, and embedded software development processes for a custom charging status indicator controller based on the Microchip PIC12F1822 8-bit microcontroller, fully compatible with the CN3303 lithium battery charging IC.

This project was developed to meet the engineering requirements of a newly designed hardware product by providing a visual cumulative charging animation (Red-Yellow-Green) and a battery charging status indicator.

---

## Development Stages and Hardware Solutions

1. **Requirements and Schematic Design:** Input signals (`STAT` charging signal) and output requirements were identified. The schematic design was completed using KiCad.
2. **Signal Isolation and Prototyping:** During the initial prototyping stage, leakage currents and LED interference caused by the microcontroller's digital input and pull-up resistors were identified at the open-drain output of the charging module (CN3303). To resolve this hardware-related locking issue, complete electrical isolation was achieved by implementing a high-impedance voltage divider (22K series resistor, 100K pull-down resistor) on the input line.
3. **PCB Layout and Fabrication:** A compact, single-sided PCB was designed to comply with in-house prototyping processes using the LPKF milling and engraving method. Following testing, the hardware was assembled and brought into operation through manual laboratory soldering.
4. **Embedded Software and ADC Filtering:** To eliminate signal noise caused by voltage fluctuations during the CN3303 battery detection mode, the `STAT` pin was monitored using the ADC (Analog-to-Digital Converter) instead of a digital GPIO input. Using MPLAB X IDE and the XC8 compiler, software hysteresis (voltage thresholds), 1-second time-based stabilization (debounce) algorithms, and a State Machine architecture were implemented in C.

---

## Hardware Specifications

*   **Microcontroller:** Microchip PIC12F1822 (SOIC-8)
*   **Oscillator:** Internal 4 MHz HFINTOSC (No external crystal oscillator required)
*   **Input Interface (Analog):** The signal from the CN3303 `STAT` line is connected to the RA0 pin through a 22K (R4) series resistor and a 100K (R5) pull-down resistor to prevent leakage currents.
*   **Output Interface:** Three-stage status LEDs (Red, Yellow, Green), driven through 470 Ω resistors via the `RA5`, `RA4`, and `RA2` pins.
*   **Programming Interface:** An integrated ICSP (In-Circuit Serial Programming) header enabling direct on-board debugging and firmware programming using PICkit 3. *(Note: The RA3/MCLR pin has been excluded from software-based signal reading due to hardware isolation requirements).*

---

## Repository Structure

*   `Firmware/`: Contains MPLAB X C source code (`main.c`) and production HEX files.
*   `Hardware/`: Contains KiCad schematic (`.kicad_sch`) and PCB layout (`.kicad_pcb`) files.
*   `Images/`: Contains photographs documenting breadboard tests, the milled PCB, hardware modifications, and final assembly.

---

## Firmware Overview

The firmware reads the input signal in analog (ADC) mode to prevent hardware noise and unstable state transitions (e.g., oscillations occurring when no battery is connected). It manages the cumulative charging animation based on the filtered signal using predefined hysteresis thresholds (`ADC_CHARGE_MIN` and `ADC_FULL_MAX`) and 1-second time-based filtering. Thanks to the State Machine architecture, the microcontroller remains responsive and does not become blocked during the animation process.

10-PIC12F1822-Charging-Status-Indicator-PCB-Design-and-Fabrication/Software/main.c
