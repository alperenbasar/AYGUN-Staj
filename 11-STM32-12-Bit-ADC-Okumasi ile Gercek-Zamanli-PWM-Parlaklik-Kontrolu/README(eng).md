# STM32 Real-Time PWM Brightness Control via 12-Bit ADC Reading

This project is developed to read voltage data from an external potentiometer using the internal Analog-to-Digital Converter (ADC1) of the STM32F429 microcontroller, mathematically scale this data, and control the brightness of an LED in real-time using a hardware Timer PWM (TIM3) signal.

---

## Architecture and Data Flow

1. **Analog Conversion (Polling Mode):**
   - The analog voltage applied to the `PA0` pin is scanned with a 12-bit resolution (0–4095 range) via `hadc1`.
   - Completion of the conversion is verified using the `HAL_ADC_PollForConversion` function, and the instantaneous value is read via `HAL_ADC_GetValue`.

2. **Dynamic Scaling (Mapping):**
   - The 12-bit ADC range [0 - 4095] is normalized to the timer period resolution range [0 - 999]:
     $$\text{PWM} = \frac{\text{ADC} \times 999}{4095}$$

3. **Hardware Signal Modulation:**
   - The resulting value is directly written to the `TIM3_CH1` hardware register (CCR1) via the `__HAL_TIM_SET_COMPARE` macro, ensuring zero-delay updates to the LED brightness.
