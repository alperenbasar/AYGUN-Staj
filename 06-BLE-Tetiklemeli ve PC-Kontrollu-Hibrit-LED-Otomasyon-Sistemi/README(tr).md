# BLE Tetiklemeli ve PC Kontrollü Hibrit LED Otomasyon Sistemi
Bu çalışma; mobil cihaz (akıllı telefon), HC-08 BLE modülü, STM32F429 mikrodenetleyicisi ve bilgisayar terminali (Tera Term) arasında durum makinesi (state machine) mantığıyla çalışan iki yönlü bir interaktif kontrol mimarisidir.

### Çalışma Mantığı ve Kontrol Akışı
1. **Mobil Tetikleme (HC-08 / USART6):**
   - Telefondan gönderilen karakterler `HAL_UART_Receive_IT` kesmesi ile bayt bayt diziye aktarılır.
   - Satır sonu karakteri (`\r` veya `\n`) geldiğinde metin doğrulaması yapılır. Gelen komut `lamba` ise sistem bir sonraki kontrol durumuna geçer.
2. **Terminal Yönlendirmesi (VCP / USART1):**
   - Komut algılandığında PC tarafındaki seri terminale (Tera Term) seçim menüsü gönderilir ve sistem kullanıcı girişini beklemeye alır (`pc_bekleniyor = 1`).
3. **Donanım Çıkış Kontrolü:**
   - PC klavyesinden `1` tuşuna basıldığında harici LED pini aktif edilir.
   - `2` tuşuna basıldığında kart üzerindeki dahili LED pini aktif edilir.
4. **Hata ve Taşma Yönetimi (Overrun Handling):**
   - Olası tampon taşmalarına ve hat hatalarına karşı `HAL_UART_ErrorCallback` fonksiyonu ile `ORE` (Overrun Error) bayrakları otomatik temizlenerek asenkron dinleme sürekliliği sağlanır.
