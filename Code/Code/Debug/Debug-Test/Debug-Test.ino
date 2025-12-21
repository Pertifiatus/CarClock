#include <Servo.h>

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

// Test-Variablen
int testMin = 0;
int testStd = 0;
float testTemp = 15.0;
float testFuel = 2.5;
float angleStep = 0.0; // Für die Sinus-Kurve

void setup() {
  Serial.begin(9600);
  Serial.println("--- SERVO TEST MODUS GESTARTET ---");
}

// Hilfsfunktion zum Bewegen und Detachen
void moveAndDetach(Servo &s, int pin, int angle) {
  s.attach(pin);
  s.write(angle);
  delay(400); // Etwas schneller für den Test
  s.detach();
}

void loop() {
  // 1. SIMULATION: MINUTEN (schneller Durchlauf)
  int minAngle = map(testMin, 0, 60, 175, 15);
  Serial.print("Minute: "); Serial.print(testMin);
  moveAndDetach(MinServo, pinMin, minAngle);

  // 2. SIMULATION: STUNDEN
  int stdAngle = map(testStd, 0, 12, 163, 14);
  Serial.print(" | Stunde: "); Serial.print(testStd);
  moveAndDetach(StdServo, pinStd, stdAngle);

  // 3. SIMULATION: TEMP & FUEL (Sinus-Welle für flüssige Bewegung)
  // Wir simulieren Werte, die sanft hoch und runter gehen
  float sineWave = (sin(angleStep) + 1.0) / 2.0; // Wert zwischen 0.0 und 1.0
  
  int tempAngle = sineWave * 180;
  int fuelAngle = (1.0 - sineWave) * 180; // Fuel genau entgegengesetzt
  
  moveAndDetach(TempServo, pinTemp, tempAngle);
  moveAndDetach(FuelServo, pinFuel, fuelAngle);

  Serial.print(" | S-Wave: "); Serial.println(sineWave);

  // Werte für den nächsten Durchgang erhöhen
  testMin+=10;
  if (testMin > 60) {
    testMin = 0;
    testStd++;
  }
  if (testStd > 12) {
    testStd = 0;
  }
  
  angleStep += 0.2; // Geschwindigkeit der Sinus-Welle

}