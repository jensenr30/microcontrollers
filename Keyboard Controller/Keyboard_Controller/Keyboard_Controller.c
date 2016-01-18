/*
 * Keyboard_Controller.c
 *
 * Created: 5/6/2014 6:51:26 PM
 * Author: Ryan Jensen
 * 
 * The purpose of this program is to control a keyboard (that is wired in a keypad matrix format).
 * This microcontroller will drive two 12-bit DACs that will convert a series of 1's and 0's into an analog CV.
 * That analog CV will in turn control some oscillator or other analog device.
 
 * There will also be a "Gate" output and a "Retrigger" output.
 * The gate output will go high whenever any key is pressed.
 * The retrigger output will go high when another key is pressed while that last one is still being played.
 
 * Ideally, there would be 61 DACS (for 61 keys) but that would be impractical and I would need 61 oscillators.
 * So as a compromise, I would like to have 2 12-bit DACs (24 bits total, three 8 bit shift registers and three 8 bit buffers)
 * this would allow me to play a bass synth and a lead at the same time.
 * This will need to be accomplished through splitting the piano into two "voices"
 */ 

/*

KEYBOARD LAYOUT

61 keys total



C1								oct_0

C#1	D1	D#1	E1	F1	F#1			oct_1A

G1	G#1	A1	A#1	B1	C2			oct_1B

C#2	D2	D#2	E2	F2	F#2			oct_2A

G2	G#2	A2	A#2	B2	C3			oct_2B

C#3	D3	D#3	E3	F3	F#3			oct_3A

G3	G#3	A3	A#3	B3	C4			oct_3B

C#4	D4	D#4	E4	F4	F#4			oct_4A

G4	G#4	A4	A#4	B4	C5			oct_4B

C#5	D5	D#5	E5	F5	F#5			oct_5A

G5	G#5	A5	A#5	B5	C6			oct_5B






columns 0-5 (6 total columns) correspond to what key is played in the half octave.

Rows 0-10 (11 total rows) correspond to which half octave has been pushed down.


*/





//-----------------------------------------------
// HEADERS
//-----------------------------------------------

#define F_CPU 8000000			// CPU clock speed. This definition is for delay.h
#include <avr/io.h>				// because I'm almost certain I need this
#include <util/delay.h>			// for the delay functions

#define bool char				// It is easier to understand the variable's purpose when it is named in this way.
#define true 1					// shorthand
#define false 0					// shorthand


//-----------------------------------------------
// PIN DEFINITIONS
//-----------------------------------------------



// this is the master output shift register that outputs:
// DAC information and gate signals.
#define p_shift_master_clock	PORTD0		// this is the clock pin for the master output shift register.
#define p_shift_master_data		PORTD1		// this is the data  pin for the master output shift register.
#define p_shift_master_update	PORTD2		// this is the buffer update pin for the master output shift register.

// this is the 8-bit shift register that outputs data to the keyboard.
#define p_shift_keys_clock		PORTD3		// This is the clock of the output 8-bit shift register.
// Six bits are used for the keyboard controller.
#define p_shift_keys_data		PORTD4		// This is the data pin of the shift register.

// this is the input shift register that inputs data from the keyboard
#define p_shift_in_clock		PORTC0		// this clocks the shift register, bringing the information bit by bit to the data input.
#define p_shift_in_grab			PORTC1		// when this input is in the low state, the shift register is inputting all 16 bits.
// when it goes high, it stops inputting bits and stores the values.
#define p_shift_in_data			PORTC2		// data input. This is the serial data from the input of the shift register.
// Each bit will represent one half-octave of the matrix. The bits are shifted in from.
// oct_0 to oct_5.

// these are extra pins from the 16-bit shift-in register that are used for things other than grabbing data from the keyboard
// this is the bit in the 16-bit shift-in register that tells if the user wants to calibrate the device
// active high.
#define b_calibrate_enable 11 // (12th bit)


//-----------------------------------------------
// DAC STUFF
//-----------------------------------------------

#define REFERENCE_VOLTAGE	5.0f	
#define DAC_BIT_LENGTH		12		// this is how many bits the two DACs have
#define DAC_VOLTS_PER_OCTAVE 1.0f	// this is how many volts per octave there are (linear)
									// this is a linear relationship between key and voltage that starts at 0 V at data 0x000.
									// Voutput = (5 V) * DAC_VOLTS_PER_OCTAVE * (k/12)


//-----------------------------------------------
// SHIFT REGISTER INPUT AND OUTPUT FUNCTIONS
//-----------------------------------------------


