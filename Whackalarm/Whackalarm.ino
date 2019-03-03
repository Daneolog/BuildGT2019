/** This code describes the various functions of the Whackalarm based on what phase the alarm is in.
 *  Phase 0 - A person is not detected.
 *  Phase 1 - A person is detected, and is warned to wake up.
 *  Phase 2 - The person is whacked with an arm repeatedly.
 *  Phase 3 - The person has water dumped on their face. (Phase 2 continues.)
 */

#include <Servo.h> // library for controlling servo

#define PResSource A0 // photoresistor pin is analog 0

Servo servo;

int angle = 40; // starting angle for the servo
int spd = 3; // Arduino pin for adjusting speed
int leg1 = 6; // Sets motor spin direction
int leg2 = 7; // Sets motor spin direction
boolean isAwake = true; // if the person is awake or in phase 1
boolean hasHit = false; // if the arm has whacked the person this cycle
boolean dumpOK = false; // if the Whackalarm has entered phase 3
boolean waterDumped = false; // if the water has been dumped on the person

void setup() {
  Serial.begin(9600); // begin Serial comms
  servo.attach(9); // servo attached to digital pin 9
  servo.write(angle); // set the servo to the initial angle
  pinMode(spd, OUTPUT);
  pinMode(leg1, OUTPUT);
  pinMode(leg2, OUTPUT);
  digitalWrite(leg1, LOW); // set the motor to move forward
  digitalWrite(leg2, HIGH); // set the motor to move forward
}

void loop() {
  int amountOfLight = analogRead(PResSource); // read the photoresistor value
  if (Serial.available() > 0) {
    int incomingByte = Serial.read();
    Serial.read();
    if (incomingByte == 50) { // if the Python script sends a "2"
      dumpOK = true;
      isAwake = false;
    }
    if (incomingByte == 49) { // if the Python script sends a "1"
      isAwake = false;
    }
    else if (incomingByte == 48) { // if the Python script sends a "0"
      isAwake = true;
    }
  }
  if (amountOfLight < 200 && !waterDumped && dumpOK) { // dump water if it has not been dumped
    dumpWater();
    Serial.flush(); // get rid of data from during the water dump
  }
  if (amountOfLight < 200 && !isAwake) { // whack the person if we are in phase 2 or 3
    servo.write(angle);
    if (angle > 180 || angle < 40) { // if the arm is going out of bounds, bring it back
      hasHit = !hasHit;
    }
    angleAdjust(hasHit);
  }
  delay(2);
}

//void servoSmack() {
//  while(angle < 180) {
//    servo.write(angle);
//    angle++;
//    delay(2);
//  }
//  while(angle > 0) {
//    servo.write(angle);
//    angle--;
//    delay(2);
//  }
//}
void angleAdjust(boolean hasHit) { // Change the arm direction based on its position in the cycle
  if (!hasHit) {
    angle++;
  }
  else {
    angle--;
  }
}
void dumpWater() { // Dump the water on the person
  analogWrite(spd, 255);
  delay(500);
  analogWrite(spd, 0);
  delay(500);
  // waterDumped = true; // ensures this function only runs once
}
