/*
 * Testing_LED_Driver.c
 *
 * Created: 3/28/2014 3:46:00 PM
 *  Author: Ryan
 * The purpose of this program is just to test some LED driving capabilities.
 * I want to be able to update a 16x16 LED matrix at 30 frames per second.
 * Also, the display cannot be much dimmer than when normally driven by a steady current.
 */ 


#define F_CPU 8000000UL			// define the clock speed as 8MHz. This clock speed is used in the delay.h header file. This does not actually change the clock speed of the device.
#include <avr/io.h>				// this is the basic Input output of AVR microcontrollers I think
#include <util/delay.h>			// this allows me to use the delay functions
#include <avr/interrupt.h>		// this allows me to use interrupt functions

// these are the demultiplexer control pins
#define p_demux0 PORTD1
#define p_demux1 PORTD1
#define p_demux2 PORTD2
#define p_demux3 PORTD3

// these are the shift register control pins
#define p_shift_data PORTD4
#define p_shift_clock PORTD5

// This pin will enable or disable the LED matrix. 1=on 0=off
#define p_matrix_enable PORTD6

//------------------------------------------------------
// Global Variables
//------------------------------------------------------

// this is the variable that analog-to-digital conversions dumps into.
// this interger is "volatile" because data will be dumped into it during interrupt calls.
volatile int analogReturn;

//------------------------------------------------------
// Timers
//------------------------------------------------------

void timer1_init(){
	
	TCCR0 |= (1 << CS01);		// timer has a prescaler of 8.
	TCCR0 |= (1 << WGM01);		// timer works in CTC mode. (Clear Timer on Compare Match).
	TCNT1 = TCNT1L = 0;			// reset the timer count.
	OCR0 = 0x40;					// set the output compare register.
								// this register determines the speed at which the columns/rows are cycled through.
	TIMSK |= (1 << OCIE0);		// enable an interrupt upon output compare match for the timer.
	
}

// this function handles when the timer0 has a compare match.
ISR(TIMER0_COMP_vect){
	/*
	PORTD ^= (1 << PORTD7);		// toggle pin 7 of portd.
	_delay_us(2);				// wait for a bit.
	PORTD ^= (1 << PORTD7);		// toggle pin 7 of portd.
	*/
}

//------------------------------------------------------
// Analog to Digital stuff
//------------------------------------------------------

// initialize all of the Analog-to-Digital Subsystems.
// This function prepares the ADC subsystem to read from any one of the 8 ADC inputs with the interrupt enbaled.
// The result from the conversion will be stored in the AnalogReturn variable.
void init_ADC(){
	ADCSRA |= (1 << ADEN); // enable the ADC.
	
	ADMUX &= !(1 << MUX4);		// turn off mux pins 4 and 3. These are only used for differential analog inputs. lets disregard that for now.
	ADMUX &= !(1 << MUX3);
	
	ADCSRA |= (1 << ADIE);		// enable analog read complete interrupts.
	
}

// this function will read the analog value at a pin and spit it out into the analogReturn variable.
// note that pin can only be 0 through 7
void analog_read_0(){
	
	/*
	// select the correct pin
	if(pin & 4){
		ADMUX &= !(1 << MUX2);
	}
	if(pin & 2){
		ADMUX &= !(1 << MUX1);
	}
	if(pin & 1){
		ADMUX &= !(1 << MUX0);
	}
	*/
	
	
	ADCSRA |= (1 << ADSC);		// start converting analog to digital.
	
}


// This function handles the analog-to-digital conversion complete interrupt.
ISR(ADC_vect){
	
	analogReturn = 0 | ADCL | (ADCH << 8);	// combine the two bytes to form a 10-bit number for the analogReturn value.
	
}




int main(void)
{
	//timer1_init();		// initialize timer1 settings. This is the timer that controls the speed of the display
	/*
	DDRD = 0xff; // set PORTD as an output port
	PORTD = 0; // turn off all PORTD pins
	
	PORTD |= (1<<PORTD0);
	PORTD |= (1<<PORTD7);
	while(1){
		PORTD ^= (1 <<PORTD7);
		_delay_us(1400);		// wait for 14 us
		PORTD ^= (1 <<PORTD7);
		_delay_us(200);		// wait for 14 us
	}
	*/
	
	
	sei();				// enable global interrupts.
	
	init_ADC();			// initialize the ADC subsystems
	DDRD = 0xff;		// set PORTD as an output port
	DDRC = 0x03;		// set the first two pins of portc as outputs.
	DDRA = 0;			// set PORTA to inputs
	PORTD = 0;			// turn off all PORTD pins
	
	// main program infinite loop
	while(1){
		
		analogReturn = -1;			// set analogReturn to invalid value
		
		analog_read_0();				// start ADC conversion.
		
		while(analogReturn == -1);	// wait until analgReturn is written to by the ADC interrupt handler
		
		PORTD = analogReturn/4;							// PORTD is the higher eight bits of the 10 bit number. bits 9-2.
		
		if(analogReturn & 2) PORTC |= (1 << PORTC1);	// PORTC1 is the second lowest bit in the 10 bit number. bit 1.
		else PORTC &= !(1 << PORTC1);
		
		if(analogReturn & 1) PORTC |= (1 << PORTC0);	// PORTC0 is the lowest bit in the 10 bit number. bit 0.
		else PORTC &= !(1 << PORTC0);
		
	}
}



