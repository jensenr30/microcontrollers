///////////////////////////////
//                           //
//   PROPRIETARY FUNCTIONS   //
//                           //
///////////////////////////////



// this is the function that displays a number of frames. for example, disp(5); will display 5 frames of whateever is in dispdata[9][2]
void disp(int frames) {
  word dispdata2[9];
  for(int frame_count = 0; frame_count < frames; frame_count++) {
    // reset 4017 decade counter
    digitalWrite(reset_4017, 1);
    digitalWrite(reset_4017, 0);
    
    digitalWrite(enable, 0); // turn off 
    
    // this displays each row of data once
    for(row = 0; row < 9; row++) {
      
      shiftOut(data_serial, clock_serial, MSBFIRST, ((int) dispdata[row] >> 1)); // send data to shift register to display the 8 left-most LEDs
      dispdata2[row] = dispdata[row] % 2;
      digitalWrite(data_serial, dispdata2[row]); // 
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
  dispdata[0] = dispdata[1] = dispdata[2] = dispdata[3] = dispdata[4] = dispdata[5] = dispdata[6] = dispdata[7] = dispdata[8] = 0;
}
