/*
 * Digital_Oscilloscope.c
 *
 * Created:			2014-04-15
 * Last Edited:		2014-04-25
 * Author:			Ryan Jensen
 * 
 * The purpose of this program is to drive a 16x16 LED matrix to act as a digital oscilloscope.
 * Details of the design are provided below
 */ 



/*------------------------------------------------------
// PROGRAM DETAILS
//------------------------------------------------------

The 16x16 LED matrix is controlled by a fairly involved circuit.
The ATmega328P talks to the display through 7 pins:

	- shift data
	- shift clock (rising edge)
	- shift buffer update (rising edge)
	- demultiplexer 1
	- demultiplexer 2
	- demultiplexer 3
	- demultiplexer enable
	
the demultiplexer pins turn on one of the 8 rows.
There are (technically) 16 rows, but the rows are grouped together in such a way as to make time multiplexing feasible.
The rows are numbered as such:

					 TOP
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 LEFT	0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7   RIGHT
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
		0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
					BOTTOM

The shift register is 32 bits deep.
It has a buffered output that is updated on rising edges.
The diagram below shows where the shift register outputs its data (in hexadecimal).
00 is the first output (shallowest in the shift register)
1f is the last output  (deepest in the shift register)


							  TOP
		00 00 00 00 00 00 00 00 10 10 10 10 10 10 10 10 
		01 01 01 01 01 01 01 01 11 11 11 11 11 11 11 11 
		02 02 02 02 02 02 02 02 12 12 12 12 12 12 12 12 
		03 03 03 03 03 03 03 03 13 13 13 13 13 13 13 13 
		04 04 04 04 04 04 04 04 14 14 14 14 14 14 14 14 
		05 05 05 05 05 05 05 05 15 15 15 15 15 15 15 15 
		06 06 06 06 06 06 06 06 16 16 16 16 16 16 16 16 
 LEFT   07 07 07 07 07 07 07 07 17 17 17 17 17 17 17 17   RIGHT
		08 08 08 08 08 08 08 08 18 18 18 18 18 18 18 18 
		09 09 09 09 09 09 09 09 19 19 19 19 19 19 19 19 
		0a 0a 0a 0a 0a 0a 0a 0a 1a 1a 1a 1a 1a 1a 1a 1a 
		0b 0b 0b 0b 0b 0b 0b 0b 1b 1b 1b 1b 1b 1b 1b 1b 
		0c 0c 0c 0c 0c 0c 0c 0c 1c 1c 1c 1c 1c 1c 1c 1c 
		0d 0d 0d 0d 0d 0d 0d 0d 1d 1d 1d 1d 1d 1d 1d 1d 
		0e 0e 0e 0e 0e 0e 0e 0e 1e 1e 1e 1e 1e 1e 1e 1e 
		0f 0f 0f 0f 0f 0f 0f 0f 1f 1f 1f 1f 1f 1f 1f 1f 
							 BOTTOM

*/


#define F_CPU 8000000UL			// define the clock speed as 8MHz. This clock speed is used in the delay.h header file. This does not actually change the clock speed of the device.
#include <avr/io.h>				// this has the proprietary IO functions for AVR microcontrollers. Atmel Studio will automagically include the right io header file specific to your device (for example, the iom328p.h header for the ATmega328P).
#include <util/delay.h>			// this enables the delay functions
#include <avr/interrupt.h>		// this enables the interrupt functions

//------------------------------------------------------
// PORTD
//------------------------------------------------------

// these are the demultiplexer control pins
#define p_demux0 PORTD0
#define p_demux1 PORTD1
#define p_demux2 PORTD2
#define p_demux_en PORTD3

// these are the shift register control pins. The shift register is 32 bits.
#define p_shift_data PORTD4
#define p_shift_clock PORTD5
// This pin will clock the data through the D-flip flops that are acting as a buffer on the output of the 32-bit shift register 
#define p_shift_update PORTD6

// this is the external trigger input
#define p_trigger PORTD7

// this is the input that, when clocked, will increase the speed of the display 2 fold
#define p_sec_per_div PORTB2


//------------------------------------------------------
// Global Variables
//------------------------------------------------------

volatile char column = 0;				// this keeps track of what column (0,1,2,3,4,5,6, or 7) that is currently being displayed
volatile char sector = 0;				// this keeps track of which half of the matrix is currently being displayed.
volatile char sweep = 0;				// this keeps track of whether or not we are currently sweeping

//------------------------------------------------------
// Timers
//------------------------------------------------------

void init_timer0(){
	
	TCCR0A |= (1 << WGM01);				// timer works in CTC mode. (Clear Timer on Compare Match).
	TCCR0B |=(1 << CS02)|(1 << CS00);	// set up timer prescaler
	TCNT0 = 0;							// reset the timer count.
	OCR0A = 0xff;						// set the output compare register.
										// this register determines the speed at which the columns/rows are cycled through.
	TIMSK0|= (1 << OCIE0A);				// enable an interrupt upon output compare match for value in timer0 register A
	
}

// this function handles when the timer0 has a compare match.
ISR(TIMER0_COMPA_vect){
	
	column++;							// increment column counter
	if(column >= 8){					// counter overflows at 8
		column = 0;						// reset to column 0
		if(sector == 1){
			sweep = 0;					// if you have reached the last part
			sector = 0;					// reset sector
		}
		else
			sector = 1;					// otherwise, increment the sector
	}
	
}


