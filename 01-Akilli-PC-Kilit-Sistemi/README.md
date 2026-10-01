STM32 Tabanlı Güvenli PC Kilit ve BLE Bildirim Sistemi
Bu proje, STM32F429I-DISC1 geliştirme kiti, HC-08 BLE modülü ve Python tabanlı bir ana bilgisayar yazılımı kullanılarak geliştirilmiş iki aşamalı bir fiziksel güvenlik ve kimlik doğrulama sistemidir. Şifre doğrulama yükü işletim sisteminden alınarak doğrudan mikrodenetleyici donanımına aktarılmıştır.

Temel Özellikler
Donanımsal Kimlik Doğrulama: Şifre doğrulaması tamamen STM32 üzerinde çalışan gömülü yazılımla gerçekleştirilir.

Dokunmatik Arayüz: Kart üzerindeki 2.4" TFT LCD ekran ve STMPE811 dokunmatik kontrolcüsü ile sayısal tuş takımı (Numpad) sunulur.

Tam Ekran Kalkanı (Python): Tkinter ile geliştirilen kalkan, karttan gelen LOCK komutuyla ekranı kaplar ve fare/klavye erişimini engeller. Doğru şifre girilip UNLOCK komutu geldiğinde ekran açılır.

Kablosuz Güvenlik Bildirimi (HC-08 BLE): Kilit durumu ve şifre denemeleri UART5 hattına bağlı HC-08 modülü üzerinden telefona anlık bildirim olarak gönderilir.

Donanım ve Bağlantılar
STM32F429I-DISC1: Ana kontrolcü ve dokunmatik arayüz (FreeRTOS altyapısı).

USART1 (115200 Baud): ST-LINK Sanal COM Port üzerinden bilgisayara kilit komutlarını iletir.

UART5 (9600 Baud): HC-08 Bluetooth modülü üzerinden telefona güvenlik telemetrisi iletir.

Kullanım
STM32 kodunu karta yükleyin ve kartı bilgisayara bağlayın.

Bilgisayarda pyserial kütüphanesini kurun (pip install pyserial).

akilli_kilit.py dosyasındaki COM portunu ayarlayıp çalıştırın (python akilli_kilit.py). (Acil çıkış: ESC)
