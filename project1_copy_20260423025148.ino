#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <DHT.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial bluetooth(10, 11); // RX = 10, TX = 11
#define DHTPIN A3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
        

#define TRIG_PIN 8
#define ECHO_PIN 9
#define BUZZER_PIN 13
#define IN1 7
#define IN2 6
#define IN3 5
#define IN4 4
#define REVERSE_MS 700

const String PASSWORD = "1234";
const int OBSTACLE_DIST = 15;
const int MAX_BEEP_DIST = 50;
const int EEPROM_UNLOCK_ADDR = 0;

bool isUnlocked = false;
bool isMoving = false;
String pwBuffer = "";
char lastCommand = 'S'; // 'F','B','L','R','S','E'

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  if (EEPROM.read(EEPROM_UNLOCK_ADDR) == 1) {
    isUnlocked = true;
    lcd.setCursor(0, 0);
    lcd.print("Access granted");
    lcd.setCursor(0, 1);
    lcd.print("Ready control");
    tone(BUZZER_PIN, 1000, 200);
    delay(1500);
    lcd.clear();
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Enter password:");
  }
}

void loop() {
  if (!isUnlocked) {
    checkPassword();
    return;
  }

  if (bluetooth.available()) {
    char command = bluetooth.read();
    lastCommand = command;
    switch (command) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopMotors(); break;
      case 'E': lockSystem(); return;
    }
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  static unsigned long lastLCDUpdate = 0;

  if (!isnan(h) && !isnan(t) && millis() - lastLCDUpdate > 1000) {
    lastLCDUpdate = millis();
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(t, 1);
    lcd.print("C ");
    lcd.print("H:");
    lcd.print(h, 0);
    lcd.print("%  ");
  }

  if (isMoving) {
    int distance = getDistance();

    if (distance > 0 && distance <= OBSTACLE_DIST) {
      lcd.setCursor(0, 1);
      lcd.print("Obstacle ahead!");
      tone(BUZZER_PIN, 800, 200);
      delay(200);
      noTone(BUZZER_PIN);
      stopMotors();

      
    }
  }
}


void checkPassword() {
  while (bluetooth.available()) {
    char c = bluetooth.read();
    if (isPrintable(c)) pwBuffer += c;
    lcd.setCursor(0, 1);
    for (int i = 0; i < pwBuffer.length(); i++)
      lcd.print('*');

    if (pwBuffer.length() == PASSWORD.length()) {
      if (pwBuffer == PASSWORD) {
        isUnlocked = true;
        EEPROM.write(EEPROM_UNLOCK_ADDR, 1);
        lcd.clear();
        lcd.print("Access granted");
        tone(BUZZER_PIN, 1000, 200);
        delay(1000);
        lcd.clear();
      } else {
        lcd.clear();
        lcd.print("Wrong password");
        tone(BUZZER_PIN, 300, 200);
        delay(1200);
        lcd.clear();
        lcd.print("Enter password:");
      }
      pwBuffer = "";
    }
  }
}

void lockSystem() {
  isUnlocked = false;
  EEPROM.write(EEPROM_UNLOCK_ADDR, 0);
  stopMotors();
  lcd.clear();
  lcd.print("System locked");
  tone(BUZZER_PIN, 600, 300);
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 40000);
  if (duration == 0) return -1;
  return (int)(duration * 0.034 / 2);
}


void moveForward() {
  isMoving = true;
  lcd.setCursor(0, 1);
  lcd.print("Moving forward ");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  isMoving = true;
  lcd.setCursor(0, 1);
  lcd.print("Moving backward");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  isMoving = true;
  lcd.setCursor(0, 1);
  lcd.print("Turning left   ");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  isMoving = true;
  lcd.setCursor(0, 1);
  lcd.print("Turning right  ");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void stopMotors() {
  isMoving = false;
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  lcd.setCursor(0, 1);
  lcd.print("Stopped        ");
}

bool isPrintable(char c) {
  return c >= 32 && c <= 126;
}
