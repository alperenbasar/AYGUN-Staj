# Aygün Cerrahi Aletler - Staj Çalışmaları

Bu depoda, staj süresince Ar-Ge departmanında STM32 mikrodenetleyicileri ve çevre birimleri kullanılarak geliştirdiğim gömülü sistemler, haberleşme protokolleri ve yazılım projeleri yer almaktadır.

---

## Kullanılan Donanım ve Araçlar
- **Geliştirme Kiti:** STM32F429I-DISC1 (ARM Cortex-M4 @ 180 MHz)
- **Ekran & Grafik Donanımı:** 2.4" QVGA TFT LCD, LTDC, STMPE811 Dokunmatik Kontrolcü, Harici SDRAM
- **Kablosuz Haberleşme:** HC-08 Bluetooth Low Energy (BLE) Modülü
- **Geliştirme Ortamı:** STM32CubeIDE (HAL Kütüphaneleri), Tera Term Terminali, Python (Tkinter & pySerial)

---

## Proje İndeksi

1. [01-Akilli-PC-Kilit-Sistemi](./01-Akilli-PC-Kilit-Sistemi)
   - STM32F429I dokunmatik ekranı üzerinden çalışan gömülü şifre doğrulama sistemi.
   - Python tabanlı tam ekran kalkanı (overlay) ile bilgisayar erişimini engelleme/açma.
   - HC-08 BLE modülü üzerinden mobil cihaza anlık durum telemetrisi iletimi.

2. [02-Telefon-PC-Haberleşme-Köprüsü](./02-Telefon-PC-Haberleşme-Köprüsü)
   - Akıllı telefon (BLE Terminal) ile bilgisayar (Tera Term) arasında iki yönlü seri haberleşme köprüsü.
   - Asenkron veri alımı için kesme tabanlı altyapı (`HAL_UART_Receive_IT`) ve callback yönetimi.

3. [03-BLE-Tetiklemeli ve PC-Kontrollü-Hibrit-LED-Otomasyonu](./03-BLE-Tetiklemeli%20ve%20PC-Kontrollü-Hibrit-LED-Otomasyonu)
   - Telefondan gelen Bluetooth komutuyla tetiklenen durum makinesi (state machine) mimarisi.
   - PC terminaline dinamik menü basılması ve klavye girdisine göre dahili/harici LED kontrolü.
   - Seri port kilitlenmelerini önleyen `ORE` (Overrun Error) hata yönetimi.

4. [04-STM32-Donanımsal-Timer-PWM ile Pürüzsüz LED Parlaklık Kontrolü](./04-STM32-Donanımsal-Timer-PWM%20ile%20Pürüzsüz%20LED%20Parlaklık%20Kontrolü)
   - `TIM1 Channel 1` üzerinden donanımsal Darbe Genişlik Modülasyonu (PWM) sinyali üretimi.
   - 1000 adımlı yüksek çözünürlükle akıcı ve titreşimsiz "Breathing / Nefes Alma" görsel efekti.

5. [05-STM32-12-Bit-ADC-Okuması ile Gerçek Zamanlı PWM Parlaklık Kontrolü](./05-STM32-12-Bit-ADC-Okuması%20ile%20Gerçek%20Zamanlı%20PWM%20Parlaklık%20Kontrolü)
   - Potansiyometre üzerinden 12-bit çözünürlükte (0–4095) analog gerilim verisi okuma (`ADC1`).
   - Matematiksel ölçekleme ile analog değerin `TIM3` PWM doluluk oranına dönüştürülmesi ve gerçek zamanlı LED parlaklık ayarı.

6. [06-STM32F429I-DISC1-Dahili-TFT-LCD-Ekran-İlk-Sürüş-Denemesi](./06-STM32F429I-DISC1-Dahili-TFT-LCD-Ekran-İlk-Sürüş-Denemesi)
   - Dahili 2.4" TFT LCD ekranın BSP sürücü katmanı ile ayağa kaldırılması ("Hello World" testi).
   - Maksimum çekirdek frekansı (180 MHz Over-Drive) ve temel saat/güç konfigürasyonunun doğrulanması.

7. [07-STM32F429--Dahili-TFT-LCD ile 3D-Starfield-Grafik-Simülasyonu](./07-STM32F429--Dahili-TFT-LCD%20ile%203D-Starfield-Grafik-Simülasyonu)
   - 3 boyutlu uzay koordinatlarının ($X, Y, Z$) ekrana 2D perspektif izdüşümü ile yansıtılması.
   - Harici SDRAM üzerinde ekran yırtılmasını (flicker) engelleyen dinamik piksel temizleme ve çizim tekniğiyle 70 parçacıklı yıldız alanı simülasyonu.
