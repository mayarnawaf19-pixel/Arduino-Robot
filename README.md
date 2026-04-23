# Smart Arduino Bluetooth-Controlled Robot

A smart embedded robotics project built using Arduino that combines Bluetooth control, password security, sensor integration, and autonomous obstacle avoidance.

---

## 📌 Project Overview

This robot system allows users to control movement via a Bluetooth mobile connection after entering a secure password. It also monitors environmental conditions and reacts automatically to obstacles in its path.

The system is designed as a compact demonstration of embedded systems, combining real-time control, sensors, and simple decision-making logic.

---

## Features

- Bluetooth remote control (via serial commands)
- Password-protected access system (stored in EEPROM)
- Real-time temperature & humidity monitoring (DHT11)
- LCD display (I2C 16x2) for system status
- Obstacle detection using ultrasonic sensor (HC-SR04)
- Automatic collision avoidance (reverse + stop)
- Buzzer alerts for warnings and system feedback
- Motor control using H-bridge logic (IN1–IN4)

---

## System Behavior

1. On startup:
   - The system checks EEPROM for saved unlock status.
   - If locked, it requests a password via Bluetooth.

2. Authentication:
   - Correct password unlocks the system.
   - Wrong password resets input and notifies the user.

3. Control Mode:
   - The robot receives commands via Bluetooth:
     - F → Move Forward  
     - B → Move Backward  
     - L → Turn Left  
     - R → Turn Right  
     - S → Stop  
     - E → Lock system  

4. Safety System:
   - If an obstacle is detected within a safe distance:
     - The robot stops immediately
     - Moves backward
     - Triggers buzzer alert

5. Monitoring:
   - Temperature and humidity are continuously displayed on the LCD.

---

## Hardware Requirements

- Arduino UNO (or compatible board)
- HC-05 / HC-06 Bluetooth module
- DHT11 temperature & humidity sensor
- HC-SR04 ultrasonic sensor
- I2C 16x2 LCD display
- L298N motor driver (or equivalent)
- DC motors (x2 or x4 depending on chassis)
- Buzzer
- Power supply / battery pack

---

## Software Requirements

- Arduino IDE
- Libraries:
  - Wire.h
  - LiquidCrystal_I2C.h
  - SoftwareSerial.h
  - DHT.h
  - EEPROM.h

---

## Bluetooth Commands

| Command | Action         |
|----------|----------------|
| F        | Move Forward   |
| B        | Move Backward  |
| L        | Turn Left      |
| R        | Turn Right     |
| S        | Stop           |
| E        | Lock System    |

## Installation

1. Open the Arduino code in Arduino IDE  
2. Install required libraries  
3. Upload to Arduino board
