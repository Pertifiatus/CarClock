#include <DS3231.h>
#include <Servo.h>

// RTC an Pin 6 (SDA) und 7 (SCL)
DS3231 rtc(6, 7);
Time t;

// Servo Objekte
Servo TempServo;
Servo MinServo;
Servo StdServo;
Servo FuelServo;

// Pins für die Servos
const int pinTemp = 2;
const int pinMin  = 4;
const int pinStd  = 3;
const int pinFuel = 5;
const int pinBat  = A0; // Analoger Pin für Batteriespannung

// Speicher für alte Werte (um nur bei Änderung zu bewegen)
int oldMin  = -1;
int oldStd  = -1;
int oldTemp = -1;
int oldFuel = -1;

void setup() {
  Serial.begin(9600);
  rtc.begin();
  
  // Initialprüfung der Zeit
  t = rtc.getTime();
  if (t.year < 2025) {
    rtc.setDOW(SUNDAY);      
    rtc.setTime(0, 31, 0);    
    rtc.setDate(21, 12, 2025);
  }
}

// Hilfsfunktion zum Bewegen und Abschalten
void moveAndDetach(Servo &s, int pin, int angle) {
  s.attach(pin);
  s.write(angle);
  delay(600); // Zeit zum Erreichen der Position
  s.detach();
}

void loop() {
  t = rtc.getTime();
  float currentTemp = rtc.getTemp() - 3.25;
  
  // 1. STUNDEN (Skala 0-12h -> 0-180 Grad)
  int h = t.hour;
  if (h >= 12) h -= 12;
  if (h != oldStd) {
    int angle = map(h, 0, 12, 0, 180);
    moveAndDetach(StdServo, pinStd, angle);
    oldStd = h;
  }

  // 2. MINUTEN (Skala 0-60min -> 0-180 Grad)
  if (t.min != oldMin) {
    int angle = map(t.min, 0, 60, 180, 25);
    moveAndDetach(MinServo, pinMin, angle);
    oldMin = t.min;
  }

  // 3. TEMPERATUR (Skala 15°C - 35°C -> 0-180 Grad)
  int tempInt = (int)currentTemp;
  if (tempInt != oldTemp) {
    int angle = map(tempInt, 15, 35, 0, 180);
    angle = constrain(angle, 0, 180);
    moveAndDetach(TempServo, pinTemp, angle);
    oldTemp = tempInt;
  }

  // 4. FUEL (Batteriespannung 2.5V - 3.2V -> 0-180 Grad)
  int rawBat = analogRead(pinBat);
  float voltage = rawBat * (5.0 / 1023.0);
  int fuelPercent = map(voltage * 100, 250, 320, 0, 180); // In Millivolt für map()
  fuelPercent = constrain(fuelPercent, 0, 180);
  
  if (abs(fuelPercent - oldFuel) > 2) { // Nur bei merklicher Änderung (Rauschen verhindern)
    moveAndDetach(FuelServo, pinFuel, fuelPercent);
    oldFuel = fuelPercent;
  }

  // Serieller Monitor zur Kontrolle
  Serial.print(rtc.getTimeStr());
  Serial.print(" | Temp: ");
  Serial.print(currentTemp);
  Serial.print("C | Bat: ");
  Serial.print(voltage);
  Serial.println("V");

  delay(2000); 
}