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


#define speaker_pin 9    // a transducer is connected to ground with a 100 ohm resistor

#define clock_serial 2   // clock of the serial to parallel 74_164 chip
#define data_serial 3    // serial output to the serial to parallel 74_164 chip

#define clock_4017 4     // the clock of the row controlling 4017 decade counter
#define reset_4017 5     // the reset of the 4017 decade counter

#define enable 6         // pin responsible for enabling and disabling the display

#define pot_pin 0        // potentiometer is a variable voltage divider at analog pin 0
#define button 0         // button is ACTIVE LOW


// ALL THE VARIABLE DECLARATIONS!! ////////////////////////////////

int dispdata[9][2] = {
  { B00000000,0 }, // 0
  { B00000000,0 }, // 1
  { B00000000,0 }, // 2
  { B00000000,0 }, // 3
  { B00000000,0 }, // 4
  { B00000000,0 }, // 5
  { B00000000,0 }, // 6
  { B00000000,0 }, // 7
  { B00000000,0 }  // 8
};

// whore variables for for() and while() loops:
int
gen, //  first degree whore variable
gen2 // second degree whore variable
;

// variables for the disp() function:
word
row
;

// animation/game specific variables:
float
potentiometer = 511
;
int
ship_position = 4,      // ship position is the variable that stores where your ship is (0-8)
shot[2] = { 0,-1 },     // this holds information for the shot that you fired {x, y}
shot_itterations = 0,   // this holds information for how many frames have passed since the shot was last updated
enemies[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1}       // this holds the information for the enemies. if enemies[3] = 2, that means that in column 3 (the fourth column from the right) has an enemy in row 2 (thrid row from the top)
;
////////////////////////////////////////////////////////////////////




void setup() {
  
  Serial.begin(9600); // begin serial communication
  
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
  ////////////////////////////////////////////////////////////////////////////////////
  
  
  // this for loop tells the user (via serial monitor) what the values of the enemies int array is. ///////////////////////////
  for(gen = 0; gen < 9; gen ++) {
    Serial.print("enemies[");
    Serial.print(gen);
    Serial.print("] is ");
    Serial.println(enemies[gen]);
  }
  
  
  
}



void loop() {
  
  
  // this is stuff that has to be done each itteration of the game cycle ///////////////////////////////////////////////////////////////////////
  potentiometer = analogRead(pot_pin); // reads the pot position
  ship_position = potentiometer *= 0.00875; // translates the input from the pot to an x coordinate on the LED matrix
  clear_disp_data(); // clear dispdata
  
  
  // this switch() statement handles generating the ship (the thing the user controls at the bottom of the matrix)
  switch (ship_position) {
    case 0:
      dispdata[7][1] = dispdata[8][1] = 1;
      break;
    default:
      dispdata[7][0] = dispdata[8][0] = 1;
      for(gen = 1; gen < ship_position; gen++)
        dispdata[7][0] = dispdata[8][0] *= 2;
      break;
  }
  
  
  // this detects if the user presses the button. ////////////////////////////////////////////////////////////////////////////////////////////
  if ((!digitalRead(button)) && (shot[1] == -1))
    shot = { ship_position, 6 }; // if butten is pressed (and another shot is NOT in progress) fire a shot; set shot coordinates to the beginning of the shot cycle.
    
  
  // this regulates how fast the shot moves when there is a shot on screen. //////////////////////////////////////////////////////////////////
  if(shot[1] >= 0) {
    if(shot_itterations == 2) {
      shot[1]--; // makes the shot move higher until it reaches -1 (out of bounds)
      shot_itterations = 0;
    }
    shot_itterations++; // increase how many times the shot has sat in one LED (pixel)
  }
  
  
  // this handles making the shot in appear on the matrix only if the shot is at a y value greater than -1. /////////////////////////////////
  if(shot[1] > -1) {
    switch (shot[0]) {
      case 0:
        dispdata[shot[1]][1] = 1;
        break;
      default:
        dispdata[shot[1]][0] = 1; 
        for(gen = 1; gen < shot[0]; gen++)
          dispdata[shot[1]][0] *= 2;
        break;
    }
  }
    
  
  // puts enemies data into dispdata[] /////////////////////////////////////////////////////////////////////////////////////
  // under dev.
   
  
  disp(1); // display one frame of the current display configuration of the dispdata array
  
}
