# STM32F429I-DISC1 Dahili TFT LCD Ekran İlk Sürüş Denemesi (BSP Hello World)

Bu çalışma; STM32F429I-DISC1 geliştirme kartı üzerinde yer alan 2.4 inç QVGA TFT LCD paneline Board Support Package (BSP) sürücü katmanı kullanılarak ilk metin çıktısının ("merhaba") yazdırılması ve ekran için gerekli yüksek performanslı saat/güç konfigürasyonunun doğrulanması amacıyla gerçekleştirilmiştir.

### 📌 Çalışma Mantığı ve Öne Çıkan Detaylar
- **Maksimum Çekirdek Performansı (180 MHz & Over-Drive):** LCD TFT denetleyicisi (LTDC) ve SDRAM arayüzünün gerektirdiği yüksek bellek bant genişliğini sağlamak amacıyla harici kristal (HSE) kaynaklı PLL yapılandırması kurulmuş ve `HAL_PWREx_EnableOverDrive()` ile çekirdek tepe hızına ulaştırılmıştır.
- **BSP Ekran Katmanı Doğrulaması:** Karmaşık LTDC ve frame buffer register yapılandırmaları yerine donanım soyutlama sağlayan ST BSP kütüphanesi entegre edilmiş, `BSP_LCD_DisplayStringAt` çağrısı ile ekrana piksel/metin basma hattı başarıyla test edilmiştir.
- **Sistem Durum Göstergeleri:** Kart üzerindeki kullanıcı butonu (`PA0 - B1`) ve durum gösterge LED'leri (`PG13` Yeşil / `PG14` Kırmızı) tanımlanarak çalışma durumu donanımsal olarak izlenmiştir.

### ⚙️ Sistem ve Saat Yapılandırması
* **Mikrodenetleyici:** STM32F429ZIT6 (ARM Cortex-M4 @ 180 MHz)
* **Saat Kaynağı:** Harici Kristal (HSE) + PLL
* **Güç Modu:** Scale 1 + Over-Drive aktif
* **Gösterge ve Girişler:**
  * `PG13`: Yeşil LED (Sistem Hazır / Power ON)
  * `PG14`: Kırmızı LED
  * `PA0`: Kullanıcı Butonu (B1 Push Button)
* **Ekran Donanımı:** 2.4" QVGA (240x320) Renkli TFT LCD (İlişkili BSP sürücüsü üzerinden)

### 📂 Dosyalar
* `main.c`: Yüksek hızlı sistem saat kurulumu, GPIO başlatmaları ve BSP LCD metin yazdırma çağrısını içeren ana C kodu.
