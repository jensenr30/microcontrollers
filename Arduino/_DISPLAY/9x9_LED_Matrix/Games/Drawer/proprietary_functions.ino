///////////////////////////////
//                           //
//   PROPRIETARY FUNCTIONS   //
//                           //
///////////////////////////////



// this is the function that displays a number of frames. for example, disp(5); will display 5 frames of whateever is in dispdata[9][2]
void disp(int frames) {
  for(int frame_count = 0; frame_count < frames; frame_count++) {
    // reset 4017 decade counter
    digitalWrite(reset_4017, 1);
    digitalWrite(reset_4017, 0);
    
    digitalWrite(enable, 0); // turn off 
    
    // this displays each row of data once
    for(row = 0; row < 9; row++) {
      
      shiftOut(data_serial, clock_serial, MSBFIRST, ((int) dispdata[row][0])); // send data to shift register to display the 8 left-most LEDs
      digitalWrite(data_serial, dispdata[row][1]); // 
      digitalWrite(enable, 1);
      
      delayMicroseconds(1440); // this is how long each row is displayed
      
      digitalWrite(enable, 0); // turn off display
      
      digitalWrite(clock_4017, 0); // move to next row
      digitalWrite(clock_4017, 1);
      
    }
  }
}

void blankdispdata()
{
  dispdata[0][0] = dispdata[0][1] = dispdata[1][0] = dispdata[1][1] = dispdata[2][0] = dispdata[2][1] = dispdata[3][0] = dispdata[3][1] = dispdata[4][0] = dispdata[4][1] = dispdata[5][0] = dispdata[5][1] = dispdata[6][0] = dispdata[6][1] = dispdata[7][0] = dispdata[7][1] = dispdata[8][0] = dispdata[8][1] = 0;
}


// this function makes a cursor appear and will only modify the LED (pixel) that the cursor is in
void put_cursor(void)
{
  static int blink_rate;
  if(cycles < 10) blink_rate = 0;
  else if((cycles % 10 == 0) && cycles != 0)
  {
    blink_rate ^= 1;
  }
  
  if(LEDm9)
  {
    int binary_pixel_value = blink_rate << LEDm9 - 1;
    
    dispdata[LEDd9][0] ^= binary_pixel_value;
  }
  else
  {
    dispdata[LEDd9][1] = blink_rate;
  }
  cycles++;
}


void toggle_LED()
{
  if(LEDm9)
  {
    int binary_pixel_value = 1 << LEDm9 - 1;
    dispdata[LEDd9][0] ^= binary_pixel_value;
    picdata [LEDd9][0] ^= binary_pixel_value;
  }
  else
  {
    dispdata[LEDd9][1] ^= 1;
    picdata [LEDd9][1] ^= 1;
  }
}


void setdisptopic()
{
  int i;
  for(i = 0; i < 8; i ++)
  {
    dispdata[i][0] = picdata[i][0];
    dispdata[i][1] = picdata[i][1];
  }
}
