/*
Ryan Jensen
Sparkle_Sparkle.ino
April 29th, 2012

// DESCRIPTION OF THIS PROGRAM: /////////////////////////////////////////////////////////////////

*  This program turns on random pixels on the 9x9 LED Matrix. Hence the name, "Sparkle Sparkle".
*  Push the button the turn the clicking on and off
* The potentiometer controls the speed at which the matrix sparkles.

/////////////////////////////////////////////////////////////////////////////////////////////////
*/

// PINS USED: ///////////////////////////////////////////////////////////////////////////////////
#define speaker_pin 9    // a transducer is connected to ground with a 100 ohm resistor

#define clock_serial 2   // clock of the serial to parallel 74_164 chip
#define data_serial 3    // serial output to the serial to parallel 74_164 chip

#define clock_4017 4     // the clock of the row controlling 4017 decade counter
#define reset_4017 5     // the reset of the 4017 decade counter

#define enable 6         // pin responsible for enabling and disabling the display

#define pot_pin 0        // potentiometer is a variable voltage divider at analog pin 0
#define button 0         // button is digital pin 0 ACTIVE LOW!
/////////////////////////////////////////////////////////////////////////////////////////////////


// ALL THE VARIABLE DECLARATIONS!! ////////////////////////////////
int dispdata[9][2] = 
{
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0},
  {B00000000, 0}
};

// whore variables for for() and while() loops:
int gen, gen2;

// variables for the disp() function:
word row;

// animation/game specific variables:
////////////////////////////////////////////////////////////////////

void disp(int);


void setup() {
  
  // PIN_MODE STUFF ////////////////
  pinMode(enable, OUTPUT);
  
  pinMode(clock_serial, OUTPUT);
  pinMode(data_serial, OUTPUT);
  
  pinMode(clock_4017, OUTPUT);
  pinMode(reset_4017, OUTPUT);
  
  pinMode(button, INPUT);
  digitalWrite(button, 1); // activate pull-up resistors
  
  randomSeed(analogRead(0)); // will then shuffle the random function.
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
  
} // end of the setup function







void loop() {
  static int click_on = 0;
  
  if(!digitalRead(button)) // this if statement inputs a 1 or a 0 from the button
  {
    if(click_on) click_on = 0;
    else         click_on = 1;
  }
  
  for(int i = 0; i < 9; i++) // this fills the dispdata array with random numbers (which are going to be written to the matrix as pixels)
  {
    dispdata[i][0] = random(256);
    dispdata[i][1] = random(2);
  }
  
  if(click_on)
    tone(speaker_pin, 8000, 10); // this makes a clicking sound if click_on == 1
  
  disp(analogRead(0)/10 + 1); // display the random information on the Matrix for a period of time determined by the potentiometer
}
