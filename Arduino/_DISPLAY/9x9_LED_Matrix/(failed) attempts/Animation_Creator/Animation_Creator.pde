/*

 ***Triggers***
 4040 clock: Falling Edge
 4040 reset: Rising Edge
 4017 clock: Rising Edge
 4017 reset: Rising Edge
 
 ***SAMPLE DATA***
  r1=000000000;
  r2=000000000;
  r3=000000000;
  r4=000000000;
  r5=000000000;
  r6=000000000;
  r7=000000000;
  r8=000000000;
  r9=000000000;
 
 
 
 
 
 */
#define c4040 2     // Going from a HIGH state to a LOW state clocks the 4040
#define r4040 3     // Going from a LOW state to a HIGH state resets the 4040
#define c4017 4     // Going from a LOW state to a HIGH state clocks the 4017
#define r4017 5     // Going from a LOW state to a HIGH state resets the 4017
#define DE    6     // DE stands for display enable. A HIGH state allows the LEDs to reach ground. This doesn't effect the 4017 or the 4040's data output.

int cy;
int r;
int x;
int y;
int z;
unsigned long r1=0;
unsigned long r2=0;
unsigned long r3=0;
unsigned long r4=0;
unsigned long r5=0;
unsigned long r6=0;
unsigned long r7=0;
unsigned long r8=0;
unsigned long r9=0;

void setup(){
  pinMode(DE, LOW);
  pinMode(c4040,OUTPUT);
  pinMode(r4040,OUTPUT);
  pinMode(c4017,OUTPUT);
  pinMode(r4017,OUTPUT);
  digitalWrite(c4040, LOW);
  digitalWrite(c4017, LOW);
  off();
  RB();
  
  for(x=0;x<2;x++){
  C4017();
  }
}


void loop(){

  cy++;
  
  r1=100000000;
  r2=000000000;
  r3=000000000;
  r4=000001000;
  r5=000000000;
  r6=000000000;
  r7=000000000;
  r8=000000000;
  r9=000000000;
  DTD();
  
  
}

