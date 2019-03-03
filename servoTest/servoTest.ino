#include <Servo.h>

Servo servo;

int angle = 0;

void setup() {
  Serial.begin(9600);
  servo.attach(9);
  servo.write(angle);
}

void loop() {
  servo.write(180);
  delay(500);
  servo.write(40);
  delay(500);
}
