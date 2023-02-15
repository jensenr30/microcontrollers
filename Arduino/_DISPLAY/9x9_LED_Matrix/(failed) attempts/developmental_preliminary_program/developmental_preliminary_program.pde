#define c40 2
#define r40 3
#define c17 4
#define r17 5
#define en 6

int x;


void setup(){
  pinMode(2, OUTPUT);  //sets all outputs as outputs. (redundant, I know.)
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  
  digitalWrite(en, LOW);  // disables display

  digitalWrite(c40, HIGH);  // 4040 clock inital value = high
  
  digitalWrite(c17, LOW);  // 4017 clock inital value = low

  digitalWrite(r40, LOW);  // resets 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  digitalWrite(r17, LOW);  // resets 4017
  digitalWrite(r17, HIGH);
  digitalWrite(r40, LOW);
  
}

void loop(){
  
  digitalWrite(r40, LOW);  // resets 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  digitalWrite(r17, LOW);  // resets 4017
  digitalWrite(r17, HIGH);
  digitalWrite(r17, LOW);
  
  for(x=0; x<7; x++){  // 4040 counts up to 7
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
  
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);
  
  digitalWrite(c17, LOW);  // clocks 4017 once
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<6; x++){  // makes 4040 count to 6
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);
 
  digitalWrite(c17, LOW);  // clocks 4017 once
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<4; x++){  // makes 4040 count to 4
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);
 
  digitalWrite(c17, LOW);  // clocks 4017 once  ************************************************************************************************************
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<12; x++){  // makes 4040 count to 12
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);  // *********************************************************************************************************************************
 
  digitalWrite(c17, LOW);  // clocks 4017 once  ************************************************************************************************************
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<28 + 32 + 64; x++){  // makes 4040 count to 28 + 32 + 64
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
 digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
 delay(1);
 digitalWrite(en, LOW);  // *********************************************************************************************************************************
 
  digitalWrite(c17, LOW);  // clocks 4017 once  ************************************************************************************************************
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<12; x++){  // makes 4040 count to 12
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);  // *********************************************************************************************************************************
  
  digitalWrite(c17, LOW);  // clocks 4017 once  ************************************************************************************************************
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<4; x++){  // makes 4040 count to 4
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);  // *********************************************************************************************************************************
  
  digitalWrite(c17, LOW);  // clocks 4017 once  ************************************************************************************************************
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<6; x++){  // makes 4040 count to 6
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);  // *********************************************************************************************************************************
  
  digitalWrite(c17, LOW);  // clocks 4017 once  ************************************************************************************************************
  digitalWrite(c17, HIGH);
  
  digitalWrite(r40, LOW);  // reset 4040
  digitalWrite(r40, HIGH);
  digitalWrite(r40, LOW);
  
  for(x=0; x<7; x++){  // makes 4040 count to 7
  digitalWrite(c40, HIGH);
  digitalWrite(c40, LOW);
  }
 
  digitalWrite(en, HIGH);  // enable display, wait a millisecond, then disable display
  delay(1);
  digitalWrite(en, LOW);  // *********************************************************************************************************************************
  
}
