# STM32-Based Secure PC Lock and BLE Notification System

This project is a two-stage physical security and authentication system developed using the STM32F429I-DISC1 development board, an HC-08 BLE module, and Python-based host computer software. When the lock button on the display connected to the STM32 is pressed, the computer is locked. To unlock it, the correct password must be entered through the touchscreen interface. The password verification process is handled directly by the microcontroller hardware rather than the operating system.

## Key Features

- **Hardware-Based Authentication:** Password verification is performed entirely by embedded software running on the STM32 microcontroller.

- **Touchscreen Interface:** A numerical keypad (Numpad) is provided through the onboard 2.4" TFT LCD display and STMPE811 touchscreen controller.

- **Full-Screen Lock Shield (Python):** The lock shield, developed using Tkinter, covers the entire screen and blocks mouse and keyboard access upon receiving a LOCK command from the development board. The screen is restored when the correct password is entered and an UNLOCK command is received.

- **Wireless Security Notifications (HC-08 BLE):** Lock status and password attempts are transmitted to a mobile phone as real-time notifications through the HC-08 module connected to the UART5 interface.

## Hardware and Connections

- **STM32F429I-DISC1:** Main controller and touchscreen interface (FreeRTOS-based architecture).

- **USART1 (115200 Baud):** Transmits lock commands to the computer through the ST-LINK Virtual COM Port.

- **UART5 (9600 Baud):** Transmits security telemetry to a mobile phone through the HC-08 Bluetooth module.

## Usage

Upload the STM32 firmware to the development board and connect the board to the computer.

Install the `pyserial` library on the computer (`pip install pyserial`).

Configure the COM port in `akilli_kilit.py` and run the application (`python akilli_kilit.py`). (Emergency exit: ESC)
