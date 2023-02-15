#define C6 4
#define C5 5
#define C4 6
#define C3 7
#define C2 8
#define C1 9
#define R1 10
#define R2 11
#define R3 12
#define R4 13
byte rb1 = B0;
byte rb2 = B0;
byte rb3 = B0;
byte rb4 = B0;
int rowvar = 0;
unsigned long frame = 1;
int time = 0;
int cy = 1;


void setup(){
  pinMode(C1, OUTPUT);pinMode(C2, OUTPUT);pinMode(C3, OUTPUT);pinMode(C4, OUTPUT);pinMode(C5, OUTPUT);pinMode(C6, OUTPUT);pinMode(R1, OUTPUT);pinMode(R2, OUTPUT);pinMode(R3, OUTPUT);pinMode(R4, OUTPUT);
}


void loop(){
  
  /* 
  SAMPLE FRAME:
  
  if(frame == 1){
  rb4 = B000000;
  rb3 = B000000;
  rb2 = B000000;
  rb1 = B000000;
  time=750;
  }
  
  */
  
  
  if(frame == 7){
    frame = 1;
  }
  
  if(frame == 1){
  rb4 = B100000;
  rb3 = B010001;
  rb2 = B001010;
  rb1 = B000100;
  time=150;
  }
  
  if(frame == 2){
  rb4 = B010000;
  rb3 = B101000;
  rb2 = B000101;
  rb1 = B000010;
  time=150;
  }
  
  if(frame == 3){
  rb4 = B001000;
  rb3 = B010100;
  rb2 = B100010;
  rb1 = B000001;
  time=150;
  }
  
  if(frame == 4){
  rb4 = B000100;
  rb3 = B001010;
  rb2 = B010001;
  rb1 = B100000;
  time=150;
  }
  
  if(frame == 5){
  rb4 = B000010;
  rb3 = B000101;
  rb2 = B101000;
  rb1 = B010000;
  time=150;
  }
  
  if(frame == 6){
  rb4 = B000001;
  rb3 = B100010;
  rb2 = B010100;
  rb1 = B001000;
  time=150;
  }
  
  
  
  datadisp();
}
