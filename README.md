# Automated Toll Gate Collection System using NodeMCU

This IoT-based project automates toll collection using NodeMCU (ESP8266), RFID technology, an ultrasonic sensor for vehicle detection, a servo motor for gate control, and a cloud database for logging transactions.

## 🚀 Project Overview

The system detects an approaching vehicle using an ultrasonic sensor. The driver then scans an RFID card, which is validated against a cloud database (e.g., MongoDB Atlas). If the card is valid and the balance is sufficient, the toll is deducted, and the gate opens automatically. The transaction is logged in the database, and feedback is displayed via LCD and a buzzer.

## 🧠 Features

- Vehicle detection using ultrasonic sensor (HC-SR04)
- RFID-based identification and authentication
- Real-time database integration using MongoDB Atlas (can be Firebase)
- Automated gate control using servo motor
- LCD display for status messages
- Buzzer for access feedback
- Secure and contactless toll payment system

## 🛠 Components Used

- NodeMCU (ESP8266)
- RFID Reader (RC522) and RFID Tags
- Ultrasonic Sensor (HC-SR04)
- Servo Motor (SG90)
- LCD 16x2 with I2C
- Buzzer
- MongoDB Atlas (NoSQL cloud database)
- Breadboard, jumper wires

## 🔧 Setup Instructions

1. *Hardware Connections*
   - Connect RFID, ultrasonic sensor, servo motor, LCD, and buzzer to NodeMCU as per the provided circuit diagram.

2. *Arduino Programming*
   - Open toll_gate_firmware.ino in Arduino IDE.
   - Install necessary libraries:
     - ESP8266WiFi
     - MFRC522
     - Servo
     - LiquidCrystal_I2C
   - Enter your WiFi credentials and MongoDB API details.
   - Upload the code to NodeMCU.

3. *MongoDB Atlas Setup*
   - Create a cluster and database.
   - Create a collection to store RFID user data and logs.
   - Use Node.js or a serverless function to connect NodeMCU with MongoDB via HTTP API or backend.

4. *Testing*
   - Power up the system.
   - Bring a vehicle (or hand) in range of the ultrasonic sensor.
   - Scan an RFID tag.
   - Observe gate action, LCD display, and buzzer sound.
