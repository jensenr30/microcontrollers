void columnsOff(void);
void blank_TEMPdata_array(void);
void blank_disp_array(void);
void printdisp(void);
void characters(char mychar);
void characters_into_disp(char *text);
void valid_text_speed(byte text_speed);



/* ///////////////////  textScroller documentation  ///////////////////////////////////////////////////////
char *text is a pointer to the beginning of a string.
int text_speed is what controls the speed of the text scrolling. valid input values are 0 thru 1000. 0 = slowest, 500 = medium, 1000 = fast as hell
*/
void textScroller(char *text, byte text_speed)
{
  
  valid_text_speed(text_speed); // make sure the value entered is valid. If it isn't, tell the user (through serial monitor) that it isn't.
  /* if text_speed initally equals a speed of 0, that means there will be 100 cycles of printdisp.
  If text_speed initally equals 90, there will be only 10 cycles of the printdisp() fucntion   */
  text_speed = 101 - text_speed;
  
  characters_into_disp(text); // loads character data into disp (uses TEMPdisp)
  
  columnsOff(); // turns all of the columns off (c1 is set to LOW, c2 is set to LOW, ...)
  
  // prints the data in disp[] for a certain number of frames (as determined by text_speed)
  for(int i = 0; i < text_speed; i++)
    printdisp();
  
} // end of textScroller()






/* ////////////// printdisp documentation //////////////////////
this function prints whatever is in the disp[] array to the LED matrix
it 
*/
void printdisp(void)
{
  // this single for loop is the entirety of printdisp
  for(int column = 0; column < 4; column++)
  {
    // this nested for loop turns on the correct row LEDs
    for(int row = 0; row < 6; row++)
      digitalWrite(r[row], disp[row][column]);
    digitalWrite(c[column], HIGH); // turn on the column-th led column
    delayMicroseconds(2300); // wait (allow the LEDs to be on/off for a little bit)
    digitalWrite(c[column], LOW); // turn off the column-th led column
  }
} // end of printdisp




void characters_into_disp(char *text)
{
  void blank_TEMPdata_array(void);
  blank_disp_array(); // clear the disp array. lets start out with a clean slate
  characters(text[letter]); // load the correct character into TEMPdata
  int first_letter_width = width; // save the first letters pixel width
  
  // adjust the position of the letter is TEMPdata to reflect the phase_angle
  for(int j = phase_angle; j > 0; j--)
  {
    for(int i = 0; i < 6; i++)
      TEMPdata[i] *= 2;
  } // end of for loop. TEMPdata is now positioned as it should be.
  
  // loads TEMPdata (1st character) into disp
  for(byte j = 0; j < 6; j++)
    {
      for(byte power = 1, column = 0; column < first_letter_width; power *= 2, column++)
        disp[j][column] = (TEMPdata[j]/power)%2;
  } // end for loop
  
  int columns_to_disp = phase_angle - first_letter_width + 3; /* this calculation makes columns_to_disp a 1 if the arduino can display 1 column of pixels from the second character. */
  
  
  // this if conditional will determine whether or not to display any pixels of the second character
  if(columns_to_disp > 0)
  {
    void blank_TEMPdata_array(void);
    characters(text[letter + 1]); // load the second character into TEMPdata
    int columns_to_shift = 4 - columns_to_disp; // determine the number of columns that the arduinio will need to shift the second letter over
    
    // this for loop shifts the second character to the right the appropriate amount
    for(int j = columns_to_shift; j > 0; j--)
    {
      for(int i = 0; i < 6; i++)
        TEMPdata[i] /= 2;
    } // end of for loop. TEMPdata is now positioned as it should be.
    
    // loads the appropriate amount of TEMPdata into disp (second character)
    for(byte row = 0; row < 6; row++)
    {
      for(byte power = 1, column = 0; column < columns_to_disp + 1; power *= 2, column++)
      {
        disp[row][column] = (TEMPdata[row]/power)%2;
      } // end of nested for loop
    } // end of for loop. now the second letter (from TEMPdata) was loaded into part of disp
    
  } // end if the if(columns_to_display > 0) statement
    
  
  phase_angle++; // increment phase_angle
  if(phase_angle == first_letter_width + 1)
  {
    phase_angle = 0; // reset phase_angle for the next letter
    letter++; //increment letter; move on to the next letter in the string.
  } // end if
  
} // end TEMPdata_into_disp()


