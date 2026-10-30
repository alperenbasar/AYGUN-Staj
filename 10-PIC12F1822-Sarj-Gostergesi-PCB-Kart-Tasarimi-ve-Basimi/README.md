# 10-PIC12F1822-Sarj-Gostergesi-PCB-Kart-Tasarimi-ve-Basimi

Bu repo, Microchip PIC12F1822 8-bit mikrodenetleyici tabanlı, CN3303 lityum şarj entegresi ile tam uyumlu çalışan özel bir şarj durum göstergesi kontrolcüsü için uçtan uca tasarım, prototipleme, donanımsal sinyal izolasyonu, PCB yerleşimi ve gömülü yazılım süreçlerini içermektedir.

Bu proje, yeni geliştirilen bir donanım ürünü üzerinde görsel bir kümülatif şarj animasyonu (Kırmızı-Sarı-Yeşil) ve dolum durum göstergesi sağlanması yönündeki mühendislik talebini karşılamak amacıyla hayata geçirilmiştir.

---

## Geliştirme Aşamaları ve Donanımsal Çözümler

1. Gereksinimler ve Şematik Tasarım: Giriş sinyalleri (`STAT` şarj sinyali) ve çıkış gereksinimleri belirlendi. KiCad üzerinden şematik çizimi tamamlandı.
2. Sinyal İzolasyonu ve Prototipleme: İlk prototiplemede şarj modülünün (CN3303) open-drain çıkışında mikrodenetleyicinin dijital giriş ve pull-up dirençlerinden kaynaklı kaçak akımlar ve LED çakışmaları tespit edildi. Bu donanımsal kilitlenmeyi çözmek amacıyla, giriş hattında yüksek empedanslı bir gerilim bölücü (22K seri, 100K pull-down) kullanılarak tam elektriksel izolasyon sağlandı.
3. PCB Yerleşimi ve Üretim: Şirket içi prototipleme süreçlerine (LPKF kazıma/frezeleme yöntemi) uygun, kompakt ve tek yüzlü bir PCB tasarımı yapıldı. Testler sonrası, laboratuvar tipi manuel lehimleme ile donanım ayağa kaldırıldı.
4. Gömülü Yazılım ve ADC Filtreleme: Sinyal gürültülerini (CN3303'ün pil arama modundaki voltaj dalgalanmalarını) sıfıra indirmek için, `STAT` pini dijital GPIO yerine ADC (Analog-Dijital Çevirici) ile okundu. MPLAB X IDE ve XC8 derleyicisi kullanılarak; yazılımsal histerezis (gerilim eşiği), 1 saniyelik zaman tabanlı stabilizasyon (debounce) algoritmaları ve State Machine mimarisi C dilinde kodlandı.

---

## Donanım Özellikleri

*   Mikrodenetleyici: Microchip PIC12F1822 (SOIC-8)
*   Osilatör: Dahili 4 MHz HFINTOSC (Harici kristal osilatöre ihtiyaç duyulmaz)
*   Giriş Arayüzü (Analog): CN3303'ün `STAT` hattından gelen sinyal, kaçak akımları önlemek için 22K (R4) seri direnç ve 100K (R5) pull-down direnci üzerinden RA0 pinine bağlanmıştır.
*   Çıkış Arayüzü: `RA5`, `RA4` ve `RA2` pinleri üzerinden 470 Ω dirençlerle sürülen 3 kademeli (Kırmızı, Sarı, Yeşil) durum LED'leri.
*   Programlama Arayüzü: PICkit 3 ile doğrudan kart üzerinden hata ayıklama (debug) ve kod yükleme imkanı sunan yerleşik ICSP (In-Circuit Serial Programming) başlığı. *(Not: RA3/MCLR pini donanımsal izolasyon sebebiyle yazılım okumasından çıkarılmıştır).*

---

## Repo Klasör Yapısı

*   `Firmware/`: MPLAB X C kaynak kodlarını (`main.c`) ve üretim hex dosyalarını barındırır.
*   `Hardware/`: KiCad şematik (`.kicad_sch`) ve PCB yerleşim (`.kicad_pcb`) dosyalarını içerir.
*   `Images/`: Breadboard testlerini, kazınan PCB'yi, donanımsal modifikasyonları ve final montajı belgeleyen fotoğrafları barındırır.

---

## Yazılım (Firmware) Genel Bakış

Yazılım; donanımsal gürültüleri ve kararsız mod geçişlerini (örn. pil bağlı değilken oluşan osilasyonları) engellemek için sinyali Analog (ADC) olarak okur. Belirlenen histerezis eşikleri (`ADC_CHARGE_MIN` ve `ADC_FULL_MAX`) ve 1 saniyelik zaman bazlı filtreleme ile temizlenmiş sinyal üzerinden şarj kümülatif animasyonunu yönetir. Durum makinesi mantığı sayesinde işlemci animasyon sırasında kör (kilitli) kalmaz.

10-PIC12F1822-Sarj-Gostergesi-PCB-Kart-Tasarimi-ve-Basimi/Software/main.c
