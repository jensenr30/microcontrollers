/* 
 
 This Program is for use with the 4 x 6 LED matrix version 1.
 
 NOTES:
 --When using more than one LED at at a time, it is important to
 remember to build your program around P.O.V. (persistance of 
 vision)
 
 */


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

int pov = 0;


void setup(){                 // Setup

    pinMode(C1, OUTPUT);      //  This block defines digital pins 4-13 as outputs.
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







void loop(){      // This is the main section of the program and is where we will be creating the animations

  cy = cy + 1;        // Adds 1 to cycle count variable
  cy2 = cy2 + 1;      // Adds 1 to cycle count2 variable
  cy3 = cy3 + 1;      // Adds 1 to cycle count3 variable

  co1 = 0;      // This block resets all of the "LED column" variables
  co2 = 0;
  co3 = 0;
  co4 = 0;
  co5 = 0;
  co6 = 0;

  ro = 0;      // This resets the "LED row" variable

  pov = pov + 1;      // Adds one to the "pov" variable

  digitalWrite(C1, LOW);      // This block turns off all of the LED columns
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C5, LOW);
  digitalWrite(C6, LOW);

  digitalWrite(R1, LOW);      // This block turns off all of the LED rows
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);



  ///////////////////// Start LED pattern
  Begin:
  
  digitalWrite(R2, LOW);
  digitalWrite(R1, HIGH);
  
  digitalWrite(C6, LOW);
  digitalWrite(C1, HIGH);
  delay(60);
  digitalWrite(C1, LOW);
  digitalWrite(C2, HIGH);
  delay(60);
  digitalWrite(C2, LOW);
  digitalWrite(C3, HIGH);
  delay(60);
  digitalWrite(C3, LOW);
  digitalWrite(C4, HIGH);
  delay(60);
  digitalWrite(C4, LOW);
  digitalWrite(C5, HIGH);
  delay(60);
  digitalWrite(C5, LOW);
  digitalWrite(C6, HIGH);
  delay(60);
  
  
  digitalWrite(R1, LOW);
  digitalWrite(R2, HIGH);
  
  
  delay(60);
  digitalWrite(C6, LOW);
  digitalWrite(C5, HIGH);
  delay(60);
  digitalWrite(C5, LOW);
  digitalWrite(C4, HIGH);
  delay(60);
  digitalWrite(C4, LOW);
  digitalWrite(C3, HIGH);
  delay(60);
  digitalWrite(C3, LOW);
  digitalWrite(C2, HIGH);
  delay(60);
  digitalWrite(C2, LOW);
  digitalWrite(C1, HIGH);
  delay(60);
  
  
  digitalWrite(R2, LOW);
  digitalWrite(R3, HIGH);
  
  
  digitalWrite(C6, LOW);
  digitalWrite(C1, HIGH);
  delay(60);
  digitalWrite(C1, LOW);
  digitalWrite(C2, HIGH);
  delay(60);
  digitalWrite(C2, LOW);
  digitalWrite(C3, HIGH);
  delay(60);
  digitalWrite(C3, LOW);
  digitalWrite(C4, HIGH);
  delay(60);
  digitalWrite(C4, LOW);
  digitalWrite(C5, HIGH);
  delay(60);
  digitalWrite(C5, LOW);
  digitalWrite(C6, HIGH);
  delay(60);
  
  
  digitalWrite(R3, LOW);
  digitalWrite(R4, HIGH);
  
  
  delay(60);
  digitalWrite(C6, LOW);
  digitalWrite(C5, HIGH);
  delay(60);
  digitalWrite(C5, LOW);
  digitalWrite(C4, HIGH);
  delay(60);
  digitalWrite(C4, LOW);
  digitalWrite(C3, HIGH);
  delay(60);
  digitalWrite(C3, LOW);
  digitalWrite(C2, HIGH);
  delay(60);
  digitalWrite(C2, LOW);
  digitalWrite(C1, HIGH);
  delay(60);
  digitalWrite(R4, LOW);
  digitalWrite(R3, HIGH);
  delay(60);
  digitalWrite(R3, LOW);
  digitalWrite(R2, HIGH);
  delay(60);
  goto Begin;
  
  
  
  /*
  if (pov = 1){
    ro = 2;
    co1 = 1;
  }
  if (pov = 2){
    ro = 1;
    co2 = 1;
    pov = 0;
  }
*/


  ///////////////////// End LED pattern



  if (co1 == 1){                  //  These test the "LED column" variables. If the variable = 1, the
    digitalWrite(C1, HIGH);       //  corresponding column is turned on
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




  if (ro == 1){                  //  This block test the "row LED" variable. If variable = 1, turn row 1 on.
    digitalWrite(R1, HIGH);      //  If variable = 2, turn row 2 on... (etc.)
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

  delay(5);           //  The number in the bracket sets the amount of time the arduino
}                     //  will delay untill the next cycle. I have chosen 5 milliseconds.
//  this is 1/200 of a second, and thus we will have our program
//  repeeat at a rate of 200 times a second.







