//this grabs values from a very high-frequency oscillating circuit.

#define debug false
#define sample_input 9

void setup(){
  pinMode(sample_input, INPUT);
  Serial.begin(9600);
}


void loop(){
  Serial.println(digitalRead(sample_input));
}
