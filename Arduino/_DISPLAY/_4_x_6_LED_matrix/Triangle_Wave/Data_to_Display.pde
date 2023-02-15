void datadisp(){
  rowvar++;
  cy++;
  
  
  if(rowvar == 1){
    digitalWrite(R2,LOW);
    digitalWrite(R3,LOW);
    digitalWrite(R4,LOW);
    if(rb1 >= B100000){digitalWrite(C1, HIGH);rb1 = rb1-B100000;}else{digitalWrite(C1,LOW);}
    if(rb1 >= B10000){digitalWrite(C2, HIGH);rb1 = rb1-B10000;}else{digitalWrite(C2,LOW);}
    if(rb1 >= B1000){digitalWrite(C3, HIGH);rb1 = rb1-B1000;}else{digitalWrite(C3,LOW);}
    if(rb1 >= B100){digitalWrite(C4, HIGH);rb1 = rb1-B100;}else{digitalWrite(C4,LOW);}
    if(rb1 >= B10){digitalWrite(C5, HIGH);rb1 = rb1-B10;}else{digitalWrite(C5,LOW);}
    if(rb1 >= B1){digitalWrite(C6, HIGH);rb1 = rb1-B1;}else{digitalWrite(C6,LOW);}
    digitalWrite(R1,HIGH);
  }
  
  if(rowvar == 2){
    digitalWrite(R1,LOW);
    digitalWrite(R3,LOW);
    digitalWrite(R4,LOW);
    if(rb2 >= B100000){digitalWrite(C1, HIGH);rb2 = rb2-B100000;}else{digitalWrite(C1,LOW);}
    if(rb2 >= B10000){digitalWrite(C2, HIGH);rb2 = rb2-B10000;}else{digitalWrite(C2,LOW);}
    if(rb2 >= B1000){digitalWrite(C3, HIGH);rb2 = rb2-B1000;}else{digitalWrite(C3,LOW);}
    if(rb2 >= B100){digitalWrite(C4, HIGH);rb2 = rb2-B100;}else{digitalWrite(C4,LOW);}
    if(rb2 >= B10){digitalWrite(C5, HIGH);rb2 = rb2-B10;}else{digitalWrite(C5,LOW);}
    if(rb2 >= B1){digitalWrite(C6, HIGH);rb2 = rb2-B1;}else{digitalWrite(C6,LOW);}
    digitalWrite(R2,HIGH);
  }
  
  if(rowvar == 3){
    digitalWrite(R1,LOW);
    digitalWrite(R2,LOW);
    digitalWrite(R4,LOW);
    if(rb3 >= B100000){digitalWrite(C1, HIGH);rb3 = rb3-B100000;}else{digitalWrite(C1,LOW);}
    if(rb3 >= B10000){digitalWrite(C2, HIGH);rb3 = rb3-B10000;}else{digitalWrite(C2,LOW);}
    if(rb3 >= B1000){digitalWrite(C3, HIGH);rb3 = rb3-B1000;}else{digitalWrite(C3,LOW);}
    if(rb3 >= B100){digitalWrite(C4, HIGH);rb3 = rb3-B100;}else{digitalWrite(C4,LOW);}
    if(rb3 >= B10){digitalWrite(C5, HIGH);rb3 = rb3-B10;}else{digitalWrite(C5,LOW);}
    if(rb3 >= B1){digitalWrite(C6, HIGH);rb3 = rb3-B1;}else{digitalWrite(C6,LOW);}
    digitalWrite(R3,HIGH);
  }
  
  if(rowvar == 4){
    digitalWrite(R1,LOW);
    digitalWrite(R2,LOW);
    digitalWrite(R3,LOW);
    if(rb4 >= B100000){digitalWrite(C1, HIGH);rb4 = rb4-B100000;}else{digitalWrite(C1,LOW);}
    if(rb4 >= B10000){digitalWrite(C2, HIGH);rb4 = rb4-B10000;}else{digitalWrite(C2,LOW);}
    if(rb4 >= B1000){digitalWrite(C3, HIGH);rb4 = rb4-B1000;}else{digitalWrite(C3,LOW);}
    if(rb4 >= B100){digitalWrite(C4, HIGH);rb4 = rb4-B100;}else{digitalWrite(C4,LOW);}
    if(rb4 >= B10){digitalWrite(C5, HIGH);rb4 = rb4-B10;}else{digitalWrite(C5,LOW);}
    if(rb4 >= B1){digitalWrite(C6, HIGH);rb4 = rb4-B1;}else{digitalWrite(C6,LOW);}
    digitalWrite(R4,HIGH);
  }
  
  delay(1);
  
  if(rowvar == 4){
    rowvar=0;
  }else{
    datadisp();
  }
  
  if(cy >= time){
    frame++;
    cy=0;
  }
  
}
