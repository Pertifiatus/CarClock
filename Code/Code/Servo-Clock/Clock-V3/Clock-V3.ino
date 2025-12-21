#include <DS3231.h>
#include <Servo.h>
#include <LiquidCrystal_SoftI2C.h> 

// Pins für Software I2C (RTC und LCD teilen sich diese)
const int sdaPin = 6;
const int sclPin = 7;

// Initialisierung der RTC (nutzt internes Software-I2C)
DS3231 rtc(sdaPin, sclPin);
Time t;

// Initialisierung des LCD (nutzt die LiquidCrystal_SoftI2C Library)
// Adresse 0x26, 16 Spalten, 2 Zeilen, SDA auf 6, SCL auf 7
LiquidCrystal_SoftI2C lcd(0x26, 16, 2, sdaPin, sclPin);

// Servo Objekte
Servo TempServo;
Servo MinServo;
Servo StdServo;
Servo FuelServo;

// Pins
const int pinTemp = 2;
const int pinMin  = 4;
const int pinStd  = 3;
const int pinFuel = 5;
const int pinBat  = A0;

// Speicher für alte Werte
int oldMin  = -1;
int oldStd  = -1;
int oldTemp = -1;
int oldFuel = -1;

void setup() {
  Serial.begin(9600);
  rtc.begin();
  
  // LCD Setup
  lcd.init(); 
  lcd.backlight();
  
  lcd.print("Willkommen Sibel");
  lcd.setCursor(0, 1);
  lcd.print("      <3        ");
  
  // Serieller Monitor Debug-Ausgabe
  Serial.println("System Start...");
  Serial.println("Willkommen Sibel <3");

  delay(2000);
  lcd.clear();

  t = rtc.getTime();
  if (t.year < 2025) {
    rtc.setDOW(SUNDAY);      
    rtc.setTime(0, 31, 0);    
    rtc.setDate(21, 12, 2025);
  }
}

void moveAndDetach(Servo &s, int pin, int angle) {
  s.attach(pin);
  s.write(angle);
  delay(600);
  s.detach();
}

void loop() {
  t = rtc.getTime();
  float currentTemp = rtc.getTemp() - 2.25;
  
  // --- SERVO LOGIK ---
  int h = t.hour;
  if (h >= 12) h -= 12;
  if (h != oldStd) {
    moveAndDetach(StdServo, pinStd, map(h, 0, 12, 0, 180));
    oldStd = h;
  }

  if (t.min != oldMin) {
    moveAndDetach(MinServo, pinMin, map(t.min, 0, 60, 163, 15));
    oldMin = t.min;
  }

  int tempInt = (int)currentTemp;
  if (tempInt != oldTemp) {
    int angle = constrain(map(tempInt, 15, 35, 0, 180), 0, 180);
    moveAndDetach(TempServo, pinTemp, angle);
    oldTemp = tempInt;
  }

  int rawBat = analogRead(pinBat);
  float voltage = rawBat * (5.0 / 1023.0);
  int fuelAngle = constrain(map(voltage * 100, 200, 320, 0, 180), 0, 180);
  if (abs(fuelAngle - oldFuel) > 2) {
    moveAndDetach(FuelServo, pinFuel, fuelAngle);
    oldFuel = fuelAngle;
  }

  // --- AUSGABE LCD ---
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());
  lcd.print(" ");
  lcd.print(currentTemp, 1);
  lcd.print("C ");

  lcd.setCursor(0, 1);
  lcd.print("Bat: ");
  lcd.print(voltage);
  lcd.print("V   ");

  // --- DEINE ORIGINALEN SERIELLEN AUSGABEN ---
  Serial.print(rtc.getTimeStr());
  Serial.print(" | Temp: ");
  Serial.print(currentTemp);
  Serial.print("C | Bat: ");
  Serial.print(voltage);
  Serial.println("V");

  delay(1000); 
}