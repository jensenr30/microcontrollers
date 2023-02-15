/*
Ryan Jensen
9x9_Matrix_v3.ino
February 24th 2012

Drawer
*////////////////////////////////////////////////////////////////////////////////////////////////






// PINS USED: ///////////////////////////////////////////////////////////////////////////////////
#define speaker_pin 9    // a transducer is connected to ground with a 100 ohm resistor

#define clock_serial 2   // clock of the serial to parallel 74_164 chip
#define data_serial 3    // serial output to the serial to parallel 74_164 chip

#define clock_4017 4     // the clock of the row controlling 4017 decade counter
#define reset_4017 5     // the reset of the 4017 decade counter

#define enable 6         // pin responsible for enabling and disabling the display

#define pot_pin 0        // potentiometer is a variable voltage divider at analog pin 0
#define button 0         // button is digital pin 0 ACTIVE LOW!

#define not_pressed 1
#define pressed 0
/////////////////////////////////////////////////////////////////////////////////////////////////
// ALL THE VARIABLE DECLARATIONS!! ////////////////////////////////
int dispdata[9][2];
int picdata[9][2];
// whore variables for for() and while() loops:
int gen, gen2;
// variables for the disp() function:
word row;
// animation/game specific variables:
int POT;
float LED;
int LEDm9, LEDd9; // used for calculating LED % 9 and LED / 9 respectively
int button_state[2] = {not_pressed , not_pressed};
int cycles = 0;

// functions
void blankdispdata();
void disp(int);





void setup() {
  Serial.begin(9600);
  
  // PIN_MODE STUFF ////////////////
  pinMode(enable, OUTPUT);
  
  pinMode(clock_serial, OUTPUT);
  pinMode(data_serial, OUTPUT);
  
  pinMode(clock_4017, OUTPUT);
  pinMode(reset_4017, OUTPUT);
  
  pinMode(button, INPUT);
  digitalWrite(button, 1); // activate pull-up resistors
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
} // end setup function


void loop()
{
  int LEDh[2] = {0, 0};
  int button_perm;
  
  POT = 1023 - analogRead(pot_pin);
  LED = (float)POT * (80.00 / 1023.00);
  
  LEDd9 = LED / 9;
  LEDm9 = 8 - (int)LED % 9;
  LEDh[1] = LEDh[0];
  LEDh[0] = LED;
  if(LEDh[0] !=LEDh[1]) cycles = 0;
  
  setdisptopic();
  
  button_state[1] = button_state[0];
  button_state[0] = digitalRead(button);
  if(button_state[0] == pressed && button_state[1] == not_pressed)
    toggle_LED();
  
  put_cursor();
  
  disp(1);
  cycles++;
}



