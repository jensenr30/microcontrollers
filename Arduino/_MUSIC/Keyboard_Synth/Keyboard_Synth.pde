//                   PARTS and INSTRUCTIONS:
//
// Eight tactile push button switches 
// each switch is wired in series from +5V, through an individual resistor, and to A0)
// sevon resistors (one switch is wired to A0 without any resistance)
//    Middle C....100Kohm
//    D4..........68Kohm
//    E4..........47Kohm
//    F4..........33Kohm
//    G4..........22Kohm
//    H4..........10Kohm
//    A4..........4.7Kohm
//    Tenor C.....(no resistor)
// connect A0 to GND via 68Kohm resistor
// connect a small, computer-sized speaker to Pin8 and GND





#define SENSOR 0 // select the input pin for the
// sensor resistor
#define SWITCH 9
int val = 0;
int valswitch = 0;



void setup() {
  pinMode(SWITCH, OUTPUT);
}
void loop() {
  bailout:
  val = analogRead(SENSOR); // read the value from the sensor

  valswitch = digitalRead(SWITCH);  //read


  // TONE  GENERATING:
  if (val <= 390){
    goto bailout;
  }
  if (val > 1000)  //Determines the tactile switches resistance
  {
    tone(8,523.2,10);  // Tenor C
  }
  if (980 > val){  //Determines the tactile switches resistance
    if (val > 940){  //Determines the tactile switches resistance
      if (valswitch == HIGH){
        tone(8,493.8,10);  // B4
      } 
      else {
        tone(8,466.1,10);
      }
    }
  }
  if (910 > val){  //Determines the tactile switches resistance
    if (val > 870){  //Determines the tactile switches resistance
      if (valswitch == HIGH){
        tone(8,440,10);  // A4
      } 
      else {
        tone(8,415.3,10);
        }
      }
    }
    if (790 > val){  //Determines the tactile switches resistance
      if (val > 750){  //Determines the tactile switches resistance
        tone(8,391.9,10);  //G4
      }
    }
  if (710 > val){  //Determines the tactile switches resistance
    if (val > 670){  //Determines the tactile switches resistance
      tone(8,349.2,10);  //F4
    }
  }
  if (620 > val){  //Determines the tactile switches resistance
    if (val > 580){  //Determines the tactile switches resistance
      if (valswitch == HIGH){
        tone(8,329.6,10);  //E4
      } 
      else {
        tone(8,311.1,10);  //Eb4
      }
    }
  }
  if (530 > val){  //Determines the tactile switches resistance
    if (val > 490){  //Determines the tactile switches resistance
      tone(8,293.6,10);  //D4
    }
  }
  if (430 > val){  //Determines the tactile switches resistance
    if (val > 390){  //Determines the tactile switches resistance
      tone(8,261.6,50);  // Middle C
    }
  }
}






