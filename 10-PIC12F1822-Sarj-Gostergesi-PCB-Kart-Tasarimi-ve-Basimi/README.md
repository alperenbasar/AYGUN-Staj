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

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 4000000UL

// ... (Konfigürasyon ve Makro Tanımlamaları) ...

// --- YAZILIMSAL HİSTEREZİS (ADC Eşikleri) ---
#define ADC_FULL_MAX         25     /* ~0.12 V */
#define ADC_CHARGE_MIN       60     /* ~0.28 V */
#define FILTER_TICKS         100    /* 1 sn filtre (10ms * 100) */

// --- PIN TANIMLARI ---
#define LED_R    LATAbits.LATA5
#define LED_Y    LATAbits.LATA4
#define LED_G    LATAbits.LATA2

void main(void) {
    // Dahili Osilatör ve Pin Konfigürasyonları
    OSCCON = 0x68; 
    ANSELA = 0x01;              // RA0 Analog (STAT), diğerleri dijital
    TRISA  = 0x09;              // RA0 ve RA3 giriş, diğerleri çıkış
    
    // İşlemci İçinden Sızıntıyı Önlemek İçin Pull-up Kapatma
    OPTION_REGbits.nWPUEN = 0;  
    WPUA   = 0x08;              // Sadece RA3 (ICSP/MCLR) pull-up'lı

    // ADC Kurulumu (Fosc/8, VDD Referans)
    ADCON1 = 0xA0;
    ADCON0 = 0x01;

    for (;;) {
        /* 1) Analog Sinyali Oku, Eşiklerden Geçir */
        adc = read_stat();
        if (adc >= ADC_CHARGE_MIN) {
            raw = ST_CHARGING;
        } else if (adc <= ADC_FULL_MAX) {
            raw = ST_FULL;
        } // Arada kalan voltajlarda önceki durum korunur

        /* 2) 1 Saniyelik Zaman Filtresi (Debounce) Uygula */
        // (Sinyal 1 saniye boyunca bozulmazsa State güncellenir)
        
        /* 3) State Makinesi ile Kesintisiz LED Sürümü */
        switch (state) {
        case ST_CHARGING:
            // Animasyon adımları (Bloke edici delay olmadan)
            break;
        case ST_FULL:
            // Şarj bitti, tüm LED'ler sabit
            break;
        }
        
        tick(); // 10ms gecikme
    }
}