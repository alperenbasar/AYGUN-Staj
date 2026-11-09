# MPU6050-Based Smart Security System (Smart Security System)

This project is a multi-layered smart security and alarm system developed using the STM32F429I-DISC1 development board and an MPU6050 accelerometer/gyroscope sensor. Whenever physical movement (such as unauthorized access, theft, or vibration) is detected, the system activates both local and remote alert mechanisms. The alarm can only be deactivated by entering the correct password through the STM32 touchscreen (LCD).

## Project Overview and Key Features

- **Precise Motion Detection:** Detection of physical tampering and unauthorized movement within milliseconds using the MPU6050 sensor.

## Multi-Layer Alert System:

- **Visual Alarm:** An external LED flashes at a high frequency upon alarm activation, creating a strobe effect.

- **Local Alert:** An immediate "Motion Detected!" warning is displayed on the PC terminal/Serial Monitor.

- **Mobile Push Notification:** Emergency notifications are delivered to a smartphone within seconds through Notify API integration.

- **Touchscreen Password Panel (Numpad):** The onboard TFT LCD display of the STM32 is used as a numerical keypad interface. The alarm can only be deactivated by entering the correct PIN code.

## Hardware and Components

- **Microcontroller:** STM32F429ZIT6 (STM32F429I-DISC1 Development Board)

- **Display:** Integrated 2.4" TFT LCD Touchscreen (for Numpad interface)

- **Sensor:** MPU6050 (6-Axis Accelerometer and Gyroscope)

- **Output Unit:** External LED Module

### Communication:

- I2C (Communication between the MPU6050 and STM32)

- UART (Transmission of alert messages to the PC and communication with the mobile notification module)

## System Architecture and Flow Algorithm

- **Monitoring Mode:** The MPU6050 sensor continuously reads acceleration and angular changes along the X, Y, and Z axes.

- **Triggering:** When movement exceeding the predefined threshold value is detected, the system switches to "Alarm" mode.

### Actions:

- The external LED starts flashing rapidly.

- The system sends the `\[WARNING] Motion detected!` message to the PC via the serial port.

- A notification is triggered on the smartphone through the Notify service.

**Deactivation:** The Numpad interface appears on the LCD screen. The alarm remains active until the user enters the correct password through the touchscreen. Once the correct password is entered, the LED turns off, a "System Deactivated" notification is sent to both the PC and smartphone, and the system waits before returning to monitoring mode.
