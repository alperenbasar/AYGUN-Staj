10-PIC12F1822-Sarj-Gostergesi-PCB-Kart-Tasarimi-ve-Basimi

Bu repo, Microchip PIC12F1822 8-bit mikrodenetleyici tabanlı, özel bir şarj durum göstergesi kontrolcüsü için uçtan uca tasarım, prototipleme, PCB yerleşimi ve gömülü yazılım süreçlerini içermektedir.

Bu proje, yeni geliştirilen bir donanım ürünü üzerinde görsel bir şarj animasyonu ve durum göstergesi sağlanması yönündeki mühendislik talebini karşılamak amacıyla hayata geçirilmiştir.

---

Geliştirme Aşamaları

1. Gereksinimler ve Şematik Tasarım: Giriş sinyalleri (`STAT` şarj sinyali) ve çıkış gereksinimleri belirlendi, ardından KiCad üzerinden şematik çizimi tamamlandı.
2. Prototipleme: Pin lojik yapısı ve zamanlama döngüleri bir breadboard kurulumu üzerinde test edilip doğrulandı.
3. PCB Yerleşimi ve Üretim: Şirket içi prototipleme süreçlerine (LPKF kazıma/frezeleme yöntemi) uygun, kompakt ve tek yüzlü bir PCB tasarımı yapıldı; laboratuvar tipi delikli/manuel hibrit lehimleme ile donanım ayağa kaldırıldı.
4. Gömülü Yazılım: MPLAB X IDE ve XC8 derleyicisi kullanılarak C dilinde yazıldı; dahili osilatör konfigürasyonları ve register seviyesinde kontrol mimarisi kullanıldı.

---

Donanım Özellikleri

Mikrodenetleyici: Microchip PIC12F1822 (Prototipleme için DIP-8 dönüştürmeli SOIC-8 kılıf)
Osilatör: Dahili 4 MHz HFINTOSC (Harici kristal osilatöre ihtiyaç duyulmaz)
Giriş Arayüzü: `RA2` pinine bağlanan ve $1\text{K}$ akım sınırlama direnci (`R4`) ile şartlandırılan `STAT_GIRIS` sinyal hattı.
Çıkış Arayüzü: `RA0` ve diğer port pinleri üzerinden $330\ \Omega$ dirençlerle sürülen durum LED'leri.
Programlama Arayüzü: PICkit 3 ile doğrudan kart üzerinden hata ayıklama (debug) ve kod yükleme imkanı sunan yerleşik ICSP (In-Circuit Serial Programming) başlığı.

---

Repo Klasör Yapısı

`Firmware/`: MPLAB X C kaynak kodlarını (`main.c`) ve üretim hex dosyalarını barındırır.
`Hardware/`: KiCad şematik (`.kicad_sch`) ve PCB yerleşim (`.kicad_pcb`) dosyalarını içerir.
`Images/`: Breadboard testlerini, kazınan PCB'yi ve final montajı belgeleyen fotoğrafları barındırır.

---

Yazılım (Firmware) Genel Bakış

Yazılım; dahili osilatörü yapılandırır, dahili pull-up dirençlerini aktif eder, STAT_PIN üzerinden gelen şarj durumunu yazılımsal filtreleme (debounce) ile okur ve şarj durumuna göre LED animasyonlarını veya sabit doluluk ışıklarını kontrol eder.

```c
#include <xc.h>

#define _XTAL_FREQ 4000000    

// --- PIN TANIMLAMALARI ---
#define LED1 LATA5   
#define LED2 LATA4   
#define LED3 LATA2   
#define STAT_PIN RA3 

void main(void) {
    OSCCON = 0b01101010; 
    TRISA = 0b00001000;  // RA3 giriş (STAT), diğerleri çıkış
    ANSELA = 0x00;       // Dijital Pinler

    // Dahili Pull-Up Aktif
    OPTION_REGbits.nWPUEN = 0; 
    WPUA = 0b00001000;         

    while(1) {
        // Yazılımsal Filtre ve Durum Kontrolü
        int okuma1 = STAT_PIN;
        __delay_ms(50); 
        int okuma2 = STAT_PIN;
        
        if(okuma1 == okuma2) {
            if(okuma1 == 1) {  
                // --- ŞARJ OLUYOR (Animasyon) ---
                LED1 = 1; LED2 = 0; LED3 = 0;
                __delay_ms(300);
                // (Kademeli LED döngüleri...)
            } else {
                // --- ŞARJ BİTTİ (Sabit Full) ---
                LED1 = 1; LED2 = 1; LED3 = 1;
            }
        }
    }
}