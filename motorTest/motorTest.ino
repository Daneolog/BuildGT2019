int spd = 3;
int leg1 = 6;
int leg2 = 7;

void setup() {
  pinMode(spd, OUTPUT);
  pinMode(leg1, OUTPUT);
  pinMode(leg2, OUTPUT);
  digitalWrite(leg1, LOW);
  digitalWrite(leg2, HIGH);
}

void loop() {
  analogWrite(spd, 255);
  delay(500);
  analogWrite(spd, 0);
  delay(500);
}
