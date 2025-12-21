#include "DS3231.h"
#include <Servo.h>
#include "LiquidCrystal_SoftI2C.h"

SoftwareWire myWire(6, 7);
DS3231 rtc(&myWire);
LiquidCrystal_I2C lcd(0x26, 16, 2, &myWire);

Servo sTemp, sMin, sStd, sFuel;
Time t;

const int pTemp = 5, pMin = 4, pStd = 3, pFuel = 2, pBat = A0;
int oMin = -1, oStd = -1, oTemp = -1, oFuel = -1;

void moveAndDetach(Servo &s, int pin, int angle) {
  s.attach(pin);
  s.write(constrain(angle, 0, 180));
  delay(600);
  s.detach();
}

void setup() {
  Serial.begin(9600);
  myWire.begin(); 
  lcd.begin();
  rtc.begin();
  
  lcd.print("Willkommen Sibel");
  lcd.setCursor(0, 1);
  lcd.print("       <3       ");
  lcd.clear();

  t = rtc.getTime();
  if (t.year < 2020) {
    rtc.setTime(23, 30, 0);
    rtc.setDate(21, 12, 2025);
    lcd.print("FALSCHE UHRZEIT");
    lcd.setCursor(0, 1);
    lcd.print("Schlag Per!");
  }
}

void loop() {
  t = rtc.getTime();
  float currentTemp = rtc.getTemp() - 2.25;

  // --- GRUPPE 1: STUNDEN  ---
  int h = t.hour % 12; if (h == 0) h = 12;
  if (h != oStd) {
    int angle;
    if (h <= 3)       angle = map(h, 1, 3,   163, 138); 
    else if (h <= 6)  angle = map(h, 3, 6,  138, 97);
    else if (h <= 9)  angle = map(h, 6, 9,  97, 57);
    else              angle = map(h, 9, 12, 57, 19);

    moveAndDetach(sStd, pStd, angle);
    oStd = h;
  }

  // --- GRUPPE 2: MINUTEN  ---
  if (t.min != oMin) {
    int angle;
    if (t.min <= 15)      angle = map(t.min, 0, 15,  171, 140); 
    else if (t.min <= 30) angle = map(t.min, 15, 30, 140, 97);
    else if (t.min <= 45) angle = map(t.min, 30, 45, 97, 53);
    else                  angle = map(t.min, 45, 60, 53, 11);

    moveAndDetach(sMin, pMin, angle);
    oMin = t.min;
  }

  // --- GRUPPE 3: TEMPERATUR  ---
  int tInt = (int)currentTemp;
  if (tInt != oTemp) {
     int angle;
    if (tInt <= 16)      angle = map(tInt, 0, 16,  171, 145); 
    else if (tInt <= 20) angle = map(tInt, 16, 20, 145, 130);
    else if (tInt <= 25) angle = map(tInt, 20, 25, 130, 116);
    else                 angle = map(tInt, 25, 30, 116, 85);
    moveAndDetach(sTemp, pTemp, angle);
    oTemp = tInt;
  }
  /*
  // --- GRUPPE 4: FUEL / SPANNUNG  ---
  float volt = analogRead(pBat) * (5 / 1023.0);
  int fPos = map(volt * 100, 200, 320, 0, 180);
  if (abs(fPos - oFuel) > 2) {
    moveAndDetach(sFuel, pFuel, fPos);
    oFuel = fPos;
  }
  */
  // LCD Anzeige
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 1);
  lcd.print("Temp: "); lcd.print(currentTemp, 1); lcd.print(" C ");
  
  Serial.print(rtc.getTimeStr());
  Serial.print(" | Temp: ");
  Serial.print(currentTemp);
  Serial.print("C | Bat: ");
  //Serial.print(volt);
  Serial.println("V");

  delay(2000);
}