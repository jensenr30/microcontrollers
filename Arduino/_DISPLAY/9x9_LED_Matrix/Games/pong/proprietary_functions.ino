///////////////////////////////
//                           //
//   PROPRIETARY FUNCTIONS   //
//                           //
///////////////////////////////

#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294

#define NOTE_B5  988
//#define NOTE_C6  1047
//#define NOTE_CS6 1109
#define NOTE_D6  1175
//#define NOTE_DS6 1245
//#define NOTE_E6  1319
//#define NOTE_F6  1397
#define NOTE_FS6 1480
//#define NOTE_G6  1568
//#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_B6  1976


// this is the function that displays a number of frames. for example, disp(5); will display 5 frames of whateever is in dispdata[9][2]
void disp(int frames) {
  for(int frame_count = 0; frame_count < frames; frame_count++) {
    // reset 4017 decade counter
    digitalWrite(reset_4017, 1);
    digitalWrite(reset_4017, 0);
    
    digitalWrite(enable, 0); // turn off 
    
    // this displays each row of data once
    for(row = 0; row < 9; row++) {
      
      shiftOut(data_serial, clock_serial, MSBFIRST, ((int) dispdata[row][0]));
      digitalWrite(data_serial, dispdata[row][1]);
      digitalWrite(enable, 1);
      
      delayMicroseconds(1440); // this is how long each row is displayed
      
      digitalWrite(enable, 0); // turn off display
      
      digitalWrite(clock_4017, 0); // move to next row
      digitalWrite(clock_4017, 1);
      
    }
  }
}


void loose()
{
  blankdispdata();
  digitalWrite(enable, 0);
  Serial.print("You ");
  tone(speaker_pin, NOTE_D4);
  delay(120);
  noTone(speaker_pin);
  delay(120);
  
  Serial.print("Just ");
  tone(speaker_pin, NOTE_CS4);
  delay(120);
  noTone(speaker_pin);
  delay(120);
  
  Serial.print("Lost.");
  tone(speaker_pin, NOTE_B3);
  delay(120);
  noTone(speaker_pin);
  delay(240);
  Serial.println("");
  
  coordinate[0] = 4; coordinate[1] = 0; // reset the paddle place
  trajectory = -2;
  number_of_paddle_hits = 0;
  level = 8;
}
  
  
  
  
  
  
void levelup()
{
  blankdispdata();
  Serial.print("L");
  tone(speaker_pin, NOTE_B5);
  dispdata[0][0] = 1;
  disp(5);
  noTone(speaker_pin);
  
  Serial.print("E");
  tone(speaker_pin, NOTE_D6);
  dispdata[0][0] = 0;
  disp(5);
  noTone(speaker_pin);
  
  Serial.print("V");
  tone(speaker_pin, NOTE_FS6);
  dispdata[0][0] = 1;
  disp(5);
  noTone(speaker_pin);
  
  Serial.print("E");
  tone(speaker_pin, NOTE_D6);
  dispdata[0][0] = 0;
  disp(5);
  noTone(speaker_pin);
  
  Serial.print("L ");
  tone(speaker_pin, NOTE_A6);
  dispdata[0][0] = 1;
  disp(20);
  noTone(speaker_pin);
  disp(20);
  
  Serial.print("U");
  tone(speaker_pin, NOTE_A6);
  dispdata[0][0] = 0;
  disp(10);
  noTone(speaker_pin);
  
  Serial.println("P");
  tone(speaker_pin, NOTE_B6);
  dispdata[0][0] = 1;
  disp(10);
  noTone(speaker_pin);
  disp(10);
  
  
  level--;
  coordinate[0] = 4; coordinate[1] = 0; // reset the paddle place
  trajectory = -2;
  number_of_paddle_hits = 0;
}



void blankdispdata()
{
  dispdata[0][0] = dispdata[0][1] = dispdata[1][0] = dispdata[1][1] = dispdata[2][0] = dispdata[2][1] = dispdata[3][0] = dispdata[3][1] = dispdata[4][0] = dispdata[4][1] = dispdata[5][0] = dispdata[5][1] = dispdata[6][0] = dispdata[6][1] = dispdata[7][0] = dispdata[7][1] = dispdata[8][0] = dispdata[8][1] = 0;
}
