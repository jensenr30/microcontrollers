#define A 13
#define B 12
#define C 11
#define D 10
#define E 9
#define F 8
#define G 7
#define H 6
#define D1 5
#define D2 4
#define D3 3

unsigned long cy=0;
unsigned long cy2=0;
unsigned long cy3=0;
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int e = 0;
int f = 0;
int g = 0;
int h = 0;
int d1 = 0;
int d2 = 0;
int d3 = 0;
int n = -1;

void setup(){
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(H, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);

}
void loop(){
  cy++;
  cy2++;
  cy3++;
  allLOW();
  // Start animation

  if (cy3==1){
    //enter digit 1 data here
    n=1;
    d1=1;
  }
  if (cy3==2){
    //enter digit 2 data here
    n=2;
    d2=1;
  }
  if (cy3==3){
    n=3;
    //enter digit 3 data here
    d=3;
    cy3=0;
  }

  // End animation
  segmentcontrol();
  delay(1);
}




void allLOW(){
  a=0;
  b=0;
  c=0;
  d=0;
  e=0;
  f=0;
  g=0;
  h=0;
  d1=0;
  d2=0;
  d3=0;
  n= -1;
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);
  digitalWrite(H, LOW);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
}

void segmentcontrol(){
  if (n==0){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
  }
  if (n==1){
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
  }
  if (n==2){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(G, HIGH);
  }
  if (n==3){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(G, HIGH);
  }
  if (n==4){
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  if (n==5){
    digitalWrite(A, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  if (n==6){
    digitalWrite(A, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  if (n==7){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
  }
  if (n==8){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }
  if (n==9){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  }




  if(a==1){
    digitalWrite(A, HIGH);      //This block controls single variables.  if a=1, make A HIGH
  }
  if(b==1){
    digitalWrite(B, HIGH);  
  }
  if(c==1){
    digitalWrite(C, HIGH);  
  }
  if(d==1){
    digitalWrite(D, HIGH);  
  }
  if(e==1){
    digitalWrite(E, HIGH);  
  }
  if(f==1){
    digitalWrite(F, HIGH);  
  }
  if(g==1){
    digitalWrite(G, HIGH);  
  }
  if(h==1){
    digitalWrite(H, HIGH);  
  }
  if(d1==1){
    digitalWrite(D1, HIGH);  
  }
  if(d2==1){
    digitalWrite(D2, HIGH);  
  }
  if(d3==1){
    digitalWrite(D3, HIGH);  
  }
}




