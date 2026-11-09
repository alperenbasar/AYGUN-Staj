# STM32F429I-DISC1 Onboard TFT LCD First Display Test (BSP Hello World)

This project was carried out to display the first text output ("hello") on the 2.4-inch QVGA TFT LCD panel integrated into the STM32F429I-DISC1 development board using the Board Support Package (BSP) driver layer, and to verify the high-performance clock and power configuration required for display operation.

### Operating Principle and Key Highlights

- **Maximum Core Performance (180 MHz & Over-Drive):** To provide the high memory bandwidth required by the LCD TFT controller (LTDC) and SDRAM interface, an HSE-based PLL clock configuration was implemented using an external crystal oscillator. The maximum core frequency was achieved by enabling `HAL_PWREx_EnableOverDrive()`.

- **BSP Display Layer Verification:** Instead of configuring complex LTDC and frame buffer registers manually, the ST BSP library was integrated to provide hardware abstraction. The display output pipeline was successfully tested using the `BSP_LCD_DisplayStringAt` function to render text on the LCD.

- **System Status Indicators:** The onboard user button (`PA0 - B1`) and status indicator LEDs (`PG13` Green / `PG14` Red) were configured to enable hardware-level monitoring of the system's operating status.

### System and Clock Configuration

* **Microcontroller:** STM32F429ZIT6 (ARM Cortex-M4 @ 180 MHz)
* **Clock Source:** External Crystal (HSE) + PLL
* **Power Mode:** Scale 1 + Over-Drive Enabled
* **Indicators and Inputs:**
  * `PG13`: Green LED (System Ready / Power ON)
  * `PG14`: Red LED
  * `PA0`: User Button (B1 Push Button)
* **Display Hardware:** 2.4" QVGA (240x320) Color TFT LCD (via the corresponding BSP driver)

### Files

* `main.c`: Main C source code containing high-speed system clock configuration, GPIO initialization, and BSP LCD text rendering function calls.
