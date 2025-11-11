#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AccelStepper.h>
#include <ESP32Servo.h>

// ===============================
// OLED DISPLAY
// ===============================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===============================
// STEPPER MOTOR
// ===============================
AccelStepper stepper(AccelStepper::FULL4WIRE, 21, 39, 40, 38);

// ===============================
// SERVO MOTOR
// ===============================
Servo servo;
#define SERVO_PIN 16

// ===============================
// PIN DEFINITIONS
// ===============================
const int LED1_PIN = 10;
const int LED2_PIN = 11;  
const int LED3_PIN = 12;
const int BUZZER_PIN = 3;
const int BUTTON_PIN = 13;
const int ENCODER_CLK = 6;
const int ENCODER_DT = 7;
const int POT_PIN = 1;

// ===============================
// VARIABLES
// ===============================
int potValue = 0;
int lastPotValue = 0;
int encoderValue = 0;
int lastEncoderState = 0;
int servoAngle = 90;
bool buttonState = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting System...");
  
  // Initialize I2C for OLED
  Wire.begin(8, 9);
  
  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED failed");
  } else {
    Serial.println("OLED OK");
  }
  
  // Initialize LEDs & Buzzer
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize Encoder
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_DT, INPUT);
  
  // Initialize Stepper
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(400);
  stepper.setCurrentPosition(0);
  
  // Initialize Servo - FIXED
  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);  // Standard 50Hz servo
  servo.attach(SERVO_PIN, 500, 2400); // Min 500us, Max 2400us
  servo.write(90); // Start at center position
  Serial.println("Servo initialized");
  
  // Welcome message
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("SIMPLE CONTROL");
  display.println("Servo: Pot");
  display.println("Stepper: Encoder");
  display.display();
  
  // Welcome beep
  tone(BUZZER_PIN, 1500, 200);
  delay(1000);
  
  Serial.println("System Ready!");
}

void loop() {
  // Read potentiometer
  potValue = analogRead(POT_PIN);
  
  // Debug potentiometer values
  if (abs(potValue - lastPotValue) > 10) {
    Serial.print("Pot Value: ");
    Serial.println(potValue);
    lastPotValue = potValue;
  }
  
  // Control Servo with Potentiometer - FIXED MAPPING
  servoAngle = map(potValue, 0, 4095, 0, 180);
  servoAngle = constrain(servoAngle, 0, 180);
  servo.write(servoAngle);
  
  // Read encoder
  readEncoder();
  
  // Control Stepper with Encoder
  int targetPos = encoderValue * 100; // 100 steps per encoder click
  stepper.moveTo(targetPos);
  stepper.run();
  
  // LED effects
  controlLEDs();
  
  // Button function
  if (!digitalRead(BUTTON_PIN) && !buttonState) {
    buttonState = true;
    tone(BUZZER_PIN, 2000, 150);
    // Reset positions
    encoderValue = 0;
    stepper.setCurrentPosition(0);
    Serial.println("Reset positions");
    delay(300);
  }
  
  if (digitalRead(BUTTON_PIN)) {
    buttonState = false;
  }
  
  // Update display
  updateDisplay();
  
  delay(50);
}

void readEncoder() {
  int currentCLK = digitalRead(ENCODER_CLK);
  
  if (currentCLK != lastEncoderState && currentCLK == LOW) {
    if (digitalRead(ENCODER_DT) == LOW) {
      encoderValue++;
      Serial.print("Encoder ++: ");
      Serial.println(encoderValue);
      tone(BUZZER_PIN, 1200, 30);
    } else {
      encoderValue--;
      Serial.print("Encoder --: ");
      Serial.println(encoderValue);
      tone(BUZZER_PIN, 800, 30);
    }
    // Limit range
    encoderValue = constrain(encoderValue, -40, 40);
  }
  lastEncoderState = currentCLK;
}

void controlLEDs() {
  // LED1 - blinks with activity
  digitalWrite(LED1_PIN, (millis() / 500) % 2);
  
  // LED2 - on when stepper moving forward
  digitalWrite(LED2_PIN, encoderValue > 0);
  
  // LED3 - on when servo > 90 degrees
  digitalWrite(LED3_PIN, servoAngle > 90);
}

void updateDisplay() {
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate < 300) return;
  lastUpdate = millis();
  
  display.clearDisplay();
  display.setCursor(0, 0);
  
  display.println("CONTROL SYSTEM");
  display.println("==============");
  display.print("Servo: ");
  display.print(servoAngle);
  display.println(" deg");
  display.print("Stepper: ");
  display.println(stepper.currentPosition());
  display.print("Encoder: ");
  display.println(encoderValue);
  display.print("Pot: ");
  display.println(potValue);
  
  // Visual indicator
  display.print("Servo Pos: [");
  int barWidth = map(servoAngle, 0, 180, 0, 20);
  for(int i = 0; i < 20; i++) {
    if(i < barWidth) display.print("=");
    else display.print(" ");
  }
  display.println("]");
  
  display.display();
}