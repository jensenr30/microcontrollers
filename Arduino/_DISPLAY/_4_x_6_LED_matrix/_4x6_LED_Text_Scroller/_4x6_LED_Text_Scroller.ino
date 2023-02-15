/* Welcome! Welcome to the 4x6 LED Matrix Text Scroller!

as of now this program :
doesn't actually scroll text. just displays letters
can display characteres on the Matrix (no scrolling as of April 28th, 2012)
can display a checkerboard when and invalid/uncataloged character is attempted to be displayed on the screen. also, when the program tries to display an invalid/uncataloged character
will print to the serial monitor 
both _4x6_LED_Text_Scroller.ino and functions.h were completely written by Ryan Jensen

Code description last updated April 28th, 2012 12:07 AM
*/









/*
These definitions take care of naming the correct pins.
It should be noted that these pins are called "c1" or "r3" as in "column 1" or "row 3".
Also, the names are for a 4x6 LED mattrix that is 6 tall and 4 wide (6 rows, 4 columns)
The definitions start at 0 (i.e. c0 and r0) because that is the most logical way to work the arrays (because arrays also also start at 0).
 */
#define r0 9
#define r1 8
#define r2 7
#define r3 6
#define r4 5
#define r5 4
#define c0 10
#define c1 11
#define c2 12
#define c3 13

// TEMPdata is used to store the data from the letters easily. TEMPdata will get converted and copied into the disp array when the program is ready to display the character.
byte TEMPdata[6] = 
{
  B0000, // row 0
  B0000, // row 1
  B0000, // row 2
  B0000, // row 3
  B0000, // row 4
  B0000  // row 5
};

/* disp is the array that the printdisp() function uses.
printdisp can work faster when working with single bits
(that way printdisp doesn't have to preform calculations like reading bits from specific locations in a variable */
byte disp[6][4] = 
{
  {0,0,0,0}, // row 0
  {0,0,0,0}, // row 1
  {0,0,0,0}, // row 2
  {0,0,0,0}, // row 3
  {0,0,0,0}, // row 4
  {0,0,0,0}  // row 5
};

byte phase_angle = 0;
int letter = 0;
byte width; // this variable stores a number that records the width of a given character on the 4x6 LED matrix
byte r[6] = {r0, r1, r2, r3, r4, r5}; // this will be used by the printdisp() function for writing data to the row pins
byte c[4] = {c0, c1, c2, c3}; // this will also be used by the printdisp() function for controlling which columns are on.

#include "functions.h"

void setup()
{
  Serial.begin(9600); // start serial communication (for debugging)
  // make all the pins outputs
  Serial.println("Starting Program");
  pinMode(r0, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
  pinMode(c0, OUTPUT);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  
  // Trun off ALL THE THINGS!
  digitalWrite(r0, LOW);
  digitalWrite(r1, LOW);
  digitalWrite(r2, LOW);
  digitalWrite(r3, LOW);
  digitalWrite(r4, LOW);
  digitalWrite(r5, LOW);
  digitalWrite(c0, LOW);
  digitalWrite(c1, LOW);
  digitalWrite(c2, LOW);
  digitalWrite(c3, LOW);
} // end of void setup()

void loop()
{
  /* eventually, the string of text in the text array will be scrolled by the textScroller() function. that is the goal. */
  /* as of now, the first character in the text array will be utilized in the textScroller() function. */
  #define string_length 256
  if(letter >= string_length) letter = 0; // reset the string if you are at the end of it.
  char text[string_length] = "abwxyz"; // do not allow your string to exceed 256 characters (including the implied null character at the end of the string.)
  /* text is a string as well as an array of characters.
  Use only lower case letters in the text array.
  it has 160 elements. you can make it bigger if need be. */
  
  textScroller(text, 80);
  /* send textScroller the pointer to the beginning of the text array, also send it a speed of 90 (reasonably fast scrolling) */
  
} // end of void loop()

