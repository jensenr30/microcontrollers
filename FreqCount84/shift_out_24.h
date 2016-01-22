
// this function shifts out data to a serial-in-parallel-out shift register on some port.
// This function works on ports A and B
// This function was designed to be used on the ATtiny24A microcontroller,
// but it will likely work on many other Ateml microcontrollers will similar port configurations.

// "port" is a character 'a', 'b', 'A', or 'B'. This tells the function what register to use
// "clockPin" is the pin in "port" that will be used as the clock of the shift register. values 0-7 are acceptable.
// "dataPin"  is the pin in "port" that will be used as the data  of the shift register. values 0-7 are acceptable.
	// NOTE: This function will NOT set the DDRx registers.
	// The clockPin and dataPin of the port need to be configured as outputs before attempting to shift out data.
// bits is the number of bits that need to be shifted out.
// data is the data to be shifted out.
// bitDirection is 'm' or 'M' if you want to shift out the most significant bit first (MSBF). Any other character will be interpreted as LSBF.
// this will shift out (at most) 32 bits of information at a time.
void shift_out_328(char port, uint8_t clockPin, uint8_t dataPin, uint8_t bits, uint32_t data, int8_t bitDirection){
	
	uint32_t mask = 1;		// this is used to selectively pick bits from "data" to shift out one at a time.
	
	// set up the initial state of the bit mask
	if(bitDirection == 'm' || bitDirection == 'M'){
		mask <<= (bits-1);
	}
	
	for(; bits > 0; bits--){
			
		//set data bit high if needed
		if(data&mask){
			switch(port){
			case 'a':
			case 'A':
				PORTA |=  (1 << dataPin);
				break;
			case 'b':
			case 'B':
				PORTB |=  (1 << dataPin);
				break;
			default:
				break;
			}
		}
		// otherwise, set the data bit low
		else{
			switch(port){
				case 'a':
				case 'A':
					PORTA &= ~(1 << dataPin);
					break;
				case 'b':
				case 'B':
					PORTB &= ~(1 << dataPin);
					break;
				default:
					break;
			}
		}
			
		//clock data into the register.
		switch(port){
			case 'a':
			case 'A':
				PORTA &= ~(1 << clockPin);
				PORTA |=  (1 << clockPin);
				break;
			case 'b':
			case 'B':
				PORTB &= ~(1 << clockPin);
				PORTB |=  (1 << clockPin);
				break;
			default:
				break;
		}
			
		// modify mask for the next loop iteration.
		if(bitDirection == 'm' || bitDirection == 'M')		// if the bit direction if MSB (Most Significant Bit first),
			mask >>= 1;										// shift the mask right (decrease the bit mask).
		else												// otherwise,
			mask <<= 1;										// shift the mask left  (increase the bit mask).
	}
}
