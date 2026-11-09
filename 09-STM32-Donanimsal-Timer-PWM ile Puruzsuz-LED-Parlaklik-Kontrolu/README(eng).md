# STM32 Hardware Timer PWM-Based Smooth LED Brightness Control (Breathing LED)
This project was developed to generate a hardware-based Pulse Width Modulation (PWM) signal using the advanced-control timer (TIM1) of the STM32F4 microcontroller and create a smooth "Breathing" visual effect with this signal.

### Operating Principle and Features
- **Hardware PWM Generation:** The PWM signal is generated directly through the Timer peripheral (`TIM1 Channel 1`) without placing any additional load on the processor core.
- **High-Resolution Duty Cycle Control:** The timer period (`ARR`) is set to 1000, dividing the duty cycle range from 0% to 100% into 1000 equal steps.
- **Smooth Brightness Transitions (Flicker-Free):** Brightness increase and decrease steps are performed with a precision of 1 unit at 2 ms intervals, achieving a completely smooth, flicker-free brightness curve for the human eye.

### Timer Configuration
* **Timer Used:** TIM1 (Advanced Control Timer)
* **Channel:** Channel 1 (PWM Mode 1)
* **Prescaler (PSC):** 83
* **Period (ARR):** 1000
* **Output Pin:** PA8 (TIM1_CH1)
* **Cycle Duration:** ~2 seconds fade-in, ~2 seconds fade-out (4 seconds total period)

### Files
* `main.c`: Main firmware responsible for TIM1 PWM configuration, MSP pin assignments, and updating the register-level compare value (`__HAL_TIM_SET_COMPARE`).
