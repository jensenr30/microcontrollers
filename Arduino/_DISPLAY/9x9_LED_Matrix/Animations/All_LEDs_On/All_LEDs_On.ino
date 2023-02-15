/*
Ryan Jensen
9x9_Matrix_v3.ino
February 24th 2012

// DESCRIPTION OF THIS PROGRAM: /////////////////////////////////////////////////////////////////

*  This program should (and i emphasize SHOULD) be a building block
   for all of the future programs that will use the 9 x 9 LED matrix.
*  This program was made mainly made to help the programmer with animation
   and being able to use the matrix to display things. Games are fair
   game.
/////////////////////////////////////////////////////////////////////////////////////////////////



// The current version of teh LED matrix is as follows: //////////////////////////////////////////

  *  A 4017 decade counter controls which row is connected to ground
  *  A 74ls164 serial to parallel shift register controls which 
     columns are connected to power (through 220 ohm resistors
  *  There is one transistor which controls if the LED columns can
     reach ground. If this transistor has a 0 signal at its base
     no current will be able to flow from the LEDs to ground. I call
     it the "enable" transistor and define "enable" as the pin
     connected to the base of said transistor.
  *  NOTE: the 4017 and the shift register both have rising-edge-
     triggered clocks. The 4017 also has an active-LOW reset.
  *  data_serial (pin 3) is connceted directly to the column 0 (the right-most column).
     In normal use, this pin feeds information to the shift register and then
     is set to whatever the right-most column should be.
  *  There is a 90 Kohm pot on the board that is acting as a voltage divider.
     The middle pin of the pot (which as the variable voltage)
*////////////////////////////////////////////////////////////////////////////////////////////////


// PINS USED: ///////////////////////////////////////////////////////////////////////////////////
#define speaker_pin 9    // a transducer is connected to ground with a 100 ohm resistor

#define clock_serial 2   // clock of the serial to parallel 74_164 chip
#define data_serial 3    // serial output to the serial to parallel 74_164 chip

#define clock_4017 4     // the clock of the row controlling 4017 decade counter
#define reset_4017 5     // the reset of the 4017 decade counter

#define enable 6         // pin responsible for enabling and disabling the display

#define pot_pin 0        // potentiometer is a variable voltage divider at analog pin 0
/////////////////////////////////////////////////////////////////////////////////////////////////


// ALL THE VARIABLE DECLARATIONS!! ////////////////////////////////
int dispdata[9][2] = {
  { B11111111,1 }, // 0
  { B11111111,1 }, // 1
  { B11111111,1 }, // 2
  { B11111111,1 }, // 3
  { B11111111,1 }, // 4
  { B11111111,1 }, // 5
  { B11111111,1 }, // 6
  { B11111111,1 }, // 7
  { B11111111,1 }  // 8
};

// whore variables for for() and while() loops:
int gen, gen2;

// variables for the disp() function:
word row;

// animation/game specific variables:
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
  digitalWrite(clock_serial, 0);  // clock of the shift register is 0
  ////////////////////////////////////////////////////////////////////////////////////
  
}







void loop() {
  
  
  disp(1000);
  
  
  
}
