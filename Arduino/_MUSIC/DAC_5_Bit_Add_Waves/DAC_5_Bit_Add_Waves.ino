#define dac0 2
#define dac1 3
#define dac2 4
#define dac3 5
#define dac4 6
#define memclock 8
#define W_SINE 0
#define W_TRIANGLE 1
#define W_RAMP 2
#define w_SQUARE 3
#define MAX_STEPS 200
#define MAX_WAVES 10

int i,w,j; // general purpose variable



void setup() {
  //Serial.begin(9600);
  pinMode(dac0, OUTPUT);
  pinMode(dac1, OUTPUT);
  pinMode(dac2, OUTPUT);
  pinMode(dac3, OUTPUT);
  pinMode(dac4, OUTPUT);
  pinMode(memclock, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13,0);
}


void loop(){
  
  
  
  //specify the waveform(s)
  double frequency[MAX_WAVES];
  int waveShape[MAX_WAVES];
  int numberOfWaves = 6; // number of waves to add together
  
  //set this is the highest common root harmonic of the following frequencies.
  //what that means is that the other frequencies should be able to be made by multiplying this frequency by an integer.
  //this defines the period over which the following waves are added
  int commonFrequency = 174.614;
  
  /*
  frequency[0] = 0 *commonFrequency; // desired frequency
  waveShape[0] = W_SINE;  // wave shape (W_SINE, W_TRIANGLE, W_RAMP, etc...)
  
  frequency[1] = 0 *commonFrequency;
  waveShape[1] = W_SINE;
  
  frequency[2] = 0 *commonFrequency;
  waveShape[2] = W_SINE;
  
  frequency[3] = 0 *commonFrequency;
  waveShape[3] = W_SINE;
  
  frequency[4] = 0 *commonFrequency;
  waveShape[4] = W_SINE;
  
  frequency[5] = 0 *commonFrequency;
  waveShape[5] = W_SINE;
  
  frequency[6] = 0 *commonFrequency;
  waveShape[6] = W_SINE;
  */
  
  frequency[0] = 174.614; // desired frequency
  waveShape[0] = W_SINE;  // wave shape (W_SINE, W_TRIANGLE, W_RAMP, etc...)
  
  frequency[1] = 220;
  waveShape[1] = W_SINE;
  
  frequency[2] = 349.228;
  waveShape[2] = W_SINE;
  
  frequency[3] = 440;
  waveShape[3] = W_SINE;
  
  frequency[4] = 587.330;
  waveShape[4] = W_SINE;
  
  frequency[5] = 783.991;
  waveShape[5] = W_SINE;
  
  frequency[6] = 0 *commonFrequency;
  waveShape[6] = W_SINE;
  
  //initialize variables for waveform synthesis
  double stepTime =  0.000046875; // (in seconds) this is how long the loop takes to complete a single step in the "making the wave" loop below.
  float multiplier; // this is used in the case statement when generating the wave
  double period[MAX_WAVES];
  int steps[MAX_WAVES];
  int wave[MAX_WAVES][MAX_STEPS];
  
  // get correct data put into arrays.
  for(w=0 ; w<numberOfWaves ; w++){
    period[w] = 1/frequency[w];
    steps[w] = period[w]/stepTime;
    if(steps[w] > MAX_STEPS) steps[w] = MAX_STEPS; // make sure there aren't too many steps
  }
  
  //these are calculated.
  double commonPeriod = 1.0/( (float) commonFrequency );
  int commonSteps = commonPeriod/stepTime;
  if(commonSteps > MAX_STEPS) commonSteps = MAX_STEPS;
  
  //generate the different types of waveforms
  for(w=0 ; w<numberOfWaves ; w++)
  {
    multiplier = 2*3.141593/((float)steps[w]);
    for(j=0 ; j<commonSteps ; j++){ // calculate step binary values
      wave[w][j] = 13 * ( sin(((float)j)*multiplier) + 1);
    }
  }
  
  
  //adding the waves together into one waveform
  for(i=0 ; i<commonSteps ; i++){
    for(w=1 ; w<numberOfWaves ; w++){
      wave[0][i] += wave[w][i];
    }
    wave[0][i] /= ((float)numberOfWaves);
  }
  
  
  
  
  //play the wave
  int binary;
  while(1){
    for(i=0 ; i<commonSteps ; i++){
      
      
      binary = wave[0][i];
      if (binary >=16){ digitalWrite(dac4, HIGH); binary %= 16;}  else { digitalWrite(dac4, LOW);}
      if (binary >=8){  digitalWrite(dac3, HIGH); binary %=  8;}  else { digitalWrite(dac3, LOW);}
      if (binary >=4){  digitalWrite(dac2, HIGH); binary %=  4;}  else { digitalWrite(dac2, LOW);}
      if (binary >=2){  digitalWrite(dac1, HIGH); binary %=  2;}  else { digitalWrite(dac1, LOW);}
      if (binary >=1){  digitalWrite(dac0, HIGH);}                else { digitalWrite(dac0, LOW);}
      digitalWrite(memclock, 0);
      digitalWrite(memclock, 1);
    }
  }
}
