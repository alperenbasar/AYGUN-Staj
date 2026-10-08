\# 🛡️ MPU6050 Destekli Akıllı Güvenlik Sistemi (Smart Security System)



Bu proje, STM32F429I-DISC1 geliştirme kartı ve MPU6050 ivmeölçer/jiroskop sensörü kullanılarak geliştirilmiş, çok katmanlı bir akıllı güvenlik ve alarm sistemidir. Herhangi bir fiziksel hareket (hırsızlık, sarsıntı vb.) algılandığında sistem hem yerel hem de uzak uyarı mekanizmalarını tetikler. Alarm durumunun devre dışı bırakılması, yalnızca STM32'nin dokunmatik ekranı (LCD) üzerinden doğru şifrenin girilmesiyle mümkündür.



Proje Özeti ve Temel Özellikler



\- Hassas Hareket Algılama: MPU6050 sensörü ile fiziksel manipülasyonların milisaniyeler içinde tespiti.

\- Çok Katmanlı Uyarı Sistemi:

&nbsp; - Görsel Alarm: Tetiklenme anında harici LED'in yüksek frekansta yanıp sönmesi (strobe efekti).

&nbsp; - Lokal Uyarı: PC terminali/Seri Monitör üzerine anlık olarak \*"Hareket Algılandı!"\* uyarısının düşmesi.

&nbsp; - Mobil Push Bildirimi: Notify API entegrasyonu sayesinde saniyeler içinde akıllı telefona acil durum bildiriminin gönderilmesi.

\- \*\*Dokunmatik Şifre Paneli (Numpad): STM32 üzerindeki TFT LCD ekranın bir numpad arayüzü olarak kullanılması. Alarm sadece doğru PIN kodunun girilmesiyle devre dışı bırakılabilir.



Kullanılan Donanım ve Bileşenler



\- Mikrodenetleyici: STM32F429ZIT6 (STM32F429I-DISC1 Geliştirme Kartı)

\- Ekran: Yerleşik 2.4" TFT LCD Dokunmatik Ekran (Numpad arayüzü için)

\- Sensör: MPU6050 (6 Eksen İvmeölçer ve Jiroskop)

\- Çıktı Birimi: Harici LED Modülü

\- Haberleşme: 

&nbsp; - I2C (MPU6050 ile STM32 arası iletişim)

&nbsp; - UART (PC'ye uyarı mesajlarının gönderilmesi ve mobil bildirim modülü haberleşmesi)



Sistem Mimarisi ve Akış Algoritması



1\. \*\*İzleme Modu (Armed):\*\* MPU6050 sensörü sürekli olarak X, Y ve Z eksenlerindeki ivme/açı değişimlerini okur.

2\. \*\*Tetiklenme (Triggered):\*\* Belirlenen eşik (threshold) değerinin üzerinde bir hareket algılandığında sistem "Alarm" moduna geçer.

3\. \*\*Eylemler:\*\*

&nbsp;  - Harici LED hızlı (örneğin 100ms aralıklarla) yanıp sönmeye başlar.

&nbsp;  - Seri port üzerinden PC'ye `\[UYARI] Hareket algilandi!` verisi yollanır.

&nbsp;  - Notify servisi üzerinden akıllı telefona bildirim ateşlenir.

4\. \*\*Devre Dışı Bırakma (Disarm):\*\* LCD ekranda numpad belirir. Kullanıcı doğru şifreyi dokunmatik ekrandan girene kadar alarm devam eder. Şifre doğru girildiğinde LED söner, PC'ye ve telefona \*"Sistem Devre Dışı Bırakıldı"\* bilgisi gider, sistem tekrar izleme moduna dönmek için bekler.



