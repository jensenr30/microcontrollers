// This Program is for use with the 4 x 6 LED matrix version 1. 

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

int cy = 0;
int cy2 = 0;
int cy3 = 0;

int co1 = 0;
int co2 = 0;
int co3 = 0;
int co4 = 0;
int co5 = 0;
int co6 = 0;
int ro = 0;

int accel = 1;

int cc;        //Program specific variable. C.C. stands for Column Chosen

void setup(){                 // Setup
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

  cy = cy + 1;
  cy2 = cy2 + 1;
  cy3 = cy3 + 1;
  
  co1 = 0;
  co2 = 0;
  co3 = 0;
  co4 = 0;
  co5 = 0;
  co6 = 0;
  digitalWrite(C1, LOW);
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C5, LOW);
  digitalWrite(C6, LOW);

  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);



///////////////////// Start LED pattern
if (cy == 100){
  cy = 0;
  if (accel == 1){
  accel = 2;
  } else {
    accel = 1;
  }
}

ro = random(1, 5);
cc = random(1, 7);

if (cc == 1){
  co1 = 1;
}
if (cc == 2){
  co2 = 1;
}
if (cc == 3){
  co3 = 1;
}
if (cc == 4){
  co4 = 1;
}
if (cc == 5){
  co5 = 1;
}
if (cc == 6){
  co6 = 1;
}

///////////////////// End LED pattern



  if (co1 == 1){
    digitalWrite(C1, HIGH);
  }
  if (co2 == 1){
    digitalWrite(C2, HIGH);
  }
  if (co3 == 1){
    digitalWrite(C3, HIGH);
  }
  if (co4 == 1){
    digitalWrite(C4, HIGH);
  }
  if (co5 == 1){
    digitalWrite(C5, HIGH);
  }
  if (co6 == 1){
    digitalWrite(C6, HIGH);
  }




  if (ro == 1){
    digitalWrite(R1, HIGH);
  }
  if (ro == 2){
    digitalWrite(R2, HIGH);
  }
  if (ro == 3){
    digitalWrite(R3, HIGH);
  }
  if (ro == 4){
    digitalWrite(R4, HIGH);
  }
if (accel == 1){
  delay(100 - cy + 5);
}
if (accel == 2){
  delay(5 + cy);
}
}





