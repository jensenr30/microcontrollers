// store 8-bit data in the external RAM chip
#define Mem0 2 // least significant bit
#define Mem1 3
#define Mem2 4
#define Mem3 5
#define Mem4 6
#define Mem5 7
#define Mem6 8
#define Mem7 9 // most significant bit
//array that holds the pin numbers for the data
const byte Mem[8] = {Mem0, Mem1, Mem2, Mem3, Mem4, Mem5, Mem6, Mem7};

//display debug messages to serial monitor?
#define debug 0


// active low. Data doesn't output if ReadWrite is active.
//allows the 8-bit RAM to be output to the DAC.
#define OutEn 12

// writes when low (active low)
//allows Mem0-Mem7 to be written to the 8-bit RAM.
#define WriteEn 13

// this is the pin that clocks the counter, advancing through the RAM (rising edge)
//the frequency of this clock determines the pitch of the note played.
#define Clock 10

#define SyncOut 14 // this is what pin the clock output will be. The clock output will be triggered at the beginning of each measure.

// this is the pin that resets the counter, setting all bits to 0. (active high)
#define Reset 11

int waveSteps = 32; // this is the number of steps per wave

#include "Functions.h"

/* 
at this point, I'm going to keep the sample size at 32-steps. so there will be 32 Bytes in memory.
Each byte is an 8-bit value that is converted into an "anolog" signal with 256 discrete values.
(this "diagram" is just for the hell of it)

       xxxxx                               xxxxx                               xxxxx                        
    xxx     xxx                         xxx     xxx                         xxx     xxx                    
  xx           xx                     xx           xx                     xx           xx                  
 x               x                   x               x                   x               x                 
x                 x                 x                 x                 x                 x                 x
                   x               x                   x               x                   x               x
                    xx           xx                     xx           xx                     xx           xx 
                      xxx     xxx                         xxx     xxx                         xxx     xxx   
                         xxxxx                               xxxxx                               xxxxx      
*/



// plays the waveform in the ram.
void non(double freq){
  tone( Clock, 2*freq*((double)waveSteps) );
  if(debug)Serial.print( freq );
  if(debug)Serial.print("\t");
  if(debug)Serial.print( freq*((double)waveSteps) );
  if(debug)Serial.print("\n");
}
void noff(){
  noTone(Clock);
}





void setup(){
  int i,j;
  Serial.begin(9600);
  randomSeed(analogRead(5));
  
  
  
  pinMode(OutEn, OUTPUT);
  pinMode(WriteEn, OUTPUT);
  digitalWrite(OutEn, 1); // RAM output off
  digitalWrite(WriteEn, 1); // not writing to RAM
  if(debug) Serial.println("\n\nconfigured SRAM controls");
  
  for(i=0 ; i<8 ; i++)
    pinMode(Mem[i], OUTPUT); // outputs
    digitalWrite(Mem[i], 0); // init all memory to 0
  if(debug) Serial.println("Configured output pins");
  
  pinMode(Clock, OUTPUT);
  pinMode(Reset, OUTPUT);
  digitalWrite(Clock, 0); // clock off
  digitalWrite(Reset, 0); // reset off
  if(debug) Serial.println("Configured counter controls");
  
  //write waveforms
  compute_and_write_waveform_to_SRAM();
  if(debug) Serial.println("Wrote data to SRAM");
  
  //reset address
  digitalWrite(Reset, 0);
  digitalWrite(Reset, 1);
  digitalWrite(Reset, 0);
   if(debug) Serial.println("Reset address");
  
  pinMode(SyncOut, OUTPUT);
  digitalWrite(SyncOut,0);
  
  //disable output pins
  for(i=0 ; i<8 ; i++)
    pinMode(Mem[i], INPUT);
  if(debug) Serial.println("Disabled Mem0-Mem7 outputs");
  
  digitalWrite(OutEn, 0); // turn on SRAM outputs.
  if(debug) Serial.println("Turned on SRAM outputs");
}









int itt = 0;
int freq=440;
int t =0;

void loop(){
  /*
  j++;
  
  for(i=0 ; i<8 ; i++)
    digitalWrite(Mem[i], 0  );
  
  digitalWrite(Mem[j%8], 1);
  delay(250);
  */
  if(debug){
    int i;
    if(itt < 32){
      Serial.print(itt);
      Serial.print(":\t");
      for(i=7; i>=0; i--){
        if(digitalRead(Mem[i])) Serial.print("#");
        else Serial.print("-");
      }
      Serial.println("");
    }
  }
  
  
  
  //if(debug) Serial.print("itteration ");
  //if(debug) Serial.println(itt);
  
  if(itt<32)itt++;
  else
  {
    while(1){
      
      /*
      double freq = 880; //initial freq
      
      while(1){
        freq = freq - 0.003*freq;
        if(debug)Serial.print(freq);
        if(debug)Serial.print("\t");
        non(freq);
        delay(10);
      }
      */
      
      /*
      int time = 1500;
      while(1){
        non(220);
        delay(time);
        non(195.998);
        delay(time);
        non(174.164);
        delay(time);
        non(195.998);
        delay(time);
      }
      */
      
      
      /*
      int t = 150;
      while(1){
        
        non(174.164);
        delay(t);
        noff();
        delay(2*t);
        non(174.164);
        delay(t);
        non(349.228);
        delay(t);
        non(174.164);
        delay(t);
        noff();
        delay(2*t);
      }
      */
      
      
      // the story so far
      int t = 150;
      while(1){
        digitalWrite(SyncOut, 1); // output a gate signal signifying the beginning of the measure
        non(329.628);
        delay(t);
        digitalWrite(SyncOut, 0); // turn off the gate signal.
        non(195.998);
        delay(t);
        non(261.626);
        delay(t);
        non(293.665);
        delay(t);
        non(329.628);
        delay(2*t);
        non(391.995);
        delay(2*t);
        non(329.628);
        delay(4*t);
        non(293.665);
        delay(t);
        non(329.628);
        delay(t);
        non(293.665);
        delay(t);
        non(261.626);
        delay(t);
      }
      
      
      
      /*
      while(1){
        int t = 38;
        non(220.000);
        delay(2*t);
        non(233.082);
        delay(t);
        non(246.942);
        delay(t);
        non(261.626);
        delay(t);
        non(277.183);
        delay(t);
        non(293.665);
        delay(t);
        non(311.127);
        delay(t);
        non(329.628);
        delay(t);
        non(349.228);
        delay(t);
        non(369.994);
        delay(t);
        non(391.995);
        delay(t);
        non(415.305);
        delay(t);
        non(440.000);
        delay(t);
        
      }
      */
      
      /*
      //2013-07-03  4:07pm
      //this makes an odd sound effect
      non(freq);
      freq += 0.04*((float)freq);
      delay(50);
      */
      
      /*
      //plays random pentatonic(ish) scale sequences
      double freqs[10] = {440.000, 349.228, 261.626, 261.626, 293.665, 329.628, 391.995, 440.000, 523.251, 659.255};
      //current
      non(freqs[random(0,9)]);
      delay(110*random(1,4));
      */
    }
  }
}
  