//-----------------------------------------------
// MASTER SHIFT REGISTER
// the following is a list of the pins on the master output shift register:
// the shift register shifts into bit 0 first. bits shift in the direction from 0 to 55.
// when shifting into the master output register, shift everything MSB.
// DAC_A is the DAC intended for outputting the CV for the lead keys (higher keys).
// DAC_B is the DAC intended for outputting the CV for the bass keys (lower  keys).
// DAC_A_0 is the least significant, DAC_A_11 is the most significant.
// the same MSB direction applies to DAC_B.
// when shifting data into the DACs, shift MSB.
// pins 24-48 are for direct gate control using the lower two octaves of the keyboard.
/*
0.		DAC_A_0					// these are the 12 digital outputs for the first (top) 12-bit DAC (DAC_A)
1.		DAC_A_1
2.		DAC_A_2
3.		DAC_A_3
4.		DAC_A_4
5.		DAC_A_5
6.		DAC_A_6
7.		DAC_A_7

8.		DAC_A_8
9.		DAC_A_9
10.		DAC_A_10
11.		DAC_A_11
12.		DAC_B_0					// these are the 12 digital outputs for the second (bottom) 12-bit DAC (DAC_B)
13.		DAC_B_1
14.		DAC_B_2
15.		DAC_B_3

16.		DAC_B_4
17.		DAC_B_5
18.		DAC_B_6
19.		DAC_B_7
20.		DAC_B_8
21.		DAC_B_9
22.		DAC_B_10
23.		DAC_B_11

24.		keys[0] bit 0			// these are output gate bits that correspond to the bottom two octaves of the keyboard (bottom 25 keys)
25.		keys[0] bit 1
26.		keys[0] bit 2
27.		keys[0] bit 3
28.		keys[0] bit 4
29.		keys[0] bit 5
30.		keys[0] bit 6
31.		keys[0] bit 7

32.		keys[1] bit 0
33.		keys[1] bit 1
34.		keys[1] bit 2
35.		keys[1] bit 3
36.		keys[1] bit 4
37.		keys[1] bit 5
38.		keys[1] bit 6
39.		keys[1] bit 7

40.		keys[2] bit 0
41.		keys[2] bit 1
42.		keys[2] bit 2
43.		keys[2] bit 3
44.		keys[2] bit 4
45.		keys[2] bit 5
46.		keys[2] bit 6
47.		keys[2] bit 7

48.		keys[3] bit 0
49.		gate for DAC_A			// this is the gate output for DAC_A
50.		gate for DAC_B			// this is the gate output for DAC_B
51.		 *unused*				// these are currently unused
52.		 *unused*
53.		 *unused*
54.		 *unused*
55.		 *unused*
*/
//-----------------------------------------------
// 16 BIT INPUT SHIFT REGISTER
// the following list denotes the bits of the 16 bit parallel-in serial-out shift register that is used to read 16 bits and their uses:
/*
0.	keyboard_oct_0
1.	keyboard_oct_1A
2.	keyboard_oct_1B
3.	keyboard_oct_2A
4.	keyboard_oct_2B
5.	keyboard_oct_3A
6.	keyboard_oct_3B
7.	keyboard_oct_4A
	
8.	keyboard_oct_4B
9.	keyboard_oct_5A
10.	keyboard_oct_5B
11.	calibration_enable (active high)
12.	
13.	
14.	
15.	

*/

// this function shifts out data to a serial-in-parallel-out shift register on some port.
// This function works on ports B, C, and D.
// This function was designed to be used on the ATmega328P microcontroller,
// but it will likely work on many other Ateml microcontrollers will similar port configurations.

// "port" is a character 'b', 'c', 'd', or 'B', 'C', or 'D'. This tells the function where to shift out.
// "clockPin" is the pin in "port" that will be used as the clock of the shift register. values 0-7 are acceptable.
// "dataPin"  is the pin in "port" that will be used as the data  of the shift register. values 0-7 are acceptable.
	// NOTE: This function will not set the DDRx registers.
	// The clockPin and dataPin of the port need to be configured as outputs before attempting to shift out data.
