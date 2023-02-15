#define c40 2
#define r40 3
#define c17 4
#define r17 5
#define en 6



void setup(){
  pinMode(c40, OUTPUT);
  pinMode(r40, OUTPUT);
  pinMode(c17, OUTPUT);
  pinMode(r17, OUTPUT);
  pinMode(en, OUTPUT);
  
  digitalWrite(c40, HIGH);
  
  digitalWrite(r40, LOW);
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  digitalWrite(c17, LOW);
  
  digitalWrite(r17, LOW);
  digitalWrite(r17, HIGH);
  digitalWrite(r17, LOW);
  
  digitalWrite(en, LOW);
}


void loop(){
  
  reset17();
  
  for(int x=0;x<256;x++){   // counts 4040 256 times lighting up the 9th LED in the column
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
  
  digitalWrite(en, HIGH);  //Enables display.
  delay(5);  //waits 5 ms
  digitalWrite(en, LOW);  //Disables display.
  
  reset40();  //Resets 4040
  digitalWrite(c17, LOW);  //Counts 4017 once
  digitalWrite(c17, LOW);
  
  
  for(int x=0;x<58;x++){
  digitalWrite(c40, HIGH); //Counts once
  digitalWrite(c40, LOW);
  }
  
  digitalWrite(en, HIGH);  //Enables display.
  delay(5);
  digitalWrite(en, LOW);  //Disables display.
  
  
  
}
