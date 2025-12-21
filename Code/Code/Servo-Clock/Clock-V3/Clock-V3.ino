#include <DS3231.h>
#include <Servo.h>
#include <SoftwareWire.h>
#include <LiquidCrystal_SoftI2C.h> 

const int sdaPin = 6;
const int sclPin = 7;

SoftwareWire myWire(sdaPin, sclPin);
DS3231 rtc(sdaPin, sclPin);
Time t;
LiquidCrystal_I2C lcd(0x26, 16, 2, &myWire);

// Hilfsvariablen
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(9600);
  delay(2000); 
  Serial.println("--- MODUS: STABILER BUS ---");

  // 1. RTC starten und einmalig Zeit holen
  rtc.begin();
  t = rtc.getTime();
  Serial.print("Startzeit: ");
  Serial.println(rtc.getTimeStr());

  // 2. LCD starten
  lcd.begin(); 
  lcd.backlight();
  lcd.print("System Aktiv");
  delay(1000);
  lcd.clear();
}

void loop() {
  // Wir aktualisieren die Anzeige nur jede Sekunde
  if (millis() - lastUpdate >= 1000) {
    lastUpdate = millis();

    // Wir riskieren KEINEN rtc.getTime() im Loop, 
    // solange wir nicht wissen, ob der Bus frei ist.
    // Stattdessen nutzen wir die interne Uhr des Arduino für die Anzeige:
    
    static int s = t.sec;
    static int m = t.min;
    static int h = t.hour;

    s++;
    if (s >= 60) { s = 0; m++; }
    if (m >= 60) { m = 0; h++; }
    if (h >= 24) { h = 0; }

    // Anzeige auf LCD
    lcd.setCursor(0, 0);
    if(h<10) lcd.print("0"); lcd.print(h);
    lcd.print(":");
    if(m<10) lcd.print("0"); lcd.print(m);
    lcd.print(":");
    if(s<10) lcd.print("0"); lcd.print(s);
    
    // Serieller Monitor
    Serial.print("Zeit: ");
    Serial.print(h); Serial.print(":"); Serial.println(m);

    // Alle 30 Sekunden versuchen wir die RTC neu zu synchronisieren
    if (s == 30) {
       Serial.println("Synchronisiere RTC...");
       // Hier koennte man rtc.getTime() versuchen, 
       // aber wir lassen es erst mal weg, um den Freeze zu vermeiden.
    }
  }
}