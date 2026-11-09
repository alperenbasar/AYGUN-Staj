# STM32 and HC-08 BLE Smartphone-to-PC Communication Bridge

This project is designed to establish a bidirectional wireless communication bridge between a mobile device (smartphone), an HC-08 Bluetooth Low Energy (BLE) module, and a PC terminal (Tera Term) via an STM32 microcontroller.

---

## Architecture and Data Flow

- **Mobile Input:** A character or command is transmitted wirelessly from a BLE serial terminal application on the smartphone to the HC-08 module.
- **Asynchronous Reception (Interrupt-Driven):** The HC-08 module forwards the data to the USART1 interface of the STM32. To prevent blocking the processor's main execution loop, 1-byte hardware interrupt listening is utilized via the `HAL_UART_Receive_IT` function.
- **Echo and Response (Callback):** Upon data arrival, the `HAL_UART_RxCpltCallback` function is triggered. The system transmits the processed text both to confirm successful data reception and to output a response onto the line.
- **Terminal Visualization:** By opening Tera Term via the Virtual COM Port (VCP) or an external serial line connected to the PC, incoming and outgoing data are verified in real time.

---

<h2>Hardware and Communication Parameters</h2>

- **Microcontroller:** STM32F429ZIT6
- **HC-08 BLE Interface (USART1):** 9600 Baud, 8 Data Bits, No Parity, 1 Stop Bit (Interrupt-driven)
- **Secondary Serial Interface (USART3):** 115200 Baud, 8 Data Bits, No Parity, 1 Stop Bit

**Feedback Response:** `alperen\r\n`
