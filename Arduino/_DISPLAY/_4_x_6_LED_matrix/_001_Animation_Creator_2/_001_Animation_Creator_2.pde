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
int rowvar = 1;
int frame = 1;
unsigned long time = 0;
int cy = 1;


void setup(){
  pinMode(C1, OUTPUT);
  pinMode(C2, OUTPUT);
  pinMode(C3, OUTPUT);
  pinMode(C4, OUTPUT);
  pinMode(C5, OUTPUT);
  pinMode(C6, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
}

void loop(){
  
  
  /************************
  Sample Frame:
  
  if(frame == 1){
  rb4 = B000000;
  rb3 = B000000;
  rb2 = B000000;
  rb1 = B000000;
  time=350;
  }
  
  *************************/
  
  if(frame == 1){
  rb4 = B111111;
  rb3 = B111111;
  rb2 = B111111;
  rb1 = B111111;
  time=1000;
  }
  
  
  
  
  
  
  
  
  datadisp();
}