//------------------------------------------------------
// Analog to Digital stuff
//------------------------------------------------------

// initialize all of the Analog-to-Digital Subsystems.
// This function prepares the ADC subsystem to read from any one of the 8 ADC inputs with the interrupt enabaled.
// The result from the conversion will be stored in the AnalogReturn variable.
void init_ADC(){
	
	DDRC &= ~(1 << PORTC0);			// make PORTC0 an input for analog to digital conversions
	
	ADCSRA |=  (1 << ADPS2);		// Set ADC prescalar to 128
	ADCSRA |=  (1 << ADPS1);		// "
	ADCSRA |=  (1 << ADPS0);		// "
	
	ADMUX  |=  (1 << REFS0);		// Set ADC reference to AVCC
	
	ADCSRA |=  (1 << ADATE);		// Auto Trigger Enable
	
	ADCSRA |=  (1 << ADEN);			// enable the ADC
	ADCSRA |=  (1 << ADSC);			// start continuously converting analog to digital
	
}

//------------------------------------------------------
// LED Matrix Shift Out 32 Bits
//------------------------------------------------------

#define REG_SIZE 32
// this shifts out 32 bits
void shift_display_32(unsigned long int data){
	
	int j;										// this will be used to index through the rows
	long unsigned int mask = 1;					// this will be used to index through the data
	
	for(j=0; j<REG_SIZE; j++){
		if(data&mask)							// if the bit at this index is 1,
			PORTD |=  (1 << p_shift_data);		// set the data pin high
		else									// else,
			PORTD &= ~(1 << p_shift_data);		// set the data pint low
		
		PORTD &= ~(1 << p_shift_clock);			// clock data into the register
		PORTD |=  (1 << p_shift_clock);			// "
		
		mask = mask << 1;						// the mask gets shifted left 1 bit
	}
	
}

//------------------------------------------------------
// Program Start
//------------------------------------------------------

int main (void)
{
	//------------------------------------------------------
	// Initialize Things
	//------------------------------------------------------
	DDRD = 0x7f;							// lowest 7 bits are output bits
	DDRD &= ~(1 << p_trigger);				// PORTD7 is an input
	DDRB &= ~(1 <<p_sec_per_div);			// sec_per_div is an input.
	
	sei();									// enable global interrupts
	init_ADC();								// initialize the Analog to Digital Converter. It will continuously be taking samples from here on out
	init_timer0();							// set up timer0. This timer will be used to control the speed of the oscilloscope sweep
	
	unsigned long int analogSample = 0;		// this is what will hold the values of analog sample
	unsigned long int analogSampleLast = 0;	// this is the previous value of analog sample
	unsigned char trigSample = 0;			// this holds the data from the trigger digital input
	unsigned char trigSampleLast = 0;		// this holds the last trigSample
	char trigLevel = 8;						// when the analog input passes through this level or is at this level
	char trigSlope = 1;						// this tells us what slope we want. positive or negative. 1 = positive, -1 = negative
	char secPerDiv = 0, secPerDivLast = 0;	// this keeps track of the sec_per_div button.
	
	//------------------------------------------------------
	// main program loop
	//------------------------------------------------------
	while(1){
		
		secPerDiv = PINB;
		// check to see if the user pressed the "speed up" button
		if(secPerDiv&(1<<p_sec_per_div)){
			OCR0A = 0xff;
		}
		else
		{
			OCR0A = 0x08;
		}
		
		
		analogSampleLast = analogSample;	// store previous analog sample
		analogSample = ADCL >> 6;			// generate new 4 bit analog sample
		analogSample |= ADCH << 2;
		
		if(sweep == 0){		// if there is currently no sweep, start sweeping the cursor
			TCNT0 = 0;		// reset timer
			sweep = 1;		// start sweep
			column = 0;		// start at the first column
			sector = 0;		// start in the first sector
		}
		
		// if there is currently sweeping happening,
		if(sweep){
			
			// figure out what column will be on
		
			if(column&1)						// check bit 0
				PORTD |=  (1 << p_demux0);
			else
				PORTD &= ~(1 << p_demux0);
			if(column&2)						// check bit 1
				PORTD |=  (1 << p_demux1);
			else
				PORTD &= ~(1 << p_demux1);
			if(column&4)						// check bit 2
				PORTD |=  (1 << p_demux2);
			else
				PORTD &= ~(1 << p_demux2);
			
			// shift out the correct data to each of the 32 rows
			// this algorithm calculates the right data that needs to be shifted out
			// to display a cursor
			shift_display_32( ((unsigned long int)1)<<(analogSample+(16*(sector==0))) );
			PORTD &= ~(1 << p_shift_update);				// update shift register output buffer
			PORTD |=  (1 << p_shift_update);				// "
			PORTD |= (1 << p_demux_en);						// turn on display
			_delay_ms(1);									// display for some time
			PORTD &= ~(1 << p_demux_en);					// turn off display
			
		}
	}
	
	// end the program (the program will never reach this point).
	// this is kept here for the sake of completeness.
	return 0;
}