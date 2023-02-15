/* shifty_shifty is able to handle odd shift registers. It is possible to use up to 255 bit shift registers with the byte varible being a byte
 *
 * first parameter  = D_pin: the pin that the data is sent to.
 * second parameter = C_pin: the clock pin for the shift register
 * thrid parameter  = BIT_ORD: least significant or most significant bit first (in what order does the binry number output)
 * fourth parameter = data: the number that the shift register will convert to binary and shift out
 * fifth parameter  = bits: how many bits will you be shifting out at a time with your shift register
 */



void shifty_shifty
(byte D_pin, byte C_pin, char BIT_ORD, byte data, byte bits); // declares the my_shift() function used for operating a shift register



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


