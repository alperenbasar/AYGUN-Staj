import serial
import threading
import requests
import tkinter as tk

# --- YAPILANDIRMA ---
COM_PORT = "COM10"  # Aygıt Yöneticisi'ndeki ST-Link portun (Örn: COM3, COM4, COM5)
BAUD_RATE = 115200
NTFY_TOPIC = "alperen_alarm_istasyonu"  # Telefondaki ntfy kanal adı

alarm_state = False

def send_phone_notification():
    """Android telefona ntfy üzerinden anlık bildirim atar"""
    try:
        requests.post(
            f"https://ntfy.sh/{NTFY_TOPIC}",
            data="DIKKAT! MPU6050 sensorunde izinsiz hareket algilandi!".encode('utf-8'),
            headers={
                "Title": "GUVENLIK ALARMI!",
                "Priority": "urgent",
                "Tags": "warning,rotating_light"
            },
            timeout=5
        )
        print("[BILDIRIM] Telefona uyarı gönderildi.")
    except Exception as e:
        print(f"[BILDIRIM HATASI]: {e}")

def read_serial():
    global alarm_state
    try:
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
        print(f"[SERI PORT] {COM_PORT} dinleniyor...")
        while True:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            if line == "ALARM" and not alarm_state:
                alarm_state = True
                print("[DURUM] ALARM TETIKLENDI!")
                threading.Thread(target=send_phone_notification, daemon=True).start()
            elif line == "DISARMED":
                alarm_state = False
                print("[DURUM] Alarm sıfırlandı (Güvenli).")
    except Exception as e:
        print(f"[SERI PORT HATASI]: {e}")

# --- ARAYÜZ (GUI) ---
root = tk.Tk()
root.title("Güvenlik & Telemetri İstasyonu")
root.geometry("650x420")
root.configure(bg="#121212")

status_label = tk.Label(
    root, 
    text="SİSTEM GÜVENLİ\n(İzlemede)", 
    font=("Consolas", 22, "bold"), 
    fg="#00FF66", 
    bg="#121212"
)
status_label.pack(expand=True)

flash = False
def update_ui():
    global flash
    if alarm_state:
        flash = not flash
        bg_col = "#D32F2F" if flash else "#380000"
        root.configure(bg=bg_col)
        status_label.configure(
            text="⚠ DİKKAT! HAREKET ALGILANDI! ⚠\nSİSTEM ALARMDA", 
            fg="white", 
            bg=bg_col
        )
    else:
        root.configure(bg="#121212")
        status_label.configure(
            text="SİSTEM GÜVENLİ\n(İzlemede)", 
            fg="#00FF66", 
            bg="#121212"
        )
    root.after(250, update_ui)

# Arka plan seri port okuma thread'i
threading.Thread(target=read_serial, daemon=True).start()

update_ui()
root.mainloop()