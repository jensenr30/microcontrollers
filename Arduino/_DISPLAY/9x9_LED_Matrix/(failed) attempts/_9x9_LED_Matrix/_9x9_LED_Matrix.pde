#define clockpin 4
#define resetpin 2
int x=0;

void setup(){
  pinMode(clockpin, OUTPUT);
  pinMode(resetpin, OUTPUT);
  
  //Resets the binary counter
  digitalWrite(resetpin,HIGH);
  digitalWrite(resetpin,LOW);
}
void loop(){

  //This for statement pulses the clock pin until all of the outputs of the counter are high
  
  for(x=0;x<511;x++){
    digitalWrite(clockpin, HIGH);
    digitalWrite(clockpin, LOW);
  }
  delay(1);
  digitalWrite(resetpin,HIGH);
  digitalWrite(resetpin,LOW);
  delay(8);
}

