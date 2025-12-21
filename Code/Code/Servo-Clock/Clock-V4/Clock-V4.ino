#include "DS3231.h"
#include <Servo.h>
#include "LiquidCrystal_SoftI2C.h"

SoftwareWire myWire(6, 7);
DS3231 rtc(&myWire);
LiquidCrystal_I2C lcd(0x26, 16, 2, &myWire);

Servo sTemp, sMin, sStd, sFuel;
Time t;

const int pTemp = 2, pMin = 4, pStd = 3, pFuel = 5, pBat = A0;
int oMin = -1, oStd = -1, oTemp = -1, oFuel = -1;

void moveAndDetach(Servo &s, int pin, int angle) {
  s.attach(pin);
  s.write(constrain(angle, 0, 180));
  delay(600);
  s.detach();
}

void setup() {
  myWire.begin(); 
  lcd.begin();
  rtc.begin();
  
  lcd.print("Willkommen Sibel");
  lcd.setCursor(0, 1);
  lcd.print("       <3       ");
  delay(4000);
  lcd.clear();

  t = rtc.getTime();
  if (t.year < 2025) {
    rtc.setTime(12, 0, 0);
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
  int h = (t.hour >= 12) ? t.hour - 12 : t.hour;
  if (h != oStd) {
    int angle;
    if (h <= 3)       angle = map(h, 0, 3,   0, 45); 
    else if (h <= 6)  angle = map(h, 3, 6,  45, 95); // 95 als Korrekturbeispiel
    else if (h <= 9)  angle = map(h, 6, 9,  95, 135);
    else              angle = map(h, 9, 12, 135, 180);

    moveAndDetach(sStd, pStd, angle);
    oStd = h;
  }

  // --- GRUPPE 2: MINUTEN  ---
  if (t.min != oMin) {
    int angle;
    if (t.min <= 15)      angle = map(t.min, 0, 15,  163, 125); 
    else if (t.min <= 30) angle = map(t.min, 15, 30, 125, 95);
    else if (t.min <= 45) angle = map(t.min, 30, 45, 95, 55);
    else                  angle = map(t.min, 45, 60, 55, 15);

    moveAndDetach(sMin, pMin, angle);
    oMin = t.min;
  }

  // --- GRUPPE 3: TEMPERATUR  ---
  int tInt = (int)currentTemp;
  if (tInt != oTemp) {
    int angle = map(tInt, 15, 35, 0, 180);
    moveAndDetach(sTemp, pTemp, angle);
    oTemp = tInt;
  }

  // --- GRUPPE 4: FUEL / SPANNUNG  ---
  float volt = analogRead(pBat) * (5.0 / 1023.0);
  int fPos = map(volt * 100, 200, 320, 0, 180);
  if (abs(fPos - oFuel) > 2) {
    moveAndDetach(sFuel, pFuel, fPos);
    oFuel = fPos;
  }

  // LCD Anzeige
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 1);
  lcd.print("Temp: "); lcd.print(currentTemp, 1); lcd.print(" C ");

  delay(1000);
}