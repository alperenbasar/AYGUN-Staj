# Projects

---

## Hardware and Tools Used
- **Microcontrollers & Development Boards:** STM32F429I-DISC1 (ARM Cortex-M4 @ 180 MHz), Microchip PIC12F1822, Arduino (AVR)
- **Circuit Design & Prototyping (EDA):** Altium Designer (v26), KiCad, LPKF S63 PCB Milling Machine (CNC Prototyping), SMD/THT Assembly and Soldering
- **Software & Development Environments:** C/C++, Python (Tkinter, pySerial), STM32CubeIDE (HAL Libraries, FatFs), MPLAB X IDE (XC8 Compiler), PICkit 3, Tera Term
- **Sensors & Actuators:** MPU6050 (Accelerometer and Gyroscope), INA226 (Precision Power Monitor), LDR Sensors, FT5325M Servo Motors
- **Communication, Power & Interface Modules:** HC-08 Bluetooth Low Energy (BLE), MicroSD Card Module (SPI), CN3303 Lithium Battery Charge Controller, 2.4" QVGA TFT LCD (LTDC), STMPE811 Touch Controller

---

## Project Index

1. [01-MPPT-Solar-Tracker (Capstone Project)](./01-MPPT-Solar-Tracker(Bitirme_Projesi))
   - Design of an Arduino-based dual-axis electromechanical system that orients solar panels toward the light source for maximum efficiency, using LDR sensors and FT5325M servo motors.
   - Real-time processing of instantaneous current and voltage data with INA226 precision power sensors, software-based MPPT (Maximum Power Point Tracking) analysis, and system efficiency optimization.
   - End-to-end completion of the embedded firmware, motor control logic, and hardware integration.

2. [02-PIC12F1822-Charge-Indicator-PCB-Design-and-Fabrication](./02-PIC12F1822-Sarj-Gostergesi-PCB-Kart-Tasarimi-ve-Basimi)
   - A smart charge status indicator and control board with a 3-stage (Red-Yellow-Green) cumulative fill animation, designed around the Microchip PIC12F1822 (SOIC-8) and fully compatible with the CN3303 lithium charger IC.
   - Schematic design in KiCad and single-layer PCB layout suitable for the LPKF S63 milling machine. Hardware isolation was achieved with a high-impedance voltage divider (22K/100K) to prevent leakage currents and LED conflicts at the charger module's open-drain output.
   - Unstable voltage levels on the STAT pin were read via ADC instead of a digital GPIO. To eliminate signal noise and LED flicker caused by the charger IC's mode transitions, a software voltage threshold and a 1-second time-based stabilization (debounce) algorithm were developed.

3. [03-Smart-PC-Lock-System](./03-Akilli-PC-Kilit-Sistemi)
   - Embedded password authentication system running on the STM32F429I touchscreen.
   - Blocking/unblocking computer access via a Python-based full-screen overlay.
   - Real-time status telemetry transmitted to a mobile device via the HC-08 BLE module.

4. [04-Smart-Security-System-with-STM32-and-MPU6050](./04-STM32-MPU6050-ile-Akilli-Guvenlik-Sistemi)
   - Multi-layer alarm system performing precise anomaly (motion/vibration) detection with the MPU6050 accelerometer/gyroscope sensor.
   - Alert transmission to a PC over the serial port upon a breach, plus mobile push notification integration to a smartphone via the Notify API.
   - A hardware-password alarm disarm mechanism, using the onboard touch TFT display as a numpad interface.

5. [05-STM32-G-Meter-and-Black-Box-Data-Logger](./05-STM32-GMeter-ve-Karakutu-Veri-Kaydedici)
   - MPU6050 acceleration data processed with a Low-Pass Filter and rendered as a real-time "G-Meter" (radar) interface on the display at 25 FPS.
   - Continuous logging of vehicle dynamics data (Time, AccX, AccY, AccZ) to a MicroSD card (FAT32) as a `.csv` file over the hardware SPI bus (Data Logging).
   - Non-blocking resolution of bus matrix congestion between the display's LTDC/SDRAM DMA traffic and the SD card's SPI read/write cycles through hardware pin isolation (dedicated CS pin allocation).

6. [06-BLE-Triggered-and-PC-Controlled-Hybrid-LED-Automation-System](./06-BLE-Tetiklemeli%20ve%20PC-Kontrollu-Hibrit-LED-Otomasyon-Sistemi)
   - State machine architecture triggered by Bluetooth commands from a smartphone.
   - Dynamic menu rendering on the PC terminal, with internal/external LED control based on keyboard input.
   - `ORE` (Overrun Error) handling to prevent serial port lockups.

7. [07-STM32F429-Onboard-TFT-LCD-3D-Starfield-Graphics-Simulation](./07-STM32F429--Dahili-TFT-LCD%20ile%203D-Starfield-Grafik-Simülasyonu)
   - Projection of 3D spatial coordinates ($X, Y, Z$) onto the screen through 2D perspective projection.
   - A 70-particle starfield simulation using a dynamic pixel-clearing and drawing technique on external SDRAM that prevents screen tearing (flicker).

8. [08-STM32F429I-DISC1-Onboard-TFT-LCD-First-Drive-Test (BSP Hello World)](./08-STM32F429I-DISC1-Dahili-TFT-LCD-Ekran-İlk-Surus-Denemesi%20(BSP%20Hello%20World))
   - Bringing up the onboard 2.4" TFT LCD through the BSP driver layer ("Hello World" test).
   - Verification of the maximum core frequency (180 MHz Over-Drive) and the basic clock/power configuration.

9. [09-STM32-Hardware-Timer-PWM-Smooth-LED-Brightness-Control](./09-STM32-Donanimsal-Timer-PWM%20ile%20Puruzsuz-LED-Parlaklik-Kontrolu)
   - Hardware Pulse Width Modulation (PWM) signal generation via `TIM1 Channel 1`.
   - A fluid, flicker-free "Breathing" visual effect at a high resolution of 1000 steps.

10. [10-Phone-PC-Communication-Bridge](./10-Telefon-PC-Haberlesme-Koprusu)
    - Bidirectional serial communication bridge between a smartphone (BLE Terminal) and a computer (Tera Term).
    - Interrupt-driven infrastructure (`HAL_UART_Receive_IT`) and callback handling for asynchronous data reception.

11. [11-STM32-12-Bit-ADC-Reading-Real-Time-PWM-Brightness-Control](./11-STM32-12-Bit-ADC-Okumasi%20ile%20Gercek-Zamanli-PWM-Parlaklik-Kontrolu)
    - Reading analog voltage from a potentiometer at 12-bit resolution (0–4095) via `ADC1`.
    - Mathematical scaling of the analog value to the `TIM3` PWM duty cycle for real-time LED brightness adjustment.
