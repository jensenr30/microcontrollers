/*
Ryan Jensen
9x9_Matrix_v3.ino
February 24th 2012

DESCRIPTION:

This program should (and i emphasize SHOULD) be a building block
for all of the future programs that will use the 9 x 9 LED matrix.

The current version of teh LED matrix is as follows:
  *  A 4017 decade counter controls which row is connected to ground
  *  A 74ls164 serial to parallel shift register controls which 
     columns are connected to power (through 220 ohm resistors
  *  There is one transistor which controls if the LED columns can
     reach ground. If this transistor has a 0 signal at its base
     no current will be able to flow from the LEDs to ground. I call
     it the "enable" transistor and define "enable" as the pin
     connected to the base of said transistor.
  *  NOTE: the 4017 and the shift register both have rising-edge-
     triggered clocks. The 4017 also has an active-1 reset.
*/


// clock of the serial to parallel 74_164 chip
#define clock_serial 2
// serial output to the serial to parallel 74_164 chip
#define data_serial 3

// the clock of the row controlling 4017 decade counter
#define clock_4017 4
// the reset of the 4017 decade counter
#define reset_4017 5

// pin responsible for enabling and disabling the display
#define enable 6

// ALL THE VARIABLE DECLARATIONS!! ////////////////////////////////
int dispdata[9][2] = {
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};
int gen, gen2; // whore variables for for() and while() loops.
word row, extra_time;
////////////////////////////////////////////////////////////////////




void setup() {
  
  // PIN_MODE STUFF ////////////////
  pinMode(enable, OUTPUT);
  
  pinMode(clock_serial, OUTPUT);
  pinMode(data_serial, OUTPUT);
  
  pinMode(clock_4017, OUTPUT);
  pinMode(reset_4017, OUTPUT);
  ///////////////////////////////////
  
  // INITIAL STATE OF MATRIX: ////////////////////////////////////////////////////////
  digitalWrite(enable, 0); // disable LED display
  
  digitalWrite(reset_4017, 0); // reset 4017 decade counter
  digitalWrite(reset_4017, 1);
  digitalWrite(reset_4017, 0);
  
  digitalWrite(data_serial, 0); // give the shift register a 0 at its serial input
  for(gen = 0; gen < 8; gen++) {    // clock the shift register 8 times
    digitalWrite(clock_serial, 0);
    digitalWrite(clock_serial, 1);
  }
  digitalWrite(clock_serial, 0);
}

void loop() {
  
  
  dispdata = {
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(20);
  
  dispdata = {
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 },
  { B00000000,0 }
};

  disp(5);
  
  dispdata = {
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 },
  { B00000000,0 }
};
  
  dispdata = {
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 },
  { B00000000,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 },
  { B00000001,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 },
  { B00000011,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 },
  { B00000111,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 },
  { B00001111,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 },
  { B00011111,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 },
  { B00111111,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B01111111,1 }
};
  
  disp(5);
  
  dispdata = {
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 },
  { B11111111,1 }
};
  
  disp(60);
  
  

  
  
  
  
}

///////////////////////////////
//                           //
//   PROPRIETARY FUNCTIONS   //
//                           //
///////////////////////////////

void disp(int frames) {
  for(int frame_count = 0; frame_count < frames; frame_count++) {
    // reset 4017 decade counter
    digitalWrite(reset_4017, 1);
    digitalWrite(reset_4017, 0);
    
    digitalWrite(enable, 0); // turn off 
    
    // this displays each row of data once
    for(row = 0; row < 9; row++) {
      
      shiftOut(data_serial, clock_serial, MSBFIRST, ((int) dispdata[row][0]));
      digitalWrite(data_serial, dispdata[row][1]);
      digitalWrite(enable, 1);
      delayMicroseconds(1340); // this is how long each row is displayed
        delayMicroseconds(4);
      
      digitalWrite(enable, 0); // turn off display
      
      digitalWrite(clock_4017, 0);
      digitalWrite(clock_4017, 1);
      
    }
  }
}
