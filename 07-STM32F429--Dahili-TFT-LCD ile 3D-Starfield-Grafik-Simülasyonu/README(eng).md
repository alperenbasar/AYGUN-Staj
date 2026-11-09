# STM32F429 Internal TFT LCD 3D Starfield Graphics Simulation

This project was developed to implement 3D space projection and real-time dynamic pixel manipulation using the onboard 2.4-inch QVGA TFT LCD display and external SDRAM architecture of the **STM32F429I-DISC1** development board.

### Operating Principle and Mathematical Model

- **3D-to-2D Perspective Projection:**
  A total of 70 particles with $(X, Y, Z)$ coordinates are defined in three-dimensional space. In each frame, the Z-depth value is decreased to simulate stars approaching the viewer. The 2D screen coordinates are calculated using the perspective projection formula:

  $$S_x = \frac{X \cdot d}{Z} + C_x, \quad S_y = \frac{Y \cdot d}{Z} + C_y$$

- **Flicker-Free Rendering:**
  Instead of clearing the entire screen, only the previous positions of moving stars (`prev_sx`, `prev_sy`) are erased in black, and the stars are redrawn at their new positions as 2x2 pixel blocks. This technique prevents screen flickering and maximizes rendering performance.

- **Dynamic Color and Depth:**
  Nearby stars are rendered in bright white (`LCD_COLOR_WHITE`), while distant stars are displayed in cyan (`LCD_COLOR_CYAN`) to create a sense of depth.

### Hardware and Memory Architecture

* **Development Board:** STM32F429I-DISC1
* **Display Hardware:** 2.4" QVGA (240x320) Color TFT LCD (ILI9341 Controller)
* **Graphics Controller:** LTDC (Liquid Crystal Display Controller)
* **Memory Architecture:** External SDRAM (Frame Buffer) + STMicroelectronics BSP Layer

### Files

* `main.c`: Source code containing 3D particle mathematics, SDRAM/LCD initialization sequence, and pixel-level rendering engine.
