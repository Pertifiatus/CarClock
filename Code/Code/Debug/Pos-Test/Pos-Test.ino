/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>


Servo TempServo;
Servo MinServo;
Servo StdServo;
Servo FuelServo;
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  TempServo.attach(2);  // attaches the servo on pin 9 to the servo object
  MinServo.attach(4);
  StdServo.attach(3);
  FuelServo.attach(5);
  Serial.begin(9600);
}

void loop() {
  
  StdServo.write(163);
  MinServo.write(175);
  Serial.println("Start");
delay(3000);

  StdServo.write(14);
  MinServo.write(15);
  Serial.println("Ending");
delay(3000);

}
