import tkinter as tk
import serial
import threading
import time
import sys

# Kendi ST-Link COM portunu yaz
PORT_ADI = 'COM10'
BAUD_RATE = 115200

class AkilliKilitSistemi:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Akıllı Kilit")
        
        # INTEL DPST KORUMASI: Saf siyah yerine çok koyu gri (#121212) kullanıyoruz
        self.root.configure(bg='#121212')
        self.root.attributes('-fullscreen', True) 
        self.root.attributes('-topmost', True)    
        self.root.config(cursor="none")           
        
        # YAZI RENGİ: Parlak yeşil (#00FF00) yapıldı
        self.label = tk.Label(self.root, text="SİSTEM KİLİTLİ\nLütfen STM32 ekranından şifrenizi giriniz.", 
                              fg="#00FF00", bg="#121212", font=("Arial", 26, "bold"))
        self.label.place(relx=0.5, rely=0.5, anchor="center")
        
        self.is_locked = False
        self.root.withdraw()
        
        # ACİL DURUM ÇIKIŞI
        self.root.bind('<Escape>', self.acil_cikis)

    def acil_cikis(self, event):
        print("Acil çıkış yapıldı!")
        self.root.destroy()
        sys.exit()

    def kilitle(self):
        if not self.is_locked:
            self.root.deiconify() 
            self.root.attributes('-fullscreen', True)
            self.root.attributes('-topmost', True)
            self.is_locked = True

    def kilidi_ac(self):
        if self.is_locked:
            self.root.withdraw() 
            self.is_locked = False

def seri_port_dinle(app):
    try:
        stm32 = serial.Serial(PORT_ADI, BAUD_RATE, timeout=1)
        print(f"Bağlantı Başarılı! {PORT_ADI} dinleniyor...")
    except Exception as e:
        print(f"HATA! {PORT_ADI} açılamadı: {e}")
        return

    while True:
        try:
            if stm32.in_waiting > 0:
                gelen_veri = stm32.readline().decode('utf-8', errors='ignore').strip()
                
                if gelen_veri == "LOCK":
                    print(">>> KİLİTLEME KOMUTU GELDİ!")
                    app.root.after(0, app.kilitle)
                    
                elif gelen_veri == "UNLOCK":
                    print(">>> AÇMA KOMUTU GELDİ!")
                    app.root.after(0, app.kilidi_ac)
                    
        except Exception as e:
            print("Seri port hatası:", e)
            break
        time.sleep(0.05)

if __name__ == "__main__":
    uygulama = AkilliKilitSistemi()
    dinleme_thread = threading.Thread(target=seri_port_dinle, args=(uygulama,), daemon=True)
    dinleme_thread.start()
    uygulama.root.mainloop()