#define c40 2
#define r40 3
#define c17 4
#define r17 5
#define en 6

int x;

int cd;  // variable "column data" or "cd"

long r1;  // initializes the row variables 1-9
long r2;
long r3;
long r4;
long r5;
long r6;
long r7;
long r8;
long r9;


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
  
  r1 = 000001111;
  
  d2d();
  
  delay(60000);
  
}
