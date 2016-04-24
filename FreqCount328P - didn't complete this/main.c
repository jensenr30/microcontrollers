/*
 * main.c
 * FreqCount
 *
 * Created: 2016-01-15
 *  Author: Ryan Jensen
 * 
 * This program is designed to count the frequency of a digital signal (0 to 5 V).
 * 
 * 
 * This program is licensed under the following license:
 * Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
 * Go ahead and tweak it, so long as you follow the simple rules outlined in the above license.
 * for more information, go to http://creativecommons.org/licenses/by-nc-sa/4.0/
 */



//=================================================================
// included header files
//=================================================================

#define F_CPU (20000000)		// define the clock speed to be 20 MHz.
#include <avr/io.h>				// standard input/output functions
#include <util/delay.h>			// include the use of the delay functions _delay_ms() and _delay_us()
#include <avr/interrupt.h>		// this allows me to use interrupt functions
#include "shift_out_328_PORTD.h"		// this gives us the function we need to utilize a shift register to shift OUT data.

//=================================================================
// pin definitions
//=================================================================

// PORTB
#define p_freq_in			PORTB0		// Input digital signal (0 to 5V). We try to measure the frequency of this one.



// PORTA
// OUTPUT SHIFT REGISTER pins:
#define p_SR_data			PORTD5		// this pin holds the data that must be clocked into the shift register
#define p_SR_RCK			PORTD6		// this pin updates the outputs of the shift register
#define p_SR_SCK			PORTD7		// this pin clocks data into the shift register

// this is how many seconds are contained in one clock cycle
#define clock_period_sec ( 1/(double)F_CPU )
// this is how many seconds are contained in one overflow value (if the overflow value is 1, this is how many seconds it took to reach that overflow value of 1).
#define overflow_period_sec ( (0x10000) / (double)F_CPU )

// this tells us how many clock cycles (from the 20 MHz clock) must pass before we can make a measurement with no more than 1 ppm error.
// the maximum rate at which this will measure frequency is 40 Hz (it will take a frequency measurement no more than 40 times every second).
#define min_clock_cycles_1ppm ( (uint32_t)500000 )

// this is the max value of timer1
#define timer1_TOP ((uint16_t)0xffff)


//=================================================================
// function definitions
//=================================================================

// input/output functions are defined here
#define low(port, pin)		port &=	~(1<<pin)			// this sets a certain pin low.
#define high(port, pin)		port |=	 (1<<pin)			// this sets a certain pin high.
#define toggle(port, pin)	port ^=  (1<<pin)			// this toggles a certain pin.
#define set_input(portdir, pin) portdir &= ~(1<<pin)	// this sets a certain pin as an input.
#define set_output(portdir, pin) portdir |= (1<<pin)	// this sets a certain pin as an output.


//=================================================================
// variable declaration and numerical definitions
//=================================================================

// this keeps track of whether or not the counter has been triggered for the first time.
//volatile uint8_t triggered = 0;
// this keeps track of how many times Timer1 has overflowed.
// with a clock frequency of 20 MHz, a 16-bit timer, and a 32-bit overflow counter, this program should be able to count periods as long as 162 days.
uint32_t overflows = 0;
// this records how many cycles (periods) have occurred on the p_freq_in pin.
uint16_t freq_in_cycles = 0;

// these two variables record the state of the timer and overflow counter at the moment when 
//uint16_t OFF_time_timer = 12345;
//volatile uint32_t OFF_time_overflows = 0;
// this is updated with measurements of the input frequency
double freq_in_measurement_Hz = 0;
// this is updated with measurements of the duty cycle (%)
//volatile double freq_in_duty_cycle = 0;

// this is a dummy variable used to debug the shift register
//uint32_t data;

uint8_t sevenseg[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};


// this keeps track of which digit is being displayed (The Persistance of vision technique is used. Only one 7-segment display is lit at a time.)
uint8_t digit = 0;
// this is the data that is displayed
// default data is all underlines
uint8_t disp_data[6];// = {0x08,0x08,0x08,0x08,0x08,0x08};


//=================================================================
// function declarations
//=================================================================

/*
// this starts timer0
void init_timer0()
{
	// select a clock source that runs at one eighth the frequency as the I/O clock
	TCCR0B |= (1<<CS00);
	// this sets up PWM.
	TCCR0A |= (1<<WGM01)|(1<<WGM00);
	// this sets up pin toggling (non-inverted PWM)
	TCCR0A |= (1<<COM0A1);
	// this is the default setting for PWM (50 % duty cycle)
	OCR0A = 55;
}
*/


// this gets timer1 up and running
// timer one will take care keeping track of time for measuring the line position.
void init_timer1()
{
	TCCR1A &= ~(1<<COM1A0) & ~(1<<COM1A1) & ~(1<<COM1B0) & ~(1<<COM1B0) & ~(1<<WGM11) & ~(1<<WGM10);
	TCCR1B |= (1<<CS10);			// timer1 is clocked from the main clock (20 MHz)
	
	//TCNT1H = 0;					// reset the timer1 count (both the high and low bytes)
	//TCNT1L = 0;					// "
	
	//OCR1AH = 0xff;				// set high byte of the output compare register
	//OCR1AL = 0xff;				// set low  byte of the output compare register
	
	TIMSK1 |= (1<<TOIE1);			// enable overflow interrupt
	
}


// enable and configure the input pin interrupts.
void init_input_interrupts()
{
	// enable interrupts for port B pins (PCINT 11:8).
	PCICR |= (1<<PCIE0);
	
	// enable interrupts for PCINT 10 specifically.
	// This corresponds to PORTB PB0 (pin 14) - on ATmega328P DIP-28 packages.
	// this interrupt will be triggered on a logical state change (rising/falling edge).
	PCMSK0 |= (1<<PCINT0);
}


