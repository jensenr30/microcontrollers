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
  time=100;
  }
  
  *************************/
  
  if(frame == 33){
    frame = 1;
  }
  
  
  if(frame == 1){
  rb4 = B100000;
  rb3 = B000000;
  rb2 = B000000;
  rb1 = B000000;
  time=100;
  }
  
  if(frame == 2){
  rb4 = B110000;
  rb3 = B100000;
  rb2 = B000000;
  rb1 = B000000;
  time=100;
  }
  
  if(frame == 3){
  rb4 = B111000;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B000000;
  time=100;
  }
  
  if(frame == 4){
  rb4 = B111100;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B100000;
  time=100;
  }
  
  if(frame == 5){
  rb4 = B111110;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B110000;
  time=100;
  }
  
  if(frame == 6){
  rb4 = B111111;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B111000;
  time=100;
  }
  
  if(frame == 7){
  rb4 = B111111;
  rb3 = B100001;
  rb2 = B100000;
  rb1 = B111100;
  time=100;
  }
  
  if(frame == 8){
  rb4 = B111111;
  rb3 = B100001;
  rb2 = B100001;
  rb1 = B111110;
  time=100;
  }
  
  if(frame == 9){
  rb4 = B111111;
  rb3 = B100001;
  rb2 = B100001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 10){
  rb4 = B011111;
  rb3 = B100001;
  rb2 = B100001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 11){
  rb4 = B001111;
  rb3 = B000001;
  rb2 = B100001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 12){
  rb4 = B000111;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 13){
  rb4 = B000011;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B011111;
  time=100;
  }
  
  if(frame == 14){
  rb4 = B000001;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B001111;
  time=100;
  }
  
  if(frame == 15){
  rb4 = B000000;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B000111;
  time=100;
  }
  
  if(frame == 16){
  rb4 = B000000;
  rb3 = B000000;
  rb2 = B000001;
  rb1 = B000011;
  time=100;
  }
  
  if(frame == 17){
  rb4 = B000000;
  rb3 = B000000;
  rb2 = B000000;
  rb1 = B000001;
  time=100;
  }
  
  if(frame == 18){
  rb4 = B000000;
  rb3 = B000000;
  rb2 = B000001;
  rb1 = B000011;
  time=100;
  }
  
  if(frame == 19){
  rb4 = B000000;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B000111;
  time=100;
  }
  
  if(frame == 20){
  rb4 = B000001;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B001111;
  time=100;
  }
  
  if(frame == 21){
  rb4 = B000011;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B011111;
  time=100;
  }
  
  if(frame == 22){
  rb4 = B000111;
  rb3 = B000001;
  rb2 = B000001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 23){
  rb4 = B001111;
  rb3 = B000001;
  rb2 = B100001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 24){
  rb4 = B011111;
  rb3 = B100001;
  rb2 = B100001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 25){
  rb4 = B111111;
  rb3 = B100001;
  rb2 = B100001;
  rb1 = B111111;
  time=100;
  }
  
  if(frame == 26){
  rb4 = B111111;
  rb3 = B100001;
  rb2 = B100001;
  rb1 = B111110;
  time=100;
  }
  
  if(frame == 27){
  rb4 = B111111;
  rb3 = B100001;
  rb2 = B100000;
  rb1 = B111100;
  time=100;
  }
  
  if(frame == 28){
  rb4 = B111111;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B111000;
  time=100;
  }
  
  if(frame == 29){
  rb4 = B111110;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B110000;
  time=100;
  }
  
  if(frame == 30){
  rb4 = B111100;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B100000;
  time=100;
  }
  
  if(frame == 31){
  rb4 = B111000;
  rb3 = B100000;
  rb2 = B100000;
  rb1 = B000000;
  time=100;
  }
  
  if(frame == 32){
  rb4 = B110000;
  rb3 = B100000;
  rb2 = B000000;
  rb1 = B000000;
  time=100;
  }
  
  
  
  
  
  
  
  
  datadisp();
}
