/*
connect the pin 9 of each Arduino to its own speaker.
connect the pin 10's of the intro arduino and the bass arduino together.
connect the 5V pins together for all arduinos.
connect the GND pins together for all arduinos.

with this setup, one arduino will be powering all of the rest.
consider using an external power supply when using more than 2 arduinos.




*/


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
  
  pinMode(bass_pin, OUTPUT);
  digitalWrite(bass_pin, 0);

}

void loop() {
  
  play_intro(2);
  digitalWrite(bass_pin, 1);
  // play 4 measures of intro
  //play_intro(4);
  //play_bass(2);
  
  // plays 4 more measures of intro and transitions
  //play_intro(7);
  //play_intro_transition(1);
  
  
  
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
