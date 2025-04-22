#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Ultrasonic.h>

// === Pin Definitions ===
#define SS_PIN D4
#define RST_PIN D3
#define TRIG_PIN D0
#define ECHO_PIN D8
#define SERVO_PIN D9
#define BUZZER_PIN D10  // Using TX pin (GPIO1)

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo gateServo;
Ultrasonic ultrasonic(TRIG_PIN, ECHO_PIN);

const float TOLL_AMOUNT = 2.0;

struct UserData {
  String rfidTag;
  float balance;
  char name[16];
};

UserData users[] = {
  {"63 22 30 28", 50.0, "Uttam"},
  {"C3 E9 40 16", 10.0, "Poonam"},
  {"F5 18 8D 01", 1.0,  "Nishant"}
};

int numUsers = sizeof(users) / sizeof(users[0]);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();

  Wire.begin(D1, D2);
  lcd.begin();
  lcd.backlight();

  gateServo.attach(SERVO_PIN);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  gateServo.write(0);

  lcd.setCursor(0, 0);
  lcd.print("Toll Gate Ready");
  lcd.setCursor(0, 1);
  lcd.print("Fee: $");
  lcd.print(TOLL_AMOUNT);
}

void loop() {
  if (ultrasonic.read(CM) < 10) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Vehicle Detected");
    delay(1500);
    processRFID();
  }
}

void processRFID() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan RFID Card");
  Serial.print("Scan RFID Card");
  lcd.setCursor(0, 1);
  lcd.print("Fee: $");
  lcd.print(TOLL_AMOUNT);

  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
      String rfidTag = "";
      for (byte i = 0; i < rfid.uid.size; i++) {
        if (i > 0) rfidTag += " ";
        if (rfid.uid.uidByte[i] < 0x10) rfidTag += "0";
        rfidTag += String(rfid.uid.uidByte[i], HEX);
      }
      rfidTag.toUpperCase();

      int userIndex = -1;
      for (int i = 0; i < numUsers; i++) {
        if (rfidTag == users[i].rfidTag) {
          userIndex = i;
          break;
        }
      }

      lcd.clear();
      if (userIndex >= 0) {
        lcd.setCursor(0, 0);
        lcd.print(users[userIndex].name);

        if (users[userIndex].balance >= TOLL_AMOUNT) {
          users[userIndex].balance -= TOLL_AMOUNT;

          lcd.setCursor(0, 1);
          lcd.print("Bal:$");
          lcd.print(users[userIndex].balance);

          // buzzer success
          digitalWrite(BUZZER_PIN, HIGH);
          delay(200);
          digitalWrite(BUZZER_PIN, LOW);

          delay(1500);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Payment Success");
          lcd.setCursor(0, 1);
          lcd.print("Gate Opening");

          gateServo.write(180);
          delay(4000);
          gateServo.write(0);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Gate Closed");
          lcd.setCursor(0, 1);
          lcd.print("Thank You!");
        } else {
          lcd.setCursor(0, 1);
          lcd.print("Low Bal:$");
          lcd.print(users[userIndex].balance);

          //  Buzzer alert
          for (int i = 0; i < 3; i++) {
            digitalWrite(BUZZER_PIN, HIGH);
            delay(200);
            digitalWrite(BUZZER_PIN, LOW);
            delay(200);
          }

          delay(2000);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Insufficient");
          lcd.setCursor(0, 1);
          lcd.print("No Entry");
        }
      } else {
        lcd.setCursor(0, 0);
        lcd.print("Unknown Card");
        lcd.setCursor(0, 1);
        lcd.print("Access Denied");

        // Buzzer alert
        for (int i = 0; i < 3; i++) {
          digitalWrite(BUZZER_PIN, HIGH);
          delay(200);
          digitalWrite(BUZZER_PIN, LOW);
          delay(200);
        }
      }

      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Toll Gate Ready");
      lcd.setCursor(0, 1);
      lcd.print("Fee: $");
      lcd.print(TOLL_AMOUNT);

      rfid.PICC_HaltA();
      return;
    }
    delay(100);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timeout");
  lcd.setCursor(0, 1);
  lcd.print("No Card Read");

  // Timeout buzzer
  for (int i = 0; i < 2; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }

  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Toll Gate Ready");
  lcd.setCursor(0, 1);
  lcd.print("Fee: $");
  lcd.print(TOLL_AMOUNT);
}
