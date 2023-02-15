#define pinData 13
#define pinClock 12
#define pinAnalog 0

#define BAR0  0
#define BAR1  1
#define BAR2  3
#define BAR3  7
#define BAR4  15
#define BAR5  31
#define BAR6  63
#define BAR7  127
#define BAR8  255

void shifty_shifty
(byte D_pin, byte C_pin, char BIT_ORD, byte data, byte bits); // declares the my_shift() function used for operating a shift register

void setup()
{
  Serial.begin(9600);
  pinMode(pinData, OUTPUT);
  pinMode(pinClock, OUTPUT);
}

void loop()
{
  byte data[15] = { BAR0, BAR1, BAR2, BAR3, BAR4, BAR5, BAR6, BAR7, BAR8, BAR7, BAR6, BAR5, BAR4, BAR3, BAR2 };
  int i;
  while(1)
  {
    i = (analogRead(pinAnalog) + 1) / 128;
    shifty_shifty( pinData, pinClock, 'l', data[i], 8);
    delay(20);
    Serial.println(i);
  }
}







void shifty_shifty (byte D_pin, byte C_pin, char BIT_ORD, byte data, byte bits)
{
  int i; // itterations variable used for for() loops
  int data_array[bits]; // least significant in data_array[0]

  for (i = 0; i < bits; i++) {
    data_array[i] = data % 2; // store the least significant bit of data in the ith array element
    data = data >> 1;
  }

  if (BIT_ORD == 'M' || BIT_ORD == 'm') {
    for (i = bits - 1; i >= 0; i--) {
      digitalWrite(C_pin, 0); // set clock low
      digitalWrite(D_pin, data_array[i]); // set data to the right value (1 or 0)
      digitalWrite(C_pin, 1); // clock the data into the shift register
    }
  }

  if (BIT_ORD == 'L' || BIT_ORD == 'l') {
    for (i = 0 ; i < bits; i++) {
      digitalWrite(C_pin, 0); // set clock low
      digitalWrite(D_pin, data_array[i]); // set data to the right value (1 or 0)
      digitalWrite(C_pin, 1); // clock the data into the shift register (so to speak)
    }
  }
}
