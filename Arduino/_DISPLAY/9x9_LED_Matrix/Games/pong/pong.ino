/*
Ryan Jensen
9x9_Matrix_v3.ino
February 24th 2012

// DESCRIPTION: /////////////////////////////////////////////////////////////////

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
  *  There is a potentiometer hooked up to analog pin 0.
  *  There is a speaker hooked up to digital pin 9
*/////////////////////////////////////////////////////////////////////////////////



#define speaker_pin 9    // the speaker is hooked up to ground in series with a 100 ohm resisitor

#define clock_serial 2   // clock of the serial to parallel 74_164 chip
#define data_serial 3    // serial output to the serial to parallel 74_164 chip

#define clock_4017 4     // the clock of the row controlling 4017 decade counter
#define reset_4017 5     // the reset of the 4017 decade counter

#define enable 6         // pin responsible for enabling and disabling the display

#define pot_pin 0        // analog pin 0 is where the variable voltage source is hooked up to (the variable voltage dividing pot) ranges from 0 to 1023



// ALL THE VARIABLE DECLARATIONS!! ////////////////////////////////
int dispdata[9][2] = { // dispdata is initially set to 0
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
word row; // row used ONLY in disp() function
int paddle, trajectory = 2, coordinate[2] = {4, 0}; // coordinate is in {x, y} format where the origin {0, 0} is one uppermost rightmost LED
int itterations = 0, number_of_paddle_hits = 0, level = 8; // Level 8 is easy (some delay) The lower the level, the harder it gets because of less delay.
// pot is the variable that analogRead dumps in


/* trajectory values and meanings:
  positive maans it's moving up
  negative means it's moving down
1: up and left
2: up
3: up and right

-1: down and right
-2: down
-3: down and left

if you want to change the direction (up or down) simply add or subttract 4
if you want to change the direction (right or left) simply add or subtract 
*/


/*
coordinate[0] means the current position, the x coordinate
coordinate[1] means the current position, the x coordinate
*/

////////////////////////////////////////////////////////////////////




void setup() {
  
  Serial.begin(9600);
  
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
  ////////////////////////////////////////////////////////////////////////////////////
}







void loop() {
  
  if((coordinate[1] == 0) && (number_of_paddle_hits == 10)) levelup(); // FIX THIS!!!!!!!
  
  
  // THIS HAS TO DO WITH THE PADDLE //////////////////////////////////////////
  
  paddle = ((float)analogRead(pot_pin)) * 0.0065;
  
    dispdata[8][0] = dispdata[8][1] = 0; // clears paddle data
    
    if(paddle > 6) paddle = 6; // limit pot to a max value of 6
  
  switch(paddle) {
    case 0:
      dispdata[8][1] = 1; // paddle far right
      dispdata[8][0] = 3;
      break;
    case 1:
      dispdata[8][0] = 7;
      break;
    case 2:
      dispdata[8][0] = 14;
      break;
    case 3:
      dispdata[8][0] = 28;
      break;
    case 4:
      dispdata[8][0] = 56;
      break;
    case 5:
      dispdata[8][0] = 112;
      break;
    case 6:
      dispdata[8][0] = 224;
      break;
    default:
      dispdata[8][0] = 255; // if any error occurs in the pot variable (e.g. gets bigger than 6 or less than 1) it turns them all on
      dispdata[8][1] = 1;
      break;
  }
  ////////////////////////////////////////////////////////////////////////////
  disp(1); // this displays whatever was put in dispdata
  
  itterations++;
  
  blankdispdata();
  
  
  // THIS HAS TO DO WITH THE BALL /////////////////////////////////////
  
  if(itterations >= level) {
    
    if(coordinate[1] == 0   &&   trajectory != -2){
      trajectory -= 4; // if moving up, move down.
    }
    
    // if the ball hits the paddle
    if(coordinate[1] == 7) {
      if((paddle <= coordinate[0]) && (paddle >= coordinate[0] - 2)) { // and if paddle is below ball
        trajectory += 4; // the trajectory must become upwards (2)
        tone(speaker_pin, 8400, 30); // and a tone is played
        trajectory = random(1,4);
        Serial.println(++number_of_paddle_hits); // print hits once it gets a new hit
      } else { // otherwise
        loose(); // you loose.
      }
    }
    
    if((coordinate[0] == 0) && ((trajectory == 3) || (trajectory == -1)))
      trajectory -= 2;
      
    if((coordinate[0] == 8) && ((trajectory == 1) || (trajectory == -3)))
      trajectory += 2;
      
    
    
///////// this handles changing coordinate values to reflect trajectory ///////////////////////////
    switch(trajectory) {
      case 1:           // move up and left
        coordinate[1]--;
        coordinate[0]++;
        break;
      case 2:            // move up
        coordinate[1]--;
        break;
      case 3:           // move up and right
        coordinate[1]--;
        coordinate[0]--;
        break;
      case -3:          // move down and left
        coordinate[1]++;
        coordinate[0]++;
        break;
      case -2:          // move down
        coordinate[1]++;
        break;
      case -1:          // move down and right
        coordinate[1]++;
        coordinate[0]--;
        break;
    }
    
    itterations = 0;
  }
  // end the if(itterations) conditional statement /////////////////////////////////////////////
  
  
  
  
  // this handles changing coordinates to dispdata /////////////////////////////////////////////
  if(!coordinate[0]) { // if coordinate == 0
    dispdata[coordinate[1]][1] = 1; // turn on an LED in the first row in th right column
  } else {
    dispdata[coordinate[1]][0] = 1;
    for(gen = 0; gen < coordinate[0] - 1; gen++)
      dispdata[coordinate[1]][0] *= 2;
  }
    
    
    
    
    
    
    
  /*
  
  dispdata = {
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
  
  disp(1000);
  
  */
  
  
  
  
  
}
