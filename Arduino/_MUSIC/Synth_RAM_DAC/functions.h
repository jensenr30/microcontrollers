/*
something really weird is going on with this function.
I've been working on just this function for hours and I don't understand why it isn't working.
When I try to use a for loop to implement this fuction like so:

for(i=0; i<waveSteps; i++)
  RAM_write(i,someData);

it will write everything correctly EXCEPT the second word of memory.
i.e. [0] is written, [1] is not written, [2] is written, [3] is written, (and the rest are written fine as well.

These minor glitches can be fixed by simply writing the parts that aren't being written properly a second time.

I don't know why this is happening, so you need to really be watching this function.
Becareful. I think this function is satan.
*/
void RAM_write(int address, byte data){
  int i,j;
  byte d = data; // manipuable storage for data.
  
  
  //disable RAM
  digitalWrite(OutEn, 1);   // not displaying data
  digitalWrite(WriteEn, 1); // not writing
  
  //reset address
  digitalWrite(Reset, 0);
  digitalWrite(Reset, 1);
  digitalWrite(Reset, 0);
  //move to the right address
  for(i=0; i<address; i++){
    digitalWrite(Clock, 0);
    digitalWrite(Clock, 1);
  }
  if(debug)Serial.print(i);
  if(debug)Serial.print(":\t");
  
  //write data to Mem0-Mem7
  for(i=7,j=128;  i>=0;  i--,j/=2){
    if(d >= j){
      digitalWrite(Mem[i], HIGH);
      d %= j;
      if(debug)Serial.print("#");
    }
    else{
      digitalWrite(Mem[i], LOW);
      if(debug)Serial.print("-");
    }
  }
  if(debug)Serial.println(""); //newline
  
  //write data into SRAM
  digitalWrite(WriteEn, 0);
  digitalWrite(WriteEn, 1);
}

//calculated sin at a certain point in the period of a sin wave and multiplies it to get it into the right number range.
double jensin(int currentStep, int totalSteps, int minimum, int resolution){
  double pi = 3.141596;
  double multiplier = (resolution/2)-1;
  double currentPhaseAngle = currentStep/((double)totalSteps);
  
  return multiplier * (1+sin(2*pi*currentPhaseAngle));
}



void compute_and_write_waveform_to_SRAM(){
  int s;
  byte waveform[8] = {1,2,4,8,16,32,64,128};
  
  //for(s=waveSteps; s>=0 ; s--){
  //RAM_write(0,0);
  for(s=0; s<waveSteps; s++){
    RAM_write( s, jensin(s, waveSteps, 0, 256) ); //   sin( 2pi * s/wavesteps )
  }
  RAM_write( 0, jensin(0, waveSteps, 0, 256) ); //   I have to redo element 0 and 1 of the data. I don't know why. but that is the only way it works.
  RAM_write( 1, jensin(1, waveSteps, 0, 256) );
  
  
  /*
  byte mydata = 240;
  
  RAM_write(0,mydata);
  RAM_write(1,mydata);
  RAM_write(2,mydata);
  RAM_write(3,mydata);
  RAM_write(4,mydata);
  RAM_write(5,mydata);
  RAM_write(6,mydata);
  RAM_write(7,mydata);
  RAM_write(8,mydata);
  RAM_write(9,mydata);
  RAM_write(10,mydata);
  RAM_write(11,mydata);
  RAM_write(12,mydata);
  RAM_write(13,mydata);
  RAM_write(14,mydata);
  RAM_write(15,mydata);
  RAM_write(16,mydata);
  RAM_write(17,mydata);
  RAM_write(18,mydata);
  RAM_write(19,mydata);
  RAM_write(20,mydata);
  RAM_write(21,mydata);
  RAM_write(22,mydata);
  RAM_write(23,mydata);
  RAM_write(24,mydata);
  RAM_write(25,mydata);
  RAM_write(26,mydata);
  RAM_write(27,mydata);
  RAM_write(28,mydata);
  RAM_write(29,mydata);
  RAM_write(30,mydata);
  RAM_write(31,mydata);
  */
}





