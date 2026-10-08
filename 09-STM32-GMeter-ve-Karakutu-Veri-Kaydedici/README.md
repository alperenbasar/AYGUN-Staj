# 🏎️ G-Meter ve Karakutu Veri Kaydedici (Blackbox Data Logger)

![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Hardware](https://img.shields.io/badge/Hardware-STM32F429I--DISC1-orange.svg)
![Feature](https://img.shields.io/badge/Storage-SD%20Card%20(FatFs)-lightgrey.svg)
![Status](https://img.shields.io/badge/Status-Active-brightgreen.svg)

Bu proje, araç dinamiklerini veya nesne hareketlerini gerçek zamanlı olarak ölçmek, görselleştirmek ve kaydetmek amacıyla tasarlanmış gömülü bir telemetri (karakutu) sistemidir. STM32F429I-DISC1 üzerinde çalışan sistem, MPU6050 sensöründen aldığı ivme verilerini hem ekranda canlı bir "G-Meter (Radar)" olarak gösterir hem de MicroSD karta milisaniye hassasiyetli bir CSV dosyası olarak kaydeder.

## 🚀 Proje Özeti ve Temel Özellikler

- **Gerçek Zamanlı G-Meter (Radar) Arayüzü:** MPU6050'den alınan ham ivme (X ve Y) verileri, yazılımsal düşük geçiren filtre (Low-Pass Filter) ile pürüzsüzleştirilerek TFT ekranda akıcı bir nokta animasyonuna dönüştürülür.
- **Kesintisiz Veri Kaydı (Data Logging):** Z ekseni dâhil tüm ivme verileri `Zaman(ms), AccX, AccY, AccZ` formatında SD karta kaydedilir (`karakutu.csv`).
- **Dinamik UI Bildirimleri:** Ekranda yanıp sönen "REC" animasyonu ve modüllerin anlık durumunu gösteren (SD: OK/OFF, MPU: OK/ERR) durum çubukları bulunur.
- **Hata Toleranslı Mimari (Fail-Safe):** Çalışma esnasında SD kart bağlantısı kopsa veya modül arızalansa dahi, kilit bayrağı (lock flag) algoritması sayesinde ana döngü kilitlenmez; G-Meter ve ekran 25 FPS hızında akmaya devam eder.

## 🛠️ Kullanılan Donanım ve Bağlantı Şeması

- **Mikrodenetleyici:** STM32F429ZIT6 (STM32F429I-DISC1 Geliştirme Kartı)
- **Ekran:** Yerleşik 2.4" TFT LCD
- **Sensör:** MPU6050 İvmeölçer & Jiroskop
- **Depolama:** MicroSD Kart Modülü (SPI Haberleşmesi - FAT32)

### Pin Konfigürasyonu
*Mühendislik Notu: Bu projede yerleşik ekranın SDRAM ve LTDC (LCD-TFT Display Controller) trafiği ile SD Kartın SPI trafiği arasındaki donanımsal çakışmaları (DMA Bus meşguliyeti) önlemek için, standart pinler yerine özel pin haritalaması kullanılmıştır.*

| Modül | Pin / Bağlantı | Görev |
| :--- | :--- | :--- |
| **MPU6050** | `PA8` | I2C3 SCL |
| **MPU6050** | `PC9` | I2C3 SDA |
| **SD Kart** | `PE2` | SPI4 SCK |
| **SD Kart** | `PE5` | SPI4 MISO |
| **SD Kart** | `PE6` | SPI4 MOSI |
| **SD Kart** | `PE4` | **SPI4 CS** (Çakışmayı önleyen özel pin) |
| **SD Kart** | `5V` / `GND` | Güç Beslemesi (Kart üzerindeki LDO regülatörü için) |

## 💻 Sistem Mimarisi ve Karşılaşılan Zorluklar

Bu projenin geliştirme aşamasında aşılan en büyük problem, **LTDC sürücüsü ile SPI iletişiminin veri yolunda (bus matrix) yarattığı darboğazdır.** 
CubeMX varsayılan konfigürasyonunda SD Kartın Chip Select (CS) pini, ekran belleğine (SDRAM) tahsisli bir pin ile çakıştığı için `f_mount` işlemi sırasında sistem kilitlenmekteydi. 

**Çözüm:** 
1. CS pini tamamen izole edilmiş `PE4` pinine taşınmıştır.
2. CubeMX'in kod üreticisinin SPI pin konfigürasyonlarını (Alternate Function) atlaması durumuna karşın, `main.c` içerisine donanımı zorla (force-init) başlatan manuel `GPIO_InitStruct` blokları eklenmiştir.
3. LCD Init fonksiyonundan önce SPI hattının izoleli başlatılması sağlanarak "SD: OFF" (Mount Error) sorunu kökten çözülmüştür.

## ⚙️ Kurulum ve Çalıştırma

1. MicroSD kartı bilgisayarınızda **FAT32** formatında biçimlendirin.
2. Donanım bağlantılarını yukarıdaki şemaya göre yapın (Özellikle SD Kart VCC pininin 3.3V'a değil 5V'a bağlı olduğundan emin olun).
3. Repoyu klonlayıp projeyi **STM32CubeIDE** üzerinden açın.
4. Kodu derleyin (Build) ve işlemciye yükleyin.
5. Sistem açıldığında SD kart başarıyla okunursa yeşil **SD: OK** yazacak ve kırmızı **REC** animasyonu başlayacaktır.
6. Kaydı sonlandırmak için sistemi kapatıp SD kartı PC'ye takabilir ve `karakutu.csv` dosyasını Excel/MATLAB üzerinden grafiğe dökebilirsiniz.

## 🤝 Katkıda Bulunma

Gelecek güncellemelerde "Ring Buffer" yapısı kurularak `f_sync` optimizasyonu eklenecektir. Projeyi geliştirmeye yönelik Pull Request'lere (PR) açığım.