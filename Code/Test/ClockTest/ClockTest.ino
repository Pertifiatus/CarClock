#include <DS3231.h>

// SDA an Pin 6, SCL an Pin 7
DS3231  rtc(6, 7);

Time t;

void setup() {
  Serial.begin(9600);
  rtc.begin();
  
  t = rtc.getTime();

  if (t.year < 2025) {
    Serial.println("Zeit ungueltig! Setze neue Zeit...");
    
    // Setzt die Uhrzeit auf den Zeitpunkt, an dem der Code erstellt wurde
    // Format: rtc.setTime(Stunde, Minute, Sekunde);
    // Format: rtc.setDate(Tag, Monat, Jahr);
    // Format: rtc.setDOW(Wochentag);
    
    // HINWEIS: Hier musst du beim ersten Mal die Werte manuell eintragen:
    rtc.setDOW(SUNDAY);      
    rtc.setTime(0, 12, 30);    
    rtc.setDate(21, 12, 2025);
    Serial.println("Zeit wurde neu kalibriert.");
    } else {
    Serial.println("Zeit ist aktuell, keine Korrektur noetig.");
  }
}

void loop() {
  // Zeit und Datum wie gewohnt
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");
  Serial.print(rtc.getTimeStr());
  
  // Temperatur auslesen und anzeigen
  Serial.print(" -- Temp: ");
  Serial.print(rtc.getTemp()-1.5); // Gibt die Temperatur als float zurück
  Serial.println(" C");
  
  delay(1000);
}