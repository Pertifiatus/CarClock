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

int pos = 0;  // variable to store the servo position

void setup() {
  TempServo.attach(5);  // attaches the servo on pin 9 to the servo object
  MinServo.attach(4);
  StdServo.attach(3);
//  FuelServo.attach(2);
  Serial.begin(9600);

  StdServo.write(163);
  MinServo.write(171);
  FuelServo.write(173);
  TempServo.write(171);
  delay(2000);
}

void loop() {

  //Start Position
  StdServo.write(163);
  MinServo.write(171);
  FuelServo.write(173);
  TempServo.write(171);
  Serial.println("Start");
delay(5000);
/*
  StdServo.write(138);
  MinServo.write(140);
  FuelServo.write(153);
  TempServo.write(145);
  Serial.println("Viertel");
  delay(5000);

  StdServo.write(97);
  MinServo.write(97);
  FuelServo.write(130);
  TempServo.write(130);
  Serial.println("Mid");
delay(5000);

  StdServo.write(57);
  MinServo.write(53);
  FuelServo.write(110);
  TempServo.write(116);
  Serial.println("DreiViertel");
delay(5000);
  //End Position
  StdServo.write(19);
  MinServo.write(11);
  FuelServo.write(83);
  TempServo.write(85);
  Serial.println("Ending");
delay(5000);
*/
}
