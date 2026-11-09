# MPPT Analizli Çift Eksenli Güneş Takip Sistemi

Bu proje, LDR sensörler ve FT5325M servo motorlar kullanılarak güneş panellerinin optimum yönelimini sağlayan Arduino tabanlı elektromekanik bir sistemdir[cite: 22, 23]. Sistem, INA226 güç sensörleri ile anlık verileri işleyerek MPPT (Maksimum Güç Noktası Takibi) analizi gerçekleştirmektedir[cite: 23, 26, 55].

Bu çalışma, Güneş'in gökyüzündeki yatay ve dikey hareketlerini otonom olarak takip ederek "kosinüs kayıplarını" önlemeyi ve laboratuvar ortamında elektriksel yük optimizasyonunu gerçekleştirmeyi hedefler[cite: 25, 26]. 

## Proje Özeti ve Hedefleri
- **Otonom Yönelim:** Güneşin konumunu 4 adet LDR sensörle algılayarak çift eksenli mekanik takip (yatayda 0-180 derece, dikeyde 0-90 derece)[cite: 22, 63].
- **Güç Analizi:** INA226 hassas güç analizörü ve 73 $\Omega$ alüminyum yük direnci ile anlık voltaj, akım ve güç ölçümü[cite: 28, 48, 63].
- **Verim Optimizasyonu:** Sabit açılı sistemlere kıyasla elde edilen güç artışının MPPT mantığıyla analizi ve laboratuvar ortamında 1,8296 W Maksimum Güç Noktasının (MPP) tespiti[cite: 48].

## Kullanılan Donanım ve Bileşenler
- **Mikrodenetleyici:** Arduino Uno R3 (ATmega328, AVR Mimarisi)[cite: 37]
- **Sensörler:** LDR Işık Sensörleri (4 adet), INA226 Dijital Akım ve Güç Monitörü[cite: 22, 35, 55]
- **Aktüatörler:** 2x Feetech FT5320M Yüksek Torklu Dijital Servo Motor (Çift Eksen)[cite: 38, 55]
- **Güneş Paneli:** SolarMarket 25W 12V Monokristal Solar Panel[cite: 34]
- **Güç Yönetimi:** 3 adet seri bağlı Great Power INR18650 Şarj Edilebilir Lityum İyon Pil, XL4016 DC-DC Step-Down (Voltaj Düşürücü) Regülatör[cite: 42, 43, 55]
- **Arayüz ve Pasif Elemanlar:** Arduino Uno Sensor Shield V5.0, 50W Alüminyum Gövdeli Güç Direnci[cite: 36, 39]

## Depo İçeriği
- `yazilim/`: Arduino (.ino) gömülü yazılım kaynak kodlarını barındırır.
- `tez/`: Projenin mekanik tasarımını, teorik hesaplamalarını (MPPT, P-V ve I-V eğrileri) ve donanım entegrasyon süreçlerini detaylandıran Lisans Bitirme Tezi (.pdf) dosyasını içerir.
- `fotograflar/`: SolidWorks 3D modelleme görüntülerini, üretim/montaj aşamalarını, devre şemasını ve laboratuvar test sonuç grafiklerini belgeleyen fotoğrafları barındırır.
- `donanim/`: Projenin genel mimarisini ve Fritzing bağlantı şemasını içerir.

## Yazılım (Firmware) Genel Bakış

Yazılım, çevresel ışık koşullarını 4 LDR sensörü üzerinden Analog-Dijital Dönüştürücü (ADC) ile okur. Sensörler motor eksenlerine göre bağımsız ikili gruplara ayrılmıştır (1. Motor için A0-A1, 2. Motor için A3-A2) ve yönelim doğrudan bu sensör çiftleri arasındaki ışık şiddeti farkına göre hesaplanır. Motor titreşimlerini (osilasyon) ve mekanik aşınmayı önlemek için, kontrol algoritmasında bir 'Ölü Bant' (histerezis) eşik değeri (`esikDeger = 40`) kullanılarak Diferansiyel Kontrol Algoritması uygulanır.

Panelin mekanik güvenliği `if-else` koşullu ifadeleriyle kontrol altına alınmış olup, hareket alanı donanımsal limitler olan 1. eksen (0-30°) ve 2. eksen (0-180°) sınırlarına hapsedilmiştir. Hareket komutları 1 derecelik hassas adımlarla ve sistemin mekanik sarsıntılarını tamamen sönümlemek amacıyla yavaşlatılmış stabil bir tepki süresiyle (`1000 ms` gecikme) işlenir. INA226 sensöründen gelen güç ve voltaj ölçümleri ise I2C protokolü üzerinden Arduino'ya aktarılarak MPPT analizi tamamlanır.
