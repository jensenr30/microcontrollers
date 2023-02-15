void setup()
{
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
}

void loop()
{
  int time = 15;
  
  digitalWrite(13, 0);
  digitalWrite(10, 1);
  
  delay(time);
  
  digitalWrite(10, 0);
  digitalWrite(11, 1);
  
  delay(time);
  
  digitalWrite(11, 0);
  digitalWrite(12, 1);
  
  delay(time);
  
  digitalWrite(12, 0);
  digitalWrite(13, 1);
  
  delay(time);
  
}
