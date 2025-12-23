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
unsigned long previousMillis = 0;

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

  lcd.setCursor(3, 0);
  lcd.print("Willkommen");
  lcd.setCursor(3, 1);
  lcd.print("Sibel");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("      <3       ");
  delay(3000);
  lcd.clear();



  sTemp.attach(5);
  sMin.attach(4);
  sStd.attach(3);
  sFuel.attach(2);
  
  
  lcd.setCursor(3,0);
  lcd.print("Startup...");
  lcd.setCursor(3,1);
  lcd.print("Test-Temp");
  for (int pos = 171; pos >= 85; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sTemp.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Startup...");
  lcd.setCursor(5,1);
  lcd.print("OK :)");
  delay(1000);
  lcd.setCursor(3,1);
  lcd.print("Test-Min");
  for (int pos = 171; pos >= 11; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sMin.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Startup...");
  lcd.setCursor(5,1);
  lcd.print("OK :)");
  delay(1000);
  lcd.setCursor(3,1);
  lcd.print("Test-Hour");
  for (int pos = 163; pos >= 19; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sStd.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Startup...");
  lcd.setCursor(5,1);
  lcd.print("OK :)");
  delay(1000);
  lcd.setCursor(3,1);
  lcd.print("Test-Fuel");
  for (int pos = 173; pos >= 83; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    sFuel.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15 ms for the servo to reach the position
  }
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Startup...");
  lcd.setCursor(5,1);
  lcd.print("OK :)");
  delay(500);
  sStd.write(163);
  sMin.write(171);
  sFuel.write(173);
  sTemp.write(171);
  delay(2000);
  lcd.clear();
  sTemp.detach();
  sMin.detach();
  sStd.detach();
  sFuel.detach();

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
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= 2000) {
    previousMillis = currentMillis;

  t = rtc.getTime();
  float currentTemp = rtc.getTemp() - 2.75;

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
  if(t.hour>=21) lcd.noBacklight();
  if(t.hour<21) lcd.backlight();

  }
  // LCD Anzeige
  lcd.setCursor(3, 0);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(3, 1);
  lcd.print("Temp:"); lcd.print(rtc.getTemp() - 2.75, 1);
  
  Serial.print(rtc.getTimeStr());
  Serial.print(" | Temp: ");
  Serial.print(rtc.getTemp() - 2.75);
  Serial.print("C | Bat: ");
  //Serial.print(volt);
  Serial.println("V");

}