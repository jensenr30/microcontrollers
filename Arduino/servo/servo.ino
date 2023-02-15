#define servo 3
#define pot 0
float x;

void setup(){
  pinMode(servo, OUTPUT);
}

void loop(){
  x = 1.8554687 * ((float)analogRead(pot));
  digitalWrite(servo, 0);
  delay(20);
  digitalWrite(servo, 1);
  delayMicroseconds(500 + x);
}
