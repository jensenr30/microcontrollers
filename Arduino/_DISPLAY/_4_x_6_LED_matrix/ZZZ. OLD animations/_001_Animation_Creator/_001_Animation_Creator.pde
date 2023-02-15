/* //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 ===This Program is for use with the 4 x 6 LED matrix version 1.===
 
 
 /=============\
 |=====NOTES=====|
 \=============/
 
 
 --When using more than one LED at at a time, it is important to
 remember to build your program around P.O.V. (persistance of 
 vision)
 
 ---The surface mount LED on the arduino board itself should only
 turn on when "R4" (digital pin 13) is HIGH.
 
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// This is the START of the program

#define C6 4    // This block, for example, defines names such as "C1" as 9. so now, the following are equal:
#define C5 5    //                                                                                    digitalWrite(9, HIGH);
#define C4 6    //                                                                                    digitalWrite(C1, HIGH);
#define C3 7    // This is useful to me. I don't need to know what pin it is, and I would rather know what row or column it is
#define C2 8    // connected to it, so it is nice to be able to essentially rename pin 9 by setting the contant "C1" as 9.
#define C1 9
#define R1 10
#define R2 11
#define R3 12
#define R4 13

int cy = 0;   // this block creates and sets values of variables used in this program. NOTE: cy, cy2, and cy3 are for your use.
int cy2 = 0;  // (they start at zero and one is added to them upon each cycle.)
int cy3 = 0;
int co1 = 0;
int co2 = 0;
int co3 = 0;
int co4 = 0;
int co5 = 0;
int co6 = 0;
int ro = 0;
int let = 1;

void setup(){                 // Setup
  pinmode();      // Goes to the pinmode void in the "pinModes" tab.
}
void loop(){      // This is the main section of the program where animation happens
  begloop();      // Goes to the begloop void in the "Repeated Codes" tab.   NOTE: Letters use the "cy" variable




  //============
  //=========================
  //=================== Start LED pattern
  //============
 
  //LED animation here.

  //============
  //=================== End LED pattern
  //=========================
  //============





  LEDcontrol();      // Goes to the LEDcontrol void in the "Repeated Codes" tab.
}  

//This is the end of the loop.
//The voids down below are put down there to keep them out of sight because they are executed
//in the loop. They take up a lot of space and clog up the visual appearance of the main program.





//============//   Everything beyond this point was put here to clean-up the visual appearance of the main program



void pinmode(){
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


void begloop(){
  cy++;        // This block keeps track of the cycles that my program goes through. Letters use this variable.
  cy2++;    // through. This is essential to have when using Persistance Of Vision
  cy3++;

  co1 = 0;      // This block resets all of the "LED column" variables
  co2 = 0;
  co3 = 0;
  co4 = 0;
  co5 = 0;
  co6 = 0;

  ro = 0;      // This resets the "LED row" variable

  digitalWrite(C1, LOW);      // This block turns off all of the LED column pins (pins 4-9)
  digitalWrite(C2, LOW);
  digitalWrite(C3, LOW);
  digitalWrite(C4, LOW);
  digitalWrite(C5, LOW);
  digitalWrite(C6, LOW);

  digitalWrite(R1, LOW);      // This block turns off all of the LED row pins (pins 10-13)
  digitalWrite(R2, LOW);
  digitalWrite(R3, LOW);
  digitalWrite(R4, LOW);
}





void LEDcontrol(){
  if (co1 == 1){                  //  This block tests the "LED column" variables. If the variable = 1, the
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




  if (ro == 1){                  //  This block tests the "row LED" variable. If variable = 1, turn row 1 on.
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

  delay(1);           //  The number in the bracket sets the amount of time the arduino
  //  will delay untill the next cycle. I have chosen 1 millisecond.
  //  this is 1/1000 of a second, and thus we will have our program
  //  repeeat at a rate of 1000 times a second (or 1 KiloHertz)
}


