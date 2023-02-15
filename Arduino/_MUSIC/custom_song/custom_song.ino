/* I think something is fucking with the data in the array because when the songe plays the first time, it is fine. the second and third times, it sounds shitty.
unless there is something wrong with the arduino itself, there must be a problem in the code...
well, i'm off to what the value of OUTPUT is.

*/

#include "pitches.h"
#define speaker 9


#define measure 1200
#define half measure/2
#define quarter measure/4
#define eighth measure/8
#define sixteenth measure/16
#define rest 0

int melody1[18] = {NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_G4, NOTE_E4, NOTE_G4, rest,    NOTE_G4, NOTE_F4, NOTE_E4, NOTE_C4, rest   },
time1[18] =       {eighth,  eighth,  eighth,  eighth,  eighth,  eighth,  eighth,  eighth,  quarter, quarter, quarter, quarter, quarter, quarter, quarter, quarter, quarter, half + quarter},
notes1 = 18;  ///   0        1          2        3      4        5        6        7        8          9        10      11      12        13        14      15      16        17     

int melody2[20] = {NOTE_A3, NOTE_C4, NOTE_E4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_C5, rest,    NOTE_C5, NOTE_D5, rest,    NOTE_G4},
time2[20] =       {eighth,  eighth,  eighth,  eighth,  eighth,  eighth,  eighth,  eighth,  quarter, quarter, eighth,  eighth,  eighth,  eighth,  half,    quarter, quarter, half,    quarter, quarter},
notes2 = 20;   ///  0        1          2        3      4        5        6        7        8          9        10      11      12        13        14      15      16      17        18      19

void play_melody1(int times);
void play_melody2(int times);

void setup()
{
  pinMode(speaker, OUTPUT);
}

void loop()
{
  
 //play_melody1(2);
 play_melody2(3);
 play_melody1(1);
  
  delay(10000);
}

void play_melody2(int times)
{
  for(int j = 0; j < times; j++)
  {
    for(int i = 0; i < notes2; i++)
    {
      tone(speaker, melody2[i], time2[i]);
      delay(time2[i]);
    }
  }
}


void play_melody1(int times)
{
  for(int j = 0; j < times; j++)
  {
    for(int i = 0; i < notes1; i++)
    {
      tone(speaker, melody1[i], time1[i]);
      delay(time1[i]);
    }
  }
}
