# BLE-Triggered and PC-Controlled Hybrid LED Automation System

This project implements a bidirectional interactive control architecture based on a state machine, operating between a mobile device (smartphone), an HC-08 BLE module, an STM32F429 microcontroller, and a computer terminal (Tera Term).

### Operating Principle and Control Flow

1. **Mobile Triggering (HC-08 / USART6):**
   - Characters transmitted from the smartphone are received byte by byte into a buffer using the `HAL_UART_Receive_IT` interrupt mechanism.
   - When a line termination character (`\r` or `\n`) is received, the incoming string is validated. If the received command is `lamba`, the system transitions to the next control state.

2. **Terminal Redirection (VCP / USART1):**
   - Once the command is detected, a selection menu is transmitted to the PC-side serial terminal (Tera Term), and the system waits for user input (`pc_bekleniyor = 1`).

3. **Hardware Output Control:**
   - Pressing the `1` key on the PC keyboard activates the external LED pin.
   - Pressing the `2` key activates the onboard LED pin on the development board.

4. **Error and Overrun Handling:**
   - To prevent potential buffer overflows and communication errors, the `HAL_UART_ErrorCallback` function automatically clears `ORE` (Overrun Error) flags, ensuring continuous asynchronous reception.
