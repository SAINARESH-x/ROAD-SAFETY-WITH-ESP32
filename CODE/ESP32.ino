#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define MQ3_AO 34
#define IR_PIN 32
#define BUZZER 25

LiquidCrystal_I2C lcd(0x27, 16, 2);

unsigned long eyeClosedStart = 0;
bool wasEyeClosed = false;

void setup() {
  Serial.begin(115200);
  pinMode(IR_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(1500);
  lcd.clear();
}

void loop() {
  int alcoholLevel = analogRead(MQ3_AO);
  int blink = digitalRead(IR_PIN);

  // Display on Serial
  Serial.print("Alcohol Level: "); Serial.print(alcoholLevel);
  Serial.print("\tEye: "); Serial.println(blink == HIGH ? "Closed" : "Open");

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Alcohol:"); lcd.print(alcoholLevel); lcd.print("   ");
  lcd.setCursor(0, 1);
  lcd.print("Eye:"); lcd.print(blink == HIGH ? "Closed " : "Open   ");

  // Alcohol detection
  if (alcoholLevel > 3000) {
    Serial.println("⚠️ Alcohol Detected!");
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("ALCOHOL ALERT!");
    digitalWrite(BUZZER, HIGH);
    delay(2000);
    lcd.clear();
  }

  // Drowsiness detection
  if (blink == HIGH) {
    if (!wasEyeClosed) eyeClosedStart = millis();
    wasEyeClosed = true;

    if (millis() - eyeClosedStart > 1500) { // closed > 1.5 sec
      Serial.println("😴 Drowsiness Detected!");
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("DROWSINESS ALERT");
      digitalWrite(BUZZER, HIGH);
    }
  } else {
    wasEyeClosed = false;
    digitalWrite(BUZZER, LOW);
  }

  delay(100);
}