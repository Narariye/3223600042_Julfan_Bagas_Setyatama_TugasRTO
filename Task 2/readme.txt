Tujuan dan Gambaran Umum
Proyek ini bertujuan menghubungkan berbagai peripheral ke satu ESP32-S3 DevKit dan menjalankan masing-masing I/O pada core berbeda menggunakan fitur dual-core dengan FreeRTOS. Semua simulasi dilakukan pada Wokwi, bukan pada VSCode/PlatformIO.

Daftar Peripheral dan Fungsinya
No	Peripheral	Fungsi	Jenis	Core Digunakan
1	OLED SSD1306 (I²C 128×64)	Menampilkan string	Output	Core 1
2	LED	Indikator nyala/mati	Output	Core 0
3	Push Button	Input digital	Input	Core 1
4	Buzzer	Indikator suara	Output	Core 1
5	Potentiometer	Input analog (ADC)	Input	Core 1
6	Motor Stepper + Driver A4988	Gerakan motor presisi	Output	Core 1
7	Rotary Encoder	Pembacaan posisi/sudut	Input	Core 0
8	Servo Motor	Gerakan sudut	Output	Core 0
Panduan Langkah pada Wokwi
1. Persiapan Simulator Wokwi
Buka https://wokwi.com/

Pilih "New Project"

Pilih "ESP32-S3 DevKit" sebagai board utama.

2. Tambahkan dan Atur Peripheral
Tambahkan komponen yang sesuai dari menu "Add Part": OLED, Stepper + Driver A4988, Push Button, LED, Buzzer, Potentiometer, Rotary Encoder, Servo Motor.

Lakukan wiring virtual sesuai dengan diagram yang sama dengan gambarmu (pastikan connection sudah sesuai referensi pin ESP32-S3).

Simpan konfigurasi wiring ke file diagram.json jika ingin export/import di proyek Wokwi.

3. Penulisan Program
Di editor kode pada Wokwi (di file main.cpp), gunakan library FreeRTOS beserta multi-core task.

Implementasikan setiap peripheral sebagai sebuah task yang berbeda, lalu pin task tersebut ke core yang sesuai menggunakan xTaskCreatePinnedToCore(). Contoh pemanggilan:

cpp
xTaskCreatePinnedToCore(
  TaskOLED, "OLEDTask", 4096, NULL, 1, NULL, 1 // Core 1
);
xTaskCreatePinnedToCore(
  TaskLED, "LEDTask", 2048, NULL, 1, NULL, 0 // Core 0
);
// dst untuk task lain sesuai kebutuhan
Untuk komunikasi antar task, gunakan queue, semaphore, ataupun variable global yang sesuai manajemen RTOS.

4. Build dan Jalankan Simulasi
Setelah wiring dan kode lengkap, klik "Start Simulation" di pojok kanan atas pada Wokwi.


Wokwi akan menjalankan virtual ESP32 dan peripheral, sehingga kamu bisa melihat hasil simulasi secara real-time pada GUI.

dokumentasi dapat dilihat di drive berikut
https://drive.google.com/drive/folders/1sEPxEbqL2zgaJsMFHlgMa53SJ9DTDomd
