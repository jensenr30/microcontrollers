
// this function shifts out data to a serial-in-parallel-out shift register on some port.
// This function was designed to be used on the ATtiny24A microcontroller,
// but it will likely work on many other Ateml microcontrollers will similar port configurations.

// This shifts exclusively using PORTA pins
// "clockPin" is the pin in "port" that will be used as the clock of the shift register. values 0-7 are acceptable.
// "dataPin"  is the pin in "port" that will be used as the data  of the shift register. values 0-7 are acceptable.
	// NOTE: This function will NOT set the DDRx registers.
	// The clockPin and dataPin of the port need to be configured as outputs before attempting to shift out data.
// bits is the number of bits that need to be shifted out.
// data is the data to be shifted out.
// bitDirection is 'm' or 'M' if you want to shift out the most significant bit first (MSBF). Any other character will be interpreted as LSBF.
// this will shift out (at most) 16 bits of information at a time.
void shift_out_24_PORTA(uint8_t clockPin, uint8_t dataPin, uint8_t bits, uint16_t data, int8_t bitDirection){
	
	uint16_t mask;		// this is used to selectively pick bits from "data" to shift out one at a time.
	
	// set up the initial state of the bit mask
	if(bitDirection == 'm' || bitDirection == 'M'){
		mask = 1 << (bits-1);
	}
	else
	{
		mask = 1;
	}
	
	for(; bits > 0; bits--)
	{
		//set data bit high if needed
		if(data&mask)
		{
			PORTA |=  (1 << dataPin);
		}
		// otherwise, set the data bit low
		else
		{
			PORTA &= ~(1 << dataPin);
		}
			
		//clock data into the register.
		PORTA &= ~(1 << clockPin);
		PORTA |=  (1 << clockPin);
		
		// modify mask for the next loop iteration.
		if(bitDirection == 'm' || bitDirection == 'M')		// if the bit direction if MSB (Most Significant Bit first),
			mask >>= 1;										// shift the mask right (decrease the bit mask).
		else												// otherwise,
			mask <<= 1;										// shift the mask left  (increase the bit mask).
	}
}
