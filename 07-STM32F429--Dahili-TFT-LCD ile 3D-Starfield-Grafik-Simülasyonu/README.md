# STM32F429 Dahili TFT LCD ile 3D Starfield (Yıldız Alanı) Grafik Simülasyonu

Bu proje; **STM32F429I-DISC1** geliştirme kitinin dahili 2.4 inç QVGA TFT LCD ekranı ve harici SDRAM mimarisi üzerinde, 3 boyutlu uzay projeksiyonu ve gerçek zamanlı dinamik piksel manipülasyonu gerçekleştirmek amacıyla geliştirilmiştir.

### Çalışma Mantığı ve Matematiksel Model
- **3D'den 2D'ye Perspektif İzdüşümü:** 
  Üç boyutlu uzayda $(X, Y, Z)$ koordinatlarına sahip 70 adet parçacık tanımlanmıştır. Her karede $Z$ derinliği azaltılarak yıldızların izleyiciye yaklaşması simüle edilir. 2D ekran koordinatları perspektif projeksiyon formülüyle hesaplanır:
  $$S_x = \frac{X \cdot d}{Z} + C_x, \quad S_y = \frac{Y \cdot d}{Z} + C_y$$
- **Titreşimsiz (Flicker-Free) Çizim:**
  Tüm ekranı temizlemek yerine yalnızca hareket eden yıldızların bir önceki karedeki pozisyonları (`prev_sx`, `prev_sy`) siyah renkle silinir ve yeni pozisyona 2x2 piksel bloklar halinde basılır. Bu yöntem ekran titreşimini engeller ve render hızını maksimize eder.
- **Dinamik Renk ve Derinlik:**
  Yakındaki yıldızlar parlak beyaz (`LCD_COLOR_WHITE`), uzaktaki yıldızlar ise derinlik hissi vermek amacıyla camgöbeği (`LCD_COLOR_CYAN`) olarak renklendirilir.

### Donanım ve Bellek Mimarisi
* **Geliştirme Kiti:** STM32F429I-DISC1
* **Ekran Donanımı:** 2.4" QVGA (240x320) Renkli TFT LCD (ILI9341 sürücüsü)
* **Grafik Kontrolcü:** LTDC (Liquid Crystal Display Controller)
* **Bellek Katmanı:** Harici SDRAM (Frame Buffer) + STMicroelectronics BSP Katmanı

### Dosyalar
* `main.c`: 3D parçacık matematiği, SDRAM/LCD başlatma sekansı ve piksel düzeyinde render motorunu içeren kaynak kod.
