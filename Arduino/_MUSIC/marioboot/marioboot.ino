/*This is the mario boot function, it is designed to play 
the intro to Mario Bros. on a piezo speaker.*/

void marioboot(int speakerpin)
{
  tone(speakerpin,660,200);
  delay(200);
  tone(speakerpin,660,200);
  delay(400);
  tone(speakerpin,660,200);
  delay(400);
  tone(speakerpin,524,200);
  delay(200);
  tone(speakerpin,660,200);
  delay(400);
  tone(speakerpin,784,200);
  delay(800);
  tone(speakerpin,392,200);
}


void setup(){
  pinMode(9, OUTPUT);
}

void loop(){
  
  marioboot(9);
  
  
  
  
}
