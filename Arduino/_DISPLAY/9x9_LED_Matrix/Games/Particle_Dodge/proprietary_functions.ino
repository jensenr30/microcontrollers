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


void yourguy()
{
  int x = (analogRead(0) * 8) / 1000;
  
  if(x == 0)
  {
    dispdata[8][0] = 0;
    dispdata[8][1] = 1;
  }
  else
  {
    dispdata[8][0] = 1;
    dispdata[8][1] = 0;
  }
  
  for(int i = 1; i < x; i++)
    dispdata[8][0] = dispdata[8][0] << 1;
  
  dispdata[8][0] |= partdata80;
  dispdata[8][1] |= partdata81;
}




void particles()
{
   // these two statements move the particles from row 7 down to row 8.
  dispdata[8][0] = dispdata[7][0] | dispdata[8][0];
  dispdata[8][1] = dispdata[7][1] | dispdata[8][1];
  
  //this handles storing the particle data in the bottom row of LEDs. this is necessary because of the way i designed this animation procedure.
  partdata80 = dispdata[7][0];
  partdata81 = dispdata[7][1];
  
  //this shifts the particles in rows 0-6 down one LED on the matrix
  for(int column = 7; column > 0; column--)
  {
    dispdata[column][0] = dispdata[column - 1][0];
    dispdata[column][1] = dispdata[column - 1][1];
  }
  
  //this generates new particles for row 0
  dispdata[0][0] = random(0, 255) & random(0, 255) & random(0, 255);
  dispdata[0][1] = random(0, 1) & random(0, 1) & random(0, 1);
}




/*
void blankdispdata()
{
  dispdata[0][0] = 0;
  dispdata[0][1] = 0;
  dispdata[1][0] = 0;
  dispdata[1][1] = 0;
  dispdata[2][0] = 0;
  dispdata[2][1] = 0;
  dispdata[3][0] = 0;
  dispdata[3][1] = 0;
  dispdata[4][0] = 0;
  dispdata[4][1] = 0;
  dispdata[5][0] = 0;
  dispdata[5][1] = 0;
  dispdata[6][0] = 0;
  dispdata[6][1] = 0;
  dispdata[7][0] = 0;
  dispdata[7][1] = 0;
  dispdata[8][0] = 0;
  dispdata[8][1] = 0;
}
*/