// bits is the number of bits that need to be shifted out.
// data is the data to be shifted out.
// bitDirection
void shift_out_328(char port, unsigned char clockPin, unsigned char dataPin, unsigned int bits, unsigned long int data, char bitDirection){
	
	unsigned int i;					// this is used to count through the bits
	long unsigned int mask = 1;		// this is used to selectively pick bits from "data" to shift out one at a time.
	
	// set up the initial state of the bit mask
	if(bitDirection == 'm' || bitDirection == 'M'){
		for(i=1; i<bits; i++)	// start the mask at 2^(bits - 1)
			mask <<= 1;
	}
	
	for(i=0; i<bits; i++){
			
		//set data bit high if needed
		if(data&mask){
			switch(port){
			case 'b':
			case 'B':
				PORTB |=  (1 << dataPin);
				break;
			case 'c':
			case 'C':
				PORTC |=  (1 << dataPin);
				break;
			case 'd':
			case 'D':
				PORTD |=  (1 << dataPin);
				break;
			default:
				break;
			}
		}
		// otherwise, set the data bit low
		else{
			switch(port){
				case 'b':
				case 'B':
					PORTB &= ~(1 << dataPin);
					break;
				case 'c':
				case 'C':
					PORTC &= ~(1 << dataPin);
					break;
				case 'd':
				case 'D':
					PORTD &= ~(1 << dataPin);
					break;
				default:
					break;
			}
		}
			
		//clock data into the register.
		switch(port){
			case 'b':
			case 'B':
				PORTB &= ~(1 << clockPin);
				PORTB |=  (1 << clockPin);
				break;
			case 'c':
			case 'C':
				PORTC &= ~(1 << clockPin);
				PORTC |=  (1 << clockPin);
				break;
			case 'd':
			case 'D':
				PORTD &= ~(1 << clockPin);
				PORTD |=  (1 << clockPin);
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





//-----------------------------------------------
// KEYBOARD STUFF
//-----------------------------------------------

#define KEYBOARD_HALF_OCTS			11	// this is how many half-octaves there are in the keyboard
#define KEYBOARD_KEYS_PER_HALF_OCT	6	// this is how many keys there are per half-octave.
// this may be overkill (because you can only add so many bits before
// running into resistor matching errors) but I would rather have
// too many bits than not enough.
#define KEYBOARD_TOTAL_KEYS			61	// this is how many keys there are on the keyboard that we will be using
#define KEYBOARD_BYTES_NEEDED		8	// THIS IS HOW MANY BYTES ARE NEEDED TO STORE ALL THE GATE SIGNALS FOR ALL OF THE KEYS

// this variables keeps track of all of the gate signals.
// if a bit is 0, then the key is up (inactive)
// if a bit is 1, then the key is pressed
// keys[0]&0x01=	C0
// keys[0]&0x02=	Db0
// keys[0]&0x04=	D0
// keys[0]&0x08=	Eb0
// ...
// keys[1]&0x01=	Ab0
// keys[1]&0x02=	A0
// ...
unsigned char keys[KEYBOARD_BYTES_NEEDED];
// this is a number from 0 to (KEYBOARD_TOTAL_KEYS-1) that indicated the  lowest key being pressed
// if keyLowest  is -1, there is no key being pressed.
int keyLowest=	-1;
// this is a number from 0 to (KEYBOARD_TOTAL_KEYS-1) that indicated the highest key being pressed
// if keyHighest is -1, there is no key being pressed.
int keyHighest=	-1;


// this will update the keyLowest and keyHighest variables
void keyboard_update_highest_lowest(){
	keyLowest = -1;
	keyHighest = -1;
	unsigned int k;
	for(k=0; k<KEYBOARD_TOTAL_KEYS; k++){
		if(keys[k/8]&(1<<k%8)){				// check the key gate bit
			if(keyLowest == -1) keyLowest = k;
			keyHighest = k;
		}
	}
}


// This will take a given key, k, and some halfOctave, and index into the keys[] byte array.
// The bit will be set high if bit == 1.
// The bit will be set low  if bit == 0.
// k between 0 and 5 (inclusive bounds)
// halfOctave between 0 and 10 (inclusive bounds)
// bit either 0 or 1
// k == 0	=>		C /Gb
// k == 1	=>		Db/G
// k == 2	=>		D /Ab
// k == 3	=>		Eb/A
// k == 4	=>		E /Bb
// k == 5	=>		F /B
void keyboard_process(unsigned char k, long unsigned int halfOctaveData){
	
	int o; // this is used to index through the half-octaves
	
	// absoluteKey is 0 to 60. 0=C0, 1=Db0, 2=D0, ... 60=C5
	unsigned char absoluteKey;
	// this keeps track of whether or not the current key is pressed
	bool pressed;
	
	// if the key is C or Gb, then make it 6 instead of 0.
	// This is all part of decoding the keyboard into a number from 0 to 60.
	if(k== 0) k = 6;
	
	for(o=0; o<KEYBOARD_HALF_OCTS; o++){
		// if this specific key is pressed, then pressed=1. otherwise, pressed = 0.
		pressed = (  ( halfOctaveData & (((long unsigned int)1)<<o) ) != 0);
		// calculate the absolute key
		absoluteKey = k + 6*(o-1);
		
		// update the keys[] array that contains all of the gate signals.
		if(pressed){
			// set the right bit high
			keys[absoluteKey>>3] |=  (1 << (absoluteKey%8));
		}
		else{
			// set the right bit low
			keys[absoluteKey>>3] &= ~(1 << (absoluteKey%8));
		}
	}
}


// this shifts data in from the parallel-to-serial 16bit shift register (first 11 bits are used for the 11 rows).
// works on PORTC
unsigned int shift_in_keys(){
	
	// turn off input shift register clock
	PORTC &= ~(1 << p_shift_in_clock);
	
	// grab (clock in) all 16 bits to the register.
	PORTC &= ~(1 << p_shift_in_grab);
	PORTC |=  (1 << p_shift_in_grab);
	
	
	unsigned int i;
	unsigned char temp_input_buffer;
	unsigned int data_in = 0;
	
	for(i=0; i<16; i++){
		
		// read pin c
		temp_input_buffer = PINC;
		if( temp_input_buffer & (1 << p_shift_in_data) )
		data_in |= (1 << i);
		
		// clock the next bit in
		PORTC &= ~(1 << p_shift_in_clock);
		PORTC |=  (1 << p_shift_in_clock);
	}
	
	// return the data that was read (16 bits) first bit is the least significant.
	return data_in;
}




// this function will update the values of the following variables:
//		- keyLowest
//		- keyHighest
//		- keys
// this function will return the most recent 16 bits that it shifted in to read the keyboard.
// Because there are only 11 keys that the keyboard uses, there are 5 extra digital inputs that can be read.
// bits 0-10 (the first eleven bits) are for the keyboard.
// bit_11 (the twelfth bit) is a digital signal input that will indicate if the user wants to be in calibration mode.
// if bit_11 is high, the microcontroller will switch to calibration mode.
// if bit_11 is low, the microcontroller will be out of calibration mode.
unsigned int keyboard_grab(){
	
	
	int k;									// this is used to index through the keys
	unsigned int registerData;				// this is 16 bits that stores the value of the 16-bit parallel-in serial-out shift register.
	
	PORTD |=  (1 << p_shift_keys_data);		// set keys shift register data bit to 1
	
	PORTD &= ~(1 << p_shift_keys_clock);	// clock out a 1
	PORTD |=  (1 << p_shift_keys_clock);
	
	PORTD &=  ~(1 << p_shift_keys_data);	// set keys shift register data bit to 0
	
	// loop through all 6 keys (per half octave)
	// C/Gb first,
	// Db/G second,
	// D/Ab third,
	// Eb/A fourth,
	// E/Bb fifth,
	// F/B  sixth.
	for(k=0; k<KEYBOARD_KEYS_PER_HALF_OCT; k++){
		// process the keyboard for a given key input (C and Gb, Db and G, D and Ab, etc...)
		registerData = shift_in_keys();
		keyboard_process(k, registerData);
		PORTD &= ~(1 << p_shift_keys_clock);	// clock the data further down the matrix.
		PORTD |=  (1 << p_shift_keys_clock);	// this will turn on the next key input
	}
	
	// update the highest and lowest keyboard keys currently pressed
	keyboard_update_highest_lowest();
	return registerData;
}



// this updates the two DACs, the gate outputs, and the two DACs' respective gate outputs
void update_all_outputs(){
	
	// shift in most-significant-bit-first configuration.
	
	// shift out gate information.
	shift_out_328(  'D', p_shift_master_clock, p_shift_master_data, 2, ( (keyLowest != -1)<<1 ) | (keyHighest != -1), 'M'  );
	
	
	// for the gate key[] outputs, the bits are inverted because the outputs of the hardware are inverted. Thus, two inversions cancel each other.
	
	// shift out the highest 9 bits of the key[] gate outputs.
	shift_out_328(  'D', p_shift_master_clock, p_shift_master_data, 9, ~((((unsigned int)keys[3]&0x01)<<8)|keys[2]), 'M'  );
	// shift out the bottom 16 bits of the key[] gate outputs.
	shift_out_328(  'D', p_shift_master_clock, p_shift_master_data, 16, ~((((unsigned int)keys[1])<<8)|keys[0]), 'M'  );
	
	
	// calculate the DAC_A bits.
	unsigned long int DAC_A = (keyHighest != -1)*((((unsigned int)1) << 12)-1)*(keyHighest/((float)(KEYBOARD_TOTAL_KEYS-1)));
	unsigned long int DAC_B = (keyLowest != -1)*((((unsigned int)1) << 12)-1)*(keyLowest /((float)(KEYBOARD_TOTAL_KEYS-1)));
	// shift out the DAC information most-significant-bit-first.
	shift_out_328(  'D', p_shift_master_clock, p_shift_master_data, 2*DAC_BIT_LENGTH, (DAC_B << 12)|DAC_A, 'M'  );
	
	// clock the update pin and push everything through the buffer.
	PORTD &= ~(1 << p_shift_master_update);
	PORTD |=  (1 << p_shift_master_update);
	
}



// this function will write the sequence 0, 1, 2, 3, 4, ... (2^12 - 1) one at a time to the DACs.
// DAC_A will always be one step behind DAC_B in the sequence because of how the hardware is layed out.
// this sequence will be written to the DACs as fast as the microcontroller possibly can.
// the purpose of this function is to allow the user to calibrate the DAC
// this will be possible with the aid of an oscilloscope to visually identify and correct the discrete level changes

#define CALIBRATION_STOP_VALUE ((unsigned int)1 << 12)

// calibrationResolution is from 0 to 11.
// This variable describes how fine of steps are.
void calibration_mode(int calibrationResolution){
	
	// this variable keeps track of the data that needs to be written to the DAC
	unsigned int k;
	
	for(k=0; k < KEYBOARD_TOTAL_KEYS; k++){
		
		// shift out the current calibration data
		shift_out_328( 'D', p_shift_master_clock, p_shift_master_data, DAC_BIT_LENGTH, ((unsigned int)(( (1<<DAC_BIT_LENGTH)-1) * (k/((float)KEYBOARD_TOTAL_KEYS)))/((float)(1<<calibrationResolution)) + 0.5f), 'M' );
		
		// clock the update pin and push everything through the buffer.
		PORTD &= ~(1 << p_shift_master_update);
		PORTD |=  (1 << p_shift_master_update);
	}
}


//-----------------------------------------------
// MAIN CODE SECTION
//-----------------------------------------------

int main(void)
{
	
	// initialize the PORT input/output settings
	
	// outputs of the input shift register
	DDRC |=  (1 << p_shift_in_clock) | (1 << p_shift_in_grab);
	// input of the input shift register
	DDRC &= ~(1 << p_shift_in_data);
	
	// outputs of the output keys shift register
	DDRD |=  (1 << p_shift_keys_clock) | (1 << p_shift_keys_data);
	
	// outputs of the output master shift register
	DDRD |=  (1 << p_shift_master_clock) | (1 << p_shift_master_data) | (1 << p_shift_master_update);
	
	// turn off output keys shift register  clock and data pins
	PORTD &= ~(1 << p_shift_keys_clock);
	PORTD &= ~(1 << p_shift_keys_data);
	
	// turn off output DAC shift register clock and data pins
	PORTD &= ~(1 << p_shift_master_clock);
	PORTD &= ~(1 << p_shift_master_data);
	
	// turn off input shift register clock
	PORTC &= ~(1 << p_shift_in_clock);
	// turn on  input shift register grab (disabled)
	PORTC |=  (1 << p_shift_in_grab);
	
	// this is the variable that will store the value read from the 16-bit shift-in register.
	// this is used for thing like enabling/disabling calibration mode.
	unsigned int register16Data;
	// calibration resolution keeps track of how fine the calibration mode is set to (at a constant number of steps for a constant frequency)
	// this value can range from 0 to 11.
	int calibrationResolution = 0;
	// this keeps track of whether of not the last time through the main while(1) loop was spent in calibration mode
	bool calibratedLastTime = true;
	
    while(1)
    {
		
		// update the keyboard variables keys[], keyHighest, and keyLowest
		register16Data = keyboard_grab();
		
		// if calibrate mode is enabled,
		if(  register16Data & (  ((unsigned int)1) << b_calibrate_enable)  ){
			// run the calibration sequence and allow the user to manually calibrate the machine
			calibration_mode(calibrationResolution);
			// if this is the beginning of a calibration session,
			if(!calibratedLastTime){
				// then increase the calibration count
				calibrationResolution++;
				// and if the calibration resolution is too high, the resolution rolls over to the coarsest setting
				if(calibrationResolution >= DAC_BIT_LENGTH) calibrationResolution = 0;
			}
			// record that calibration has taken place
			calibratedLastTime = true;
		}
		// otherwise,
		else{
			// update all of the outputs off of the master shift register
			update_all_outputs();
			// record that calibration has not taken place
			calibratedLastTime = false;
		}
		
    }
}