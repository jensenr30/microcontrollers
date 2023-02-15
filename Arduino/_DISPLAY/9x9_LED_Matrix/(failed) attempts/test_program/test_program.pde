#define c4040 2
#define r4040 3
#define c4017 4
#define r4017 5
#define trans 6

void setup(){
  pinMode(c4040,OUTPUT);
  pinMode(r4040,OUTPUT);
  pinMode(c4017,OUTPUT);
  pinMode(r4017,OUTPUT);
  pinMode(trans,OUTPUT);
  //Sets Clocks for both the 4040 and the 4017 LOW
  digitalWrite(c4040,LOW);
  digitalWrite(c4017,LOW);
  //Resets the count of both the 4040 and the 4017
  digitalWrite(r4040,HIGH);
  digitalWrite(r4040,LOW);
  digitalWrite(r4017,HIGH);
  digitalWrite(r4017,LOW);
}


void loop(){
  
  
  
  //clocks the 4040 once
  digitalWrite(c4040,HIGH);
  digitalWrite(c4040,LOW);
  //clocks the 4017 once
  digitalWrite(c4017,HIGH);
  digitalWrite(c4017,LOW);
  
  delay(500);
}
