#define pin_clock 9
#define pin_reset 8
#define pin_pot_tempo 0
#define pin_pot_swing 1

#define debug 1

void setup(){
  Serial.begin(9600);
 pinMode(pin_clock, OUTPUT);
 pinMode(pin_reset, OUTPUT);
 
 digitalWrite(pin_clock, 1);
 digitalWrite(pin_reset, 0);
}

//period of the clock cycle by two (in milliseconds)
int period, period1, period2;
int swing;
void loop(){
  
  period = analogRead(pin_pot_tempo);
  swing =     analogRead(pin_pot_swing);
  
  period1 = ((swing+1)/(float)1024)*period;
  period2 = period - period1;
  
  #if(debug)
    Serial.print("period = ");
    Serial.println(period);
    Serial.print("period1 = ");
    Serial.println(period1);
    Serial.print("period2 = ");
    Serial.println(period2);
    Serial.println("");//newline
    Serial.print("swing = ");
    Serial.println(swing);
    Serial.println("");//newline
  #endif
  
  digitalWrite(pin_clock, 1);
  delay(period1/2);
  digitalWrite(pin_clock, 0);
  delay(period1/2);
  
  digitalWrite(pin_clock, 1);
  delay(period2/2);
  digitalWrite(pin_clock, 0);
  delay(period2/2);
  
}
