
⚙️ 1️⃣ Tujuan Percobaan

Menguji sistem kontrol berbasis ESP32-S3 yang mengendalikan:

Motor servo dengan potensiometer,

Motor stepper dengan rotary encoder,

LED indikator heartbeat (kedip setiap 500ms),

Buzzer aktif saat tombol ditekan,

OLED display (sementara dinonaktifkan di kode).

⚙️ 2️⃣ Peralatan dan Komponen
No	Komponen	Jumlah	Keterangan
1	ESP32-S3 WROOM-1	1	Mikrokontroler utama
2	Motor servo (SG90 / MG90)	1	Digunakan untuk kontrol posisi
3	Motor stepper + Driver A4988	1 set	Untuk kontrol gerakan presisi
4	Potensiometer 10kΩ	1	Input analog untuk servo
5	Rotary Encoder	1	Mengatur posisi stepper
6	Buzzer aktif	1	Indikator bunyi tombol
7	3x LED	3	Indikator visual
8	Tombol tekan	2	Untuk input manual
9	OLED I2C 128×64	1	(opsional, bisa diaktifkan nanti)
10	Kabel jumper & breadboard	secukupnya	Penghubung komponen
⚙️ 3️⃣ Langkah Perakitan

(merujuk ke gambar yang kamu kirim)

Hubungkan daya & ground bersama:

Semua GND komponen → GND ESP32

Semua VCC (3.3V atau 5V sesuai komponen) → pin 3V3 / 5V ESP32

Sambungkan pin sesuai kode:

Komponen	Pin ESP32	Keterangan
Servo	17	Kontrol PWM servo
Stepper (A4988)	STEP: 13, DIR: 12	Kontrol langkah & arah
Rotary Encoder	CLK: 18, DT: 19	Input arah dan langkah
Potensiometer	OUT → 5 (ADC)	Input analog
Tombol	6	Input digital, aktif LOW
LED utama	2	Kedip “heartbeat”
Buzzer	4	Aktif saat tombol ditekan
OLED	SDA: 8, SCL: 9	I2C (sementara nonaktif di kode)
⚙️ 4️⃣ Langkah Pengujian
🧩 Tahap 1 – Uji Upload & Serial Monitor

Hubungkan ESP32 ke PC.

Buka Arduino IDE / Wokwi.

Pilih board ESP32-S3 Dev Module.

Upload program yang kamu tulis.

Buka Serial Monitor (115200 baud) → pastikan muncul pesan:

Booting Panel Kontrol (MODE DEBUG STEPPER)...
OLED Nonaktif. Tes Stepper dan LED.

💡 Tahap 2 – Uji LED Heartbeat

LED pada pin 2 harus berkedip setiap 0.5 detik (500ms).

Ini menunjukkan loop dan timing sistem berjalan normal.

🧠 Tahap 3 – Uji Potensiometer ke Servo

Putar potensiometer perlahan.

Servo akan mengikuti arah rotasi (antara 0°–180°).

Jika servo bergetar → periksa kabel daya (gunakan 5V eksternal jika perlu).

🌀 Tahap 4 – Uji Rotary Encoder ke Stepper

Putar rotary encoder.

Amati Serial Monitor: harus muncul nilai seperti

Encoder di: 1
Encoder di: 2
Encoder di: 3


Stepper bergerak maju/mundur sesuai arah rotasi.

Jika arah salah, tukar kabel DIR motor atau ubah arah di kode:
stepper.moveTo(-encoderPos * 50);

🔘 Tahap 5 – Uji Tombol dan Buzzer

Tekan tombol pada pin 6 → buzzer menyala.

Lepas tombol → buzzer mati.

🖥️ Tahap 6 – (Opsional) Aktifkan OLED Display

Hapus tanda komentar // pada bagian OLED di kode:

Wire.begin(SDA_PIN, SCL_PIN);
display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


Upload ulang.

Layar akan menampilkan data seperti nilai potensiometer dan posisi encoder.
