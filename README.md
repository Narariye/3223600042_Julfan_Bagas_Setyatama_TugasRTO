SP32-S3 RTOS — Percobaan Peripheral (2 Core) -- Wokwi Simulator
Dokumentasi singkat untuk menghubungkan dan menjalankan peripheral pada ESP32-S3 (2 core) menggunakan FreeRTOS.

Perangkat yang digunakan:

3 × LED
2 × Push button
1 × Buzzer (active/passive)
1 × OLED (I2C, 128x64, addr 0x3C)
1 × Potensiometer (ADC)
1 × Rotary encoder (A, B, push)
1 × Motor stepper (dengan driver step/dir)
1 × Servo (PWM)

<img width="647" height="630" alt="image" src="https://github.com/user-attachments/assets/a93557af-12de-48f3-9283-0d3f45da5e9f" />

LED1: GPIO10
LED2: GPIO11
LED3: GPIO12
BUTTON1: GPIO13 (INPUT_PULLUP)
BUTTON2: GPIO14 (INPUT_PULLUP)
BUZZER (PWM): GPIO3
OLED (I2C): SDA = GPIO8, SCL = GPIO9
POT (ADC): GPIO1 (ADC1 channel)
ENCODER CLK (A): GPIO6
ENCODER DT (B): GPIO7
ENCODER SW: GPIO5
STEPPER IN1 (A-): GPIO21
STEPPER IN2 (A+): GPIO39
STEPPER IN3 (B+): GPIO40
STEPPER IN3 (B-): GPIO38
SERVO (PWM): GPIO16

Cara menjalankan tiap I/O di tiap core (FreeRTOS)
Pada ESP32 dapat membuat task FreeRTOS dan mem-pinnya ke core tertentu dengan xTaskCreatePinnedToCore. Contoh singkat pembuatan 2 tugas: satu di core 0 dan satu di core 1.

Hasil demo terdapat di drive berikut:
https://drive.google.com/drive/folders/1CggXNAXAsgzcbwBkyvPdySSt897X0TBs?usp=sharing
