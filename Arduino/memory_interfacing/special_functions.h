 //definitions for the inverted input control pins on the 32K SRAM chip
 #define ON 0
 #define OFF 1
 
// 16 bit shift register in charge of the address
#define address_data 8
#define address_clock 9
// 8 bit shift register in charge of the data at the address
#define data_data 10
#define data_clock 11
// these pins are connected to the 32K Static RAM chip. these are inverted inputs. active LOW
#define write_enable 12
#define output_enable 13

void write_data()
{
  digitalWrite(write_enable, 0); // activate writing
  digitalWrite(write_enable, 1); //  disable writing
}

void output(byte state)
{
  digitalWrite(output_enable, state); //turn it ON or OFF
}


void shifty_shifty(byte D_pin, byte C_pin, char BIT_ORD, int data, byte bits)
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



void data(int number)
{
  shifty_shifty(data_data, data_clock, 'm', number, 16);
}



void address(unsigned int number)
{
  shifty_shifty(address_data, address_clock, 'm', number, 16);
}
