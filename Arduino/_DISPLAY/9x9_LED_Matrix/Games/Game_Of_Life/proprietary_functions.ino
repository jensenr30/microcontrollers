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


//each pixel has a numer/denom chance of being alive on sprouting
void sprout_life(int numer, int denom)
{
  int setPixel, i, j;
  int result;
  
  for(i = 0; i < 9; i++)
  {
    dispdata[i][0] = 0;
    for(j = 0; j < 8; j++)
    {
      result = random(1, denom);
      if(numer > result) setPixel = 1;
      else               setPixel = 0;
      dispdata[i][0] += setPixel; // add setPixel to dispdata
      if(j < 7) dispdata[i][0] <<= 1; // bit shift up 1 bit
    }
    result = random(1, denom);
    if(numer > result) dispdata[i][1] = 1;
    else               dispdata[i][1] = 0;
  }
}


void sim_life( int repititions, int how_long_each_generation_lives)
{
  int i,j;
  int prox;
  byte place;
  
  for(int a = 0; a < repititions; a++)
  {
    
    for(i = 0; i < 9; i++) // row
    {
      place = 1;
      for(j = 0; j < 8; j++) // column
      {
        prox = 0;
        //general situation
        if(j > 0)
        {
          if(dispdata[i][0] & place >> 1) prox++; // if the cell to the right is alive
        }
        if(j < 7)
        {
          if(dispdata[i][0] & place << 1) prox++; // if the cell to the left is alive
        }
        if(i > 0)
        {
          if(dispdata[i-1][0] & place) prox++; // if the cell above is alive
        }
        if(i < 8)
        {
          if(dispdata[i+1][0] & place) prox++; // if the cell below is alive
        }
        
        
        if(prox >= 4 || prox <= 1) dispdata[i][0] &= ~place;
        else                       dispdata[i][0] |=  place;
        place <<= 1;
      }
    }
  
  disp(how_long_each_generation_lives);
  }
}
