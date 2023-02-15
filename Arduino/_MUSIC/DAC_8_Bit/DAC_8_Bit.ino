#define dac0 2 //least significant
#define dac1 3
#define dac2 4
#define dac3 5
#define dac4 6
#define dac5 7
#define dac6 11
#define dac7 12 // most significant

#define memclock 8

#define W_SINE 0
#define W_TRIANGLE 1
#define W_RAMP 2
#define w_SQUARE 3

#define MAX_STEPS 1000


int i,j; // general purpose variable



void setup() {
  pinMode(dac0, OUTPUT);
  pinMode(dac1, OUTPUT);
  pinMode(dac2, OUTPUT);
  pinMode(dac3, OUTPUT);
  pinMode(dac4, OUTPUT);
  pinMode(dac5, OUTPUT);
  pinMode(dac6, OUTPUT);
  pinMode(dac7, OUTPUT);
  
  pinMode(memclock, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13,0);
}


void loop(){
  
  
  
  //specify the waveform(s)
  double frequency;
  int waveShape;
  
  frequency = 820; // desired frequency
  waveShape = W_SINE;  // wave shape (W_SINE, W_TRIANGLE, W_RAMP, etc...)
  
  
  //initialize variables for waveform synthesis
  double stepTime =  0.000046875; // (in seconds) this is how long the loop takes to complete a single step in the "making the wave" loop below.
  float multiplier; // this is used in the case statement when generating the wave
  double period;
  int steps;
  int wave[MAX_STEPS];
  
  // get correct data put into arrays.
  period = 1/frequency;
  steps = period/stepTime;
  if(steps >= MAX_STEPS) steps = MAX_STEPS -1; // make sure there aren't too many steps

  
  //generate the different types of waveforms
  multiplier = 2*3.141593/((float)steps);
  for(j=0 ; j<steps ; j++){ // calculate step binary values
    wave[j] = 120 * ( sin(((float)j)*multiplier) + 1);
  }
  
  /*
  wave[0] = 0;
  wave[1] = 32;
  wave[2] = 64;
  wave[3] = 96;
  wave[4] = 128;
  wave[5] = 128 + 32;
  wave[6] = 128 + 64;
  wave[7] = 128 + 96;
  wave[8] = 128 + 128;
  */
  
  //play the wave
  int binary;
  while(1){
    for(i=0 ; i<steps ; i++){
      
      
      binary = wave[i];
      if (binary >=128){ digitalWrite(dac7, HIGH); binary %= 128;} else { digitalWrite(dac7, LOW);}
      if (binary >=64){  digitalWrite(dac6, HIGH); binary %= 64;} else  { digitalWrite(dac6, LOW);}
      if (binary >=32){  digitalWrite(dac5, HIGH); binary %= 32;} else  { digitalWrite(dac5, LOW);}
      if (binary >=16){  digitalWrite(dac4, HIGH); binary %= 16;} else  { digitalWrite(dac4, LOW);}
      if (binary >=8){   digitalWrite(dac3, HIGH); binary %= 8;}  else  { digitalWrite(dac3, LOW);}
      if (binary >=4){   digitalWrite(dac2, HIGH); binary %= 4;}  else  { digitalWrite(dac2, LOW);}
      if (binary >=2){   digitalWrite(dac1, HIGH); binary %= 2;}  else  { digitalWrite(dac1, LOW);}
      if (binary >=1){   digitalWrite(dac0, HIGH);             }  else  { digitalWrite(dac0, LOW);}
      digitalWrite(memclock, 0);
      digitalWrite(memclock, 1);
    }
  }
}





