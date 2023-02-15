/* //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
 ===This Program is for use with the 4 x 6 LED matrix version 1.===
 
 
 //=============\
 |=====NOTES=====|
 \\=============/
 
 
 --When using more than one LED at at a time, it is important to
 remember to build your program around P.O.V. (persistance of 
 vision)
 
 ---The surface mount LED on the arduino board itself should only
 turn on when "R4" (digital pin 13) is HIGH.
 
 
 
 
 *///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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







void loop(){      // This is the main section of the program and is where the animating happens

  cy = cy + 1;        // Adds 1 to cycle variable
  cy2 = cy2 + 1;      // Adds 1 to cycle2 variable
  cy3 = cy3 + 1;      // Adds 1 to cycle3 variable

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



  ///////////////////// Start LED pattern

  if (cy2 < 500){          // Start of phase 1
    if (cy2 > 0){
      ro = 1;
      co1 = 1;
    }
  }

  if (cy2 < 1000){          // Start of phase 2
    if (cy2 > 500){
      if (cy == 1){
        ro = 2;
        co1 = 1;
      } 
      else {
        ro = 1;
        co2 = 1;
        cy = 0;
      }
    }
  }

  if (cy2 < 1500){          // Start of phase 3
    if (cy2 > 1000){
      if (cy == 1){
        ro = 3;
        co1 = 1;
      }
      if (cy == 2){
        ro = 2;
        co2 = 1;
      }
      if (cy == 3){
        ro = 1;
        co3 = 1;
        cy = 0;
      }
    }
  }

  if (cy2 < 2000){          // Start of phase 4
    if (cy2 > 1500){
      if (cy == 1){
        ro = 4;
        co1 = 1;
      }
      if (cy == 2){
        ro = 3;
        co2 = 1;
      }
      if (cy == 3){
        ro = 2;
        co3 = 1;
      }
      if (cy == 4){
        ro = 1;
        co4 = 1;
        cy = 0;
      }
    }
  }

  if (cy2 < 2500){          // Start of phase 5
    if (cy2 > 2000){
      if (cy == 1){
        ro = 4;
        co2 = 1;
      }
      if (cy == 2){
        ro = 3;
        co3 = 1;
      }
      if (cy == 3){
        ro = 2;
        co4 = 1;
      }
      if (cy == 4){
        ro = 1;
        co5 = 1;
        cy = 0;
      }
    }
  }

  if (cy2 < 3000){          // Start of phase 6
    if (cy2 > 2500){
      if (cy == 1){
        ro = 4;
        co3 = 1;
      }
      if (cy == 2){
        ro = 3;
        co4 = 1;
      }
      if (cy == 3){
        ro = 2;
        co5 = 1;
      }
      if (cy == 4){
        ro = 1;
        co6 = 1;
        cy = 0;
      }
    }
  }

  if (cy2 < 3500){          // Start of phase 7
    if (cy2 > 3000){
      if (cy == 1){
        ro = 4;
        co4 = 1;
      }
      if (cy == 2){
        ro = 3;
        co5 = 1;
      }
      if (cy == 3){
        ro = 2;
        co6 = 1;
        cy = 0;
      }
      if (cy > 3){
        cy = 0;
      }
    }
  }

  if (cy2 < 4000){          // Start of phase 8
    if (cy2 > 3500){
      if (cy == 1){
        ro = 4;
        co5 = 1;
      }
      if (cy == 2){
        ro = 3;
        co6 = 1;
        cy = 0;
      }
      if (cy > 2){
        cy = 0;
      }
    }
  }

  if (cy2 < 4500){          // Start of phase 9
    if (cy2 > 4000){
      ro = 4;
      co6 = 1;
    }
  }





  if (cy2 > 6000){
    cy2 = 0;
  }
  ///////////////////// End LED pattern



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

  delayMicroseconds(175);           //  The number in the bracket sets the amount of time the arduino
}                     //  will delay untill the next cycle. I have chosen 1 millisecond.
//  this is 1/1000 of a second, and thus we will have our program
//  repeeat at a rate of 1000 times a second (or 1 KiloHertz)