// this handles when either of the two input pins changes states
ISR(PCINT0_vect)
{
	//-----------------------------------------------------------------
	// First things first: read port and record time. These are time-crucial.
	//-----------------------------------------------------------------
	// read data on PORTB
	uint8_t portBdata = PINB;
	
	// record the current time (from timer1)
	// this is useful when doing calculations on the line position
	// read from low byte then add the high byte (low byte first)
	uint16_t currentTimer1 = TCNT1L;
	currentTimer1 += TCNT1H<<8;
	
	//-----------------------------------------------------------------
	// Next up: figure out what the state of the pin is
	//-----------------------------------------------------------------
	
	// grab the state of the freq_in pin.
	uint8_t freq_in_state = ( (portBdata & (1<<p_freq_in))  >> p_freq_in );
	
	// if the signal went low,
	if( !freq_in_state )
	{
		// record that the input signal DID, in fact, have a (possibly another) falling edge.
		freq_in_cycles++;
		
		// if the current sample has been going for at least the number of cycles needed to get 1 ppm resolution,
		if( ((uint32_t)overflows<<16) + (uint16_t)currentTimer1 >= min_clock_cycles_1ppm)
		{	
			// start your next sample by resetting Timer1.
			TCNT1H = 0;						// reset the timer1 count (both the high and low bytes)
			TCNT1L = 0;						// "
				
			// calculate the period over which this sampled was performed.
			double period_sec =  (currentTimer1*clock_period_sec) + (overflows        *overflow_period_sec);
			// record the frequency
			freq_in_measurement_Hz = freq_in_cycles/period_sec;
			// calculate the OFF_time in seconds
			//double OFF_time_sec = (OFF_time_timer*clock_period_sec) + (OFF_time_overflows*overflow_period_sec);
			// record the duty cycle
			//freq_in_duty_cycle = OFF_time_sec/period_sec;
				
			// reset all variables (Timer1 was already reset above)
			freq_in_cycles = 0;
			overflows = 0;
			//OFF_time_timer = 0;
			//OFF_time_overflows = 0;
		}
		
	}
	else
	{
		
		//-----------------------------------------------------------------
		// Or record the midpoint
		//-----------------------------------------------------------------
		
		// record when the digital signal went low
		//OFF_time_timer = currentTimer1;
		//OFF_time_overflows = overflows;
		
	}
	
	
}


// this function handles when the timer1 has a compare match.
// mainly, this function simply handles changing the state of the device between warmup, active, and waiting
ISR(TIMER1_OVF_vect)
{
	// reset the timer 1 count (high byte first)
	//TCNT1H = 0;
	//TCNT1L = 0;
	// increment the counter that keeps track of how many times Timer1 has overflowed.
	overflows++;
}



// this is where the program starts
int main(void)
{
	 // PORTB
	 // set individual pin I/O directions
	set_input(DDRB, p_freq_in);
	
	
	// PORTA
	// set individual pin I/O directions
	set_output(DDRD, p_SR_data);
	set_output(DDRD, p_SR_SCK);
	set_output(DDRD, p_SR_RCK);
	// set the intial values of these pins
	//low(PORTA, p_SR_data);
	//low(PORTA, p_SR_SCK);
	//low(PORTA, p_SR_RCK);
	
	// set up timers and interrupts
	sei();								// enable global interrupts
	init_timer1();						// initialize timer1
	init_input_interrupts();			// initialize input interrupts
	
	// the main() function only serves to setup the program. everything happens based on interrupts, so our work in the main() function is done. we can kick back
	while(1)
    {
		// check if we are starting a new frame (a new set of six display digits)
		if(digit >= 6)
		{
			digit = 0;
			double decade;
			
			// grab the current frequency measurement
			double currentFreq = freq_in_measurement_Hz;
			
			// records which significant figure of the number we are currently processing
			// default is a bogus value
			int8_t sigfig = 99;
			for(decade = 10e6; decade >= 1e-6; decade/=10.0)
			{
				// if you have found the first significant digit,
				if(currentFreq >= decade && sigfig == 99)
				{
					// start processing the number into 
					sigfig = 0;
				}
				// if you have found at least the first significant digit,
				if(sigfig <= 5)
				{
					// record what this digit was by putting it in the disp_data
					disp_data[sigfig] = (uint8_t)(currentFreq/decade);
					// subtract the digit that has already been accounted for
					currentFreq -= (double)disp_data[sigfig]*decade;
					// replace the digit with the data that will produce a digit on the seven segment display
					disp_data[sigfig] = sevenseg[disp_data[sigfig]];
					// if this is the ones place,
					if(decade == 1)
					{
						// add the decimal place
						disp_data[sigfig] |= 0x80;
					}
					// increment the sigfig
					sigfig++;
				}
				
			}
		}
		
		// debugging the shift register chain
		//if(data) data <<= 1;
		//if(data >= ((uint32_t)1 << 6)) data = 1;
		//if(data == 0) data = 1;
		
		// build data and shift it into the registers
		shift_out_328_PORTD(p_SR_SCK, p_SR_data, 16, ((uint16_t)1<<(digit+8))|(uint16_t)disp_data[digit], 'm');
		
		// update the output registers
		low(PORTD,p_SR_RCK);
		high(PORTD,p_SR_RCK);
		_delay_ms(2);
		
		// We have just displayed a digit. It is time to 
		digit++;
	}
}


// fall
// right
// out of the 
// sky
