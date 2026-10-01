# STM32 Donanımsal Timer PWM ile Pürüzsüz LED Parlaklık Kontrolü (Breathing LED)
Bu proje; STM32F4 mikrodenetleyicisinin gelişmiş kontrol zamanlayıcısı (TIM1) kullanılarak donanımsal Darbe Genişlik Modülasyonu (PWM) sinyali üretilmesi ve bu sinyalle pürüzsüz bir "Breathing / Nefes Alma" görsel efektinin oluşturulması amacıyla geliştirilmiştir.

### Çalışma Mantığı ve Özellikler
- **Donanımsal PWM Üretimi:** İşlemci çekirdeğini ek yük altında bırakmadan doğrudan Timer çevre birimi (`TIM1 Channel 1`) üzerinden PWM sinyali üretilir.
- **Yüksek Çözünürlüklü Duty Cycle Kontrolü:** Timer periyodu (`ARR`) 1000 olarak ayarlanarak %0 ile %100 doluluk oranı 1000 eşit adıma bölünmüştür.
- **Akıcı Parlaklık Geçişi (Flicker-Free):** Parlaklık artırma ve azaltma adımları 1 birimlik hassasiyetle ve 2 ms'lik aralıklarla yürütülerek insan gözü için tamamen pürüzsüz, titreşimsiz bir parlaklık eğrisi elde edilmiştir.

### Zamanlayıcı (Timer) Yapılandırması
* **Kullanılan Zamanlayıcı:** TIM1 (Advanced Control Timer)
* **Kanal:** Channel 1 (PWM Mode 1)
* **Prescaler (PSC):** 83
* **Period (ARR):** 1000
* **Çıkış Pini:** PA8 (TIM1_CH1)
* **Döngü Süresi:** ~2 saniye yükselme, ~2 saniye sönme (4 saniye toplam periyot)

### Dosyalar
* `main.c`: TIM1 PWM yapılandırması, MSP pin atamaları ve register seviyesinde karşılaştırma (`__HAL_TIM_SET_COMPARE`) değerini güncelleyen ana yazılım.
