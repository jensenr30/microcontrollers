#include "pitches.h"



#define measure_length 3200  // defines measure length
#define binary_counter 9
#define bass_pin 10

int gen;
int intro[8] =         {NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4};
float intro_time[8] =  {8, 8, 8, 16, 8, 8, 8, 16};
int intro_transition[8] =            {NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D5, NOTE_C5};
float intro_transition_time[8] =  {8, 8, 8, 16, 8, 8, 8, 16};
int bass[4] = {NOTE_D3, NOTE_AS2, NOTE_C3, NOTE_G2};
int bass_length = 7 * (measure_length / 8);


// FUNCTIONS //////////////////////////////
void play_intro(int repeats);
void play_intro_transition(int repeats);


void setup() {
  pinMode(binary_counter, OUTPUT);
  digitalWrite(binary_counter, LOW);
  
  pinMode(bass_pin, INPUT);

}

void loop() {
  
  while(!digitalRead(bass_pin));
  digitalWrite(13, 1);
  play_bass(2);
  
  
  
}




void play_intro(int repeats) {
  for(;repeats; repeats--){
    for(gen = 0; gen < 8; gen++) {
      tone(binary_counter, intro[gen], (measure_length - 500) / intro_time[gen]);
      delay(measure_length / intro_time[gen]);
    }
  }
}

void play_intro_transition(int repeats) {
  for(;repeats; repeats--){
    for(gen = 0; gen < 8; gen++) {
      tone(binary_counter, intro_transition[gen], (measure_length -500) / intro_transition_time[gen]);
      delay(measure_length / intro_transition_time[gen]);
    }
  }
}

void play_bass(int repeats) {
    for(; repeats; repeats--) {
      for(gen = 0; gen < 4; gen++) {
        tone(binary_counter, bass[gen], bass_length);
        delay(bass_length);
      }
    }
}
