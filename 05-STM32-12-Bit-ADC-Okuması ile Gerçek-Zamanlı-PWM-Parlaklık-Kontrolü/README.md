# STM32 12-Bit ADC Okuması ile Gerçek Zamanlı PWM Parlaklık Kontrolü

Bu çalışma; STM32F429 mikrodenetleyicisinin dahili Analog-Dijital Dönüştürücüsü (ADC1) ile harici bir potansiyometreden gerilim verisi okunması ve bu verinin matematiksel olarak ölçeklenerek donanımsal Timer PWM (TIM3) sinyaliyle bir LED'in parlaklığının gerçek zamanlı kontrol edilmesi amacıyla geliştirilmiştir.

### 📌 Çalışma Mantığı ve Veri Akışı
1. **Analog Dönüşüm (Polling Modu):**
   - `PA0` pinine uygulanan analog gerilim, `hadc1` üzerinden 12-bit çözünürlükle (0–4095 aralığı) taranır.
   - `HAL_ADC_PollForConversion` fonksiyonuyla dönüşümün tamamlandığı teyit edilir ve anlık değer `HAL_ADC_GetValue` ile okunur.
2. **Dinamik Ölçekleme (Mapping):**
   - 12-bit ADC aralığı [0 - 4095], Timer periyot çözünürlüğü olan [0 - 999] aralığına normalize edilir:
     $$\text{PWM} = \frac{\text{ADC} \times 999}{4095}$$
3. **Donanımsal Sinyal Modülasyonu:**
   - Elde edilen değer `__HAL_TIM_SET_COMPARE` makrosu aracılığıyla doğrudan `TIM3_CH1` donanım kütüğüne (CCR1) yazılır ve LED parlaklığı gecikmesiz şekilde güncellenir.
