# Aygün Cerrahi Aletler - Staj Çalışmaları

Bu depoda, staj süresince Ar-Ge departmanında STM32 mikrodenetleyicileri ve çevre birimleri kullanılarak geliştirdiğim gömülü sistemler, haberleşme protokolleri ve yazılım projeleri yer almaktadır.

---

## Kullanılan Donanım ve Araçlar
- **Geliştirme Kiti:** STM32F429I-DISC1 (ARM Cortex-M4 @ 180 MHz)
- **Ekran & Grafik Donanımı:** 2.4" QVGA TFT LCD, LTDC, STMPE811 Dokunmatik Kontrolcü, Harici SDRAM
- **Sensörler ve Modüller:** MPU6050 (İvmeölçer ve Jiroskop), MicroSD Kart Modülü (SPI)
- **Kablosuz Haberleşme:** HC-08 Bluetooth Low Energy (BLE) Modülü
- **Geliştirme Ortamı & Yazılım:** STM32CubeIDE (HAL Kütüphaneleri, FatFs), Tera Term, Python (Tkinter & pySerial), Notify API

---

## Proje İndeksi

PROJE İNDEKSİ

01-PIC12F1822-Sarj-Gostergesi-PCB-Kart-Tasarimi-ve-Basimi
##• CN3303 lityum şarj entegresi ile tam uyumlu çalışan, Microchip PIC12F1822 (SOIC-8) kullanılarak tasarlanmış akıllı şarj durum göstergesi ve 3 kademeli (Kırmızı-Sarı-Yeşil) kümülatif dolum animasyonu kontrol kartı.
##• KiCad ile şematik tasarımı ve LPKF S63 kazıma tezgahına uygun tek katmanlı PCB yerleşimi. Şarj modülünün open-drain çıkışında oluşan kaçak akımları ve LED çakışmalarını önlemek amacıyla yüksek empedanslı gerilim bölücü (22K/100K) kullanılarak donanımsal izolasyon sağlandı.
##• STAT pininden gelen kararsız voltaj seviyeleri dijital GPIO yerine ADC ile okundu. Şarj entegresinin mod geçişlerinde yarattığı sinyal gürültülerini ve LED dalgalanmalarını sıfıra indirmek için yazılımsal gerilim eşiği ve 1 saniyelik zaman tabanlı stabilizasyon (debounce) algoritmaları geliştirildi.

02-Akilli-PC-Kilit-Sistemi
• STM32F429I dokunmatik ekranı üzerinden çalışan gömülü şifre doğrulama sistemi.
• Python tabanlı tam ekran kalkanı (overlay) ile bilgisayar erişimini engelleme/açma.
• HC-08 BLE modülü üzerinden mobil cihaza anlık durum telemetrisi iletimi.

03-STM32-MPU6050-ile-Akilli-Guvenlik-Sistemi
• MPU6050 ivmeölçer/jiroskop sensörü ile hassas anomali (hareket/sarsıntı) tespiti yapan çok katmanlı alarm sistemi.
• İhlal durumunda PC'ye seri port üzerinden uyarı iletimi ve Notify API ile akıllı telefona mobil "Push" bildirim entegrasyonu.
• Dahili dokunmatik TFT ekranın Numpad arayüzü olarak kullanılarak alarmın donanımsal şifre ile iptal edilmesi (Disarm) mekanizması.

04-STM32-GMeter-ve-Karakutu-Veri-Kaydedici
• MPU6050 ivme verilerinin Low-Pass Filter (Düşük Geçiren Filtre) ile işlenerek ekranda 25 FPS hızında gerçek zamanlı "G-Meter" (Radar) arayüzüne dönüştürülmesi.
• Araç dinamiği verilerinin (Zaman, AccX, AccY, AccZ) MicroSD karta (FAT32) donanımsal SPI hattı üzerinden kesintisiz bir .csv dosyası olarak kaydedilmesi (Data Logging).
• Ekranın LTDC/SDRAM DMA veri trafiği ile SD kartın SPI okuma/yazma döngüleri arasındaki veri yolu çakışmalarının (Bus matrix congestion), donanımsal pin izolasyonu (CS pini tahsisi) ile kilitlenmeden (non-blocking) çözümlenmesi.

05-BLE-Tetiklemeli ve PC-Kontrollu-Hibrit-LED-Otomasyon-Sistemi
• Telefondan gelen Bluetooth komutuyla tetiklenen durum makinesi (state machine) mimarisi.
• PC terminaline dinamik menü basılması ve klavye girdisine göre dahili/harici LED kontrolü.
• Seri port kilitlenmelerini önleyen ORE (Overrun Error) hata yönetimi.

06-STM32F429--Dahili-TFT-LCD ile 3D-Starfield-Grafik-Simülasyonu
• 3 boyutlu uzay koordinatlarının (X, Y, Z) ekrana 2D perspektif izdüşümü ile yansıtılması.
• Harici SDRAM üzerinde ekran yırtılmasını (flicker) engelleyen dinamik piksel temizleme ve çizim tekniğiyle 70 parçacıklı yıldız alanı simülasyonu.

07-STM32F429I-DISC1-Dahili-TFT-LCD-Ekran-İlk-Surus-Denemesi (BSP Hello World)
• Dahili 2.4" TFT LCD ekranın BSP sürücü katmanı ile ayağa kaldırılması ("Hello World" testi).
• Maksimum çekirdek frekansı (180 MHz Over-Drive) ve temel saat/güç konfigürasyonunun doğrulanması.

08-STM32-Donanimsal-Timer-PWM ile Puruzsuz-LED-Parlaklik-Kontrolu
• TIM1 Channel 1 üzerinden donanımsal Darbe Genişlik Modülasyonu (PWM) sinyali üretimi.
• 1000 adımlı yüksek çözünürlükle akıcı ve titreşimsiz "Breathing / Nefes Alma" görsel efekti.

09-Telefon-PC-Haberlesme-Koprusu
• Akıllı telefon (BLE Terminal) ile bilgisayar (Tera Term) arasında iki yönlü seri haberleşme köprüsü.
• Asenkron veri alımı için kesme tabanlı altyapı (HAL_UART_Receive_IT) ve callback yönetimi.

10-STM32-12-Bit-ADC-Okumasi ile Gercek-Zamanli-PWM-Parlaklik-Kontrolu
• Potansiyometre üzerinden 12-bit çözünürlükte (0–4095) analog gerilim verisi okuma (ADC1).
• Matematiksel ölçekleme ile analog değerin TIM3 PWM doluluk oranına dönüştürülmesi ve gerçek zamanlı LED parlaklık ayarı.
