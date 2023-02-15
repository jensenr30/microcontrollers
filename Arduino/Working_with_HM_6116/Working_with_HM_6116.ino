/* shifty_shifty is able to handle odd shift registers. It is possible to use up to 255 bit shift registers with the byte varible being a byte
 *
 * first parameter  = D_pin: the pin that the data is sent to.
 * second parameter = C_pin: the clock pin for the shift register
 * thrid parameter  = BIT_ORD: least significant or most significant bit first (in what order does the binry number output)
 * fourth parameter = data: the number that the shift register will convert to binary and shift out
 * fifth parameter  = bits: how many bits will you be shifting out at a time with your shift register
 */
 
#define    pinClear  13
#define    pinClock  12
#define    pinData   11
#define    pinW1     10
#define    pinW2     9
#define    pinOE     8
#define    pinA10    4
#define    pinA9     3
#define    pinA8     2






void shifty_shifty
(byte D_pin, byte C_pin, char BIT_ORD, int data, byte bits); // declares the my_shift() function used for operating a shift register






void setup()
{
  pinMode(pinClear, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinData,  OUTPUT);
  pinMode(pinW1,    OUTPUT);
  pinMode(pinW2,    OUTPUT);
  pinMode(pinOE,    OUTPUT);
  pinMode(pinA10,   OUTPUT);
  pinMode(pinA9,    OUTPUT);
  pinMode(pinA8,    OUTPUT);
  
  digitalWrite(pinClear, LOW);
  digitalWrite(pinClock, LOW);
  digitalWrite(pinData,  LOW);
  digitalWrite(pinW1,    LOW);
  digitalWrite(pinW2,    LOW);
  digitalWrite(pinOE,    LOW);
  digitalWrite(pinA10,   LOW);
  digitalWrite(pinA9,    LOW);
  digitalWrite(pinA8,    LOW);
}





void loop()
{
  //set higher addresses low (just to keep them constant
  digitalWrite(pinA8,  LOW);
  digitalWrite(pinA9,  LOW);
  digitalWrite(pinA10, LOW);
  
  //enable write for both chips
  digitalWrite(pinW1, LOW);
  digitalWrite(pinW2, LOW);
  
  //don't clear the shift registers.
  digitalWrite(pinClear, HIGH);
  
  shifty_shifty(pinData, pinClock, 'M', 256*256-1, 16);
  
  //disable write for both chips
  digitalWrite(pinW1, HIGH);
  digitalWrite(pinW2, HIGH);
  
  digitalWrite(pinOE, LOW); //turn output enable on
  while(1) delay(1000);
}








void shifty_shifty (byte D_pin, byte C_pin, char BIT_ORD, int data, byte bits)
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


