#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <ESP32Servo.h>

// Button (Tombol)
#define BUTTON_PIN 12 // GPIO12

// Buzzer
#define BUZZ_PIN 10 // GPIO10

// LED
#define LED_PIN 11 // GPIO11

// Motor Stepper (A4988)
#define STEP_PIN 40 // GPIO40
#define DIR_PIN  39 // GPIO39

// OLED (I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 8  // GPIO8
#define OLED_SCL 9  // GPIO9

// Potentiometer
#define POT_PIN 4 // GPIO4 (ADC1)

// Rotary Encoder
#define CLK_PIN 1 // GPIO1
#define DT_PIN  2 // GPIO2

// Servo
#define SERVO_PIN 18 // GPIO18

// OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
int counter = 0;

// Stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Servo
Servo myservo;

// Rotary
volatile int count = 0;
int lastState;

void TaskButton(void *pvParameters) {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Task Tombol berjalan di Core 1 [Prioritas 3]");

  while (1) {
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("Tombol Ditekan!");
      vTaskDelay(200 / portTICK_PERIOD_MS); 
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void TaskBuzzer(void *pvParameters) {
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.println("Task Buzzer (Manual) berjalan di Core 0 [Prioritas 1]");

  long beepDuration = 300; 
  int freq = 1000; 

  int halfPeriod_us = 1000000 / freq / 2;

  while (1) {
    
    long startTime = millis();
    while (millis() - startTime < beepDuration) {
      digitalWrite(BUZZ_PIN, HIGH);
      delayMicroseconds(halfPeriod_us);
      digitalWrite(BUZZ_PIN, LOW);
      delayMicroseconds(halfPeriod_us);
    }

    
    digitalWrite(BUZZ_PIN, LOW); 
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void TaskLED(void *pvParameters) {
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Task LED berjalan di Core 0 [Prioritas 1]");
  while (1) {
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED_PIN, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void TaskStepper(void *pvParameters) {
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);
  Serial.println("Task Stepper berjalan di Core 1 [Prioritas 5]");

  while (1) {
    Serial.println("Stepper Maju");
    stepper.moveTo(stepper.currentPosition() + 200);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    vTaskDelay(pdMS_TO_TICKS(200));

    Serial.println("Stepper Mundur");
    stepper.moveTo(stepper.currentPosition() - 200);
    while (stepper.distanceToGo() != 0) {
      stepper.run();
    }
    vTaskDelay(pdMS_TO_TICKS(200));
  }
}


void TaskOLED(void *pvParameters) {
  Wire.begin(OLED_SDA, OLED_SCL);
  Serial.println("Task OLED berjalan di Core 1 [Prioritas 1]");

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Gagal!");
    vTaskDelete(NULL);
  }
  Serial.println("OLED OK!");

  while (1) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("Test OLED ESP32-S3");
    display.setCursor(0, 20);
    display.print("Counter: ");
    display.println(counter);
    display.setCursor(0, 30);
    display.print("Core: ");
    display.println(xPortGetCoreID());

    display.display();
    counter++;
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


void TaskPot(void *pvParameters) {
  Serial.println("Task Potensio berjalan di Core 1 [Prioritas 2]");
  while (1) {
    int value = analogRead(POT_PIN);
    Serial.print("Potensio: ");
    Serial.println(value);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void TaskEncoder(void *pvParameters) {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  lastState = digitalRead(CLK_PIN);
  Serial.println("Task Encoder berjalan di Core 0 [Prioritas 5]");

  while (1) {
    int currentState = digitalRead(CLK_PIN);
    if (currentState != lastState) {
      if (digitalRead(DT_PIN) != currentState) count++;
      else count--;
      Serial.print("Encoder: ");
      Serial.println(count);
    }
    lastState = currentState;
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}


void TaskServo(void *pvParameters) {
  myservo.setPeriodHertz(50);
  myservo.attach(SERVO_PIN, 500, 2400);

  Serial.println("Task Servo (ESP32Servo) berjalan di Core 1 [Prioritas 3]");

  while (1) {
    myservo.write(0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    myservo.write(90);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    myservo.write(180);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}



void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS); 
  Serial.println("=== SISTEM GABUNGAN DIMULAI (FIXED SEMUA) ===");

  
  xTaskCreatePinnedToCore(TaskEncoder, "ENC",   2048, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(TaskBuzzer,  "BUZZ",  2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskLED,     "LED",   2048, NULL, 1, NULL, 0);

  xTaskCreatePinnedToCore(TaskStepper, "STEP",    4096, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(TaskButton,  "BUTTONS", 2048, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(TaskServo,   "SERVO",   4096, NULL, 3, NULL, 1);
  xTaskCreatePinnedToCore(TaskPot,     "POT",     2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskOLED,    "OLED",    4096, NULL, 1, NULL, 1);
}


void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}