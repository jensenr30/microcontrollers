/* shifty_shifty is able to handle odd shift registers. It is possible to use up to 255 bit shift registers with the byte varible being a byte
 *
 * first parameter  = D_pin: the pin that the data is sent to.
 * second parameter = C_pin: the clock pin for the shift register
 * thrid parameter  = BIT_ORD: least significant or most significant bit first (in what order does the binry number output)
 * fourth parameter = data: the number that the shift register will convert to binary and shift out
 * fifth parameter  = bits: how many bits will you be shifting out at a time with your shift register
 */
 void shifty_shifty(byte D_pin, byte C_pin, char BIT_ORD, int data, byte bits);
 #include "special_functions.h"
void setup()
{
  Serial.begin(9600);
  // 16 bit shift register
  pinMode(address_clock, OUTPUT);
  pinMode(address_data, OUTPUT);
  
  // 8 bit shift register
  pinMode(data_data, OUTPUT);
  pinMode(data_clock, OUTPUT);
  
  // Static RAM
  pinMode(write_enable, OUTPUT);
  pinMode(output_enable, OUTPUT);
  
  //set hardware to default state: write disabled and output disabled.
  write_data();
  output(OFF);
  // 0 out the address
  shifty_shifty(address_data, address_clock, 'm', 0, 16);
}

int i = 0;

void loop()
{
  int number = 1;
  for(i = 0; i < 16; i++)
  {
    //shift out address
    address(i);
    //shift out data
    data(number);
    write_data();
    number += 16; // increment number by 16
  }
  
  data(0);
  output(ON);
  
  while(1)
  {
    Serial.println("New");
    for(i = 0; i < 16; i++)
    {
      output(OFF);
      address(i);
      //output(ON); 
      delayMicroseconds(800);
    }
  }
    
}


