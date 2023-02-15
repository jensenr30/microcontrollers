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
