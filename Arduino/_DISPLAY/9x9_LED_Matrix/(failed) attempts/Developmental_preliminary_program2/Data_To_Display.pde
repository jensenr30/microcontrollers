


void d2d(){
  
  cd = 0;  // set cd to 0
  
  if(r1 >= 100000000){
    cd = cd + 256;
    r1 = r1 - 100000000;
  }
  
  if(r1 >= 10000000){
    cd = cd + 128;
    r1 = r1 - 10000000;
  }
  
  if(r1 >= 1000000){
    cd = cd + 64;
    r1 = r1 - 1000000;
  }
  
  if(r1 >= 100000){
    cd = cd + 32;
    r1 = r1 - 100000;
  }
  
  if(r1 >= 10000){
    cd = cd + 16;
    r1 = r1 - 10000;
  }
  
  if(r1 >= 1000){
    cd = cd + 8;
    r1 = r1 - 1000;
  }
  
  if(r1 >= 100){
    cd = cd + 4;
    r1 = r1 - 100;
  }
  
  if(r1 >= 10){
    cd = cd + 2;
    r1 = r1 - 10;
  }
  
  if(r1 == 1){  // If r1 is 1, add 1 to cd. If it isn't either 1 or 0, goto the error void.
    cd = cd + 1;
    r1 = r1 - 1;
  }
  
  
    
  digitalWrite(r40, LOW);
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  digitalWrite(r40, LOW);
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0;x<cd;x++){           // this is what clocks the 4040 for r1
    digitalWrite(c40, HIGH);
    digitalWrite(c40, LOW);
  }
  
  digitalWrite(en, HIGH);
  digitalWrite(c40, HIGH);
}





void error_in_r1(){       // Void for the "error in r1" that is supposed to show itself when there is a mistake in the r1 value.
  digitalWrite(r17, LOW);
  digitalWrite(r17, HIGH);
  digitalWrite(r17, LOW);
  
  digitalWrite(r40, LOW);
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0;x<8;x++){
  digitalWrite(c17, LOW);
  digitalWrite(c17, HIGH);
  }
  
  for(x=0;x<273;x++){
    digitalWrite(c40, HIGH);
    digitalWrite(c40, LOW);
  }
  
  for(1;1;1){
  digitalWrite(en, HIGH);
  delay(375);
  digitalWrite(en, LOW);
  delay(375);
  }
}
