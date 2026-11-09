# G-Meter and Black Box Data Logger

This project is an embedded telemetry (black box) system designed to measure, visualize, and record vehicle dynamics or object movements in real time. Running on the STM32F429I-DISC1, the system displays acceleration data acquired from the MPU6050 sensor as a live "G-Meter (Radar)" on the screen while simultaneously recording the data to a MicroSD card in CSV format with millisecond-level timestamp precision.

## Project Overview and Key Features

- **Real-Time G-Meter (Radar) Interface:** Raw acceleration data (X and Y) acquired from the MPU6050 is smoothed using a software-based Low-Pass Filter and converted into a fluid dot animation on the TFT display.

- **Continuous Data Logging:** All acceleration data, including the Z-axis, is recorded to the SD card in the format Time(ms), AccX, AccY, AccZ (`karakutu.csv`).

- **Dynamic UI Notifications:** The interface features a blinking "REC" animation and status indicators displaying the real-time operating conditions of the modules (SD: OK/OFF, MPU: OK/ERR).

- **Fault-Tolerant Architecture (Fail-Safe):** Even if the SD card connection is lost or a module malfunctions during operation, the main loop remains responsive thanks to a lock-flag mechanism. The G-Meter and display continue operating smoothly at 25 FPS.

## Hardware and Wiring Diagram

- **Microcontroller:** STM32F429ZIT6 (STM32F429I-DISC1 Development Board)

- **Display:** Integrated 2.4" TFT LCD

- **Sensor:** MPU6050 Accelerometer & Gyroscope

- **Storage:** MicroSD Card Module (SPI Communication - FAT32)

Note: In this project, custom pin mapping was implemented instead of standard pin assignments to prevent hardware conflicts (DMA bus contention) between the onboard display's SDRAM and LTDC (LCD-TFT Display Controller) traffic and the SD card's SPI communication.

## System Architecture and Challenges Encountered

The most significant challenge overcome during the development of this project was the communication bottleneck within the bus matrix caused by the interaction between the LTDC driver and SPI communication.

**Problem:** In the default CubeMX configuration, the SD card's Chip Select (CS) pin conflicted with a pin allocated to the display memory (SDRAM), causing the system to freeze during the `f_mount` operation.

**Solution:**

- The CS pin was relocated to the fully isolated PE4 pin.
- Since CubeMX's code generator may omit SPI pin configurations (Alternate Function), manual `GPIO_InitStruct` blocks were added to `main.c` to force hardware initialization.
- The SPI interface was initialized independently before the LCD initialization function, permanently resolving the "SD: OFF" (Mount Error) issue.

## Installation and Execution

1. Format the MicroSD card as FAT32 on your computer.
2. Connect the hardware according to the wiring diagram above (Make sure the SD card module's VCC pin is connected to 5V rather than 3.3V).
3. Clone the repository and open the project using STM32CubeIDE.
4. Compile the code (Build) and flash it onto the microcontroller.
5. Once the system starts, if the SD card is successfully detected, the green `SD: OK` indicator will appear and the red `REC` animation will begin.
6. To stop recording, power off the system, remove the SD card, and connect it to your PC. The `karakutu.csv` file can then be visualized and analyzed using Excel or MATLAB.