/* //////////////// valid_text_speed() documentation //////////////////////
this function will determine whether or not the speed value sent to the textScroller() function was valid (between 0 and 100)

*/
void valid_text_speed(byte text_speed)
{
  if(text_speed < 0 || text_speed > 100)
  {
    Serial.print("Invalid Speed! text_speed = "); // tell the user that there was an error with the value of text_speed
    Serial.println(text_speed); // tell the user what text speed was
    while(1)
    {
      Serial.println("Please restart your Arduino");
      delay(2000);
    } // end of the infinite while loop (oxymoron, lol)
  } // end of the if statement
} // end of the valid_text_speed function







/* ///////////////// characters() documentation ////////////////////////////
this function inputs the character you want to display and then puts that data into the TEMPdata array.
this function only will accept lower case letters and (hopfully in the near future) numberes/periods/commas/exclamation points/other common characters
reference (for copy and pasting):
      TEMPdata[5] = B0000 ;
      TEMPdata[4] = B0000 ;
      TEMPdata[3] = B0000 ;
      TEMPdata[2] = B0000 ;
      TEMPdata[1] = B0000 ;
      TEMPdata[0] = B0000 ;


*/
void characters(char mychar)
{
  switch(mychar)
  {
    case 'a':
      TEMPdata[5] = B0110; // row 5
      TEMPdata[4] = B1001; // row 4
      TEMPdata[3] = B1111; // row 3
      TEMPdata[2] = B1001; // row 2
      TEMPdata[1] = B1001; // row 1
      TEMPdata[0] = B1001; // row 0
      width = 4;           // the width tells the program (when determining the 
      break;
    case 'b':
      TEMPdata[5] = B1110 ;
      TEMPdata[4] = B1001 ;
      TEMPdata[3] = B1010 ;
      TEMPdata[2] = B1001 ;
      TEMPdata[1] = B1001 ;
      TEMPdata[0] = B1110 ;
      width = 4;
      break;
    case 'w':
      TEMPdata[5] = B10001 ;
      TEMPdata[4] = B10001 ;
      TEMPdata[3] = B10101 ;
      TEMPdata[2] = B10101 ;
      TEMPdata[1] = B01010 ;
      TEMPdata[0] = B01010 ;
      width = 4;
      break;
    case 'x':
      TEMPdata[5] = B1010 ;
      TEMPdata[4] = B1010 ;
      TEMPdata[3] = B0100 ;
      TEMPdata[2] = B0100 ;
      TEMPdata[1] = B1010 ;
      TEMPdata[0] = B1010 ;
      width = 3;
      break;
    case 'y':
      TEMPdata[5] = B1010 ;
      TEMPdata[4] = B1010 ;
      TEMPdata[3] = B0100 ;
      TEMPdata[2] = B0100 ;
      TEMPdata[1] = B0100 ;
      TEMPdata[0] = B0100 ;
      width = 3;
      break;
    case 'z':
      TEMPdata[5] = B1111 ;
      TEMPdata[4] = B0001 ;
      TEMPdata[3] = B0010 ;
      TEMPdata[2] = B0100 ;
      TEMPdata[1] = B1000 ;
      TEMPdata[0] = B1111 ;
      width = 4;
      break;
    default:
      TEMPdata[5] = B1111 ;
      TEMPdata[4] = B1111 ;
      TEMPdata[3] = B1111 ;
      TEMPdata[2] = B1111 ;
      TEMPdata[1] = B1111 ;
      TEMPdata[0] = B1111 ;
      width = 4;
      break;
  } // end of switch statement
  
    
} // end characters() function




// this function clears the TEMPdisp array (makes each element zero)
void blank_TEMPdata_array(void)
{
  
  for(int row = 0; row < 6; row++)
    TEMPdata[row] = B0;
} // end of blank_data


// this function clears the disp array (makes each element zero)
void blank_disp_array(void)
{
  
  for(int j = 0; j < 6; j++)
  {
    for(int i = 0; i < 4; i++)
      disp[j][i] = 0;
  }
} // end of blank_data


// this function turns off all columns
void columnsOff(void) 
{
  digitalWrite(c0, LOW);
  digitalWrite(c1, LOW);
  digitalWrite(c2, LOW);
  digitalWrite(c3, LOW);
} // end of columnsOff


