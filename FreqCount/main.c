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


//=================================================================
// pin definitions
//=================================================================

// PORTB
/*
#define p_sweep				PORTB1		// this is the square wave signal that tells us what direction the laser is sweeping.
										// The rising/falling edges tells us when it changes its sweep direction.
*/
#define p_freq_in			PORTB2		// Input digital signal (0 to 5V). We try to measure the frequency of this one.

// PORTA
/*
#define p_linePosition		PORTA3		// this pin shows (in time) roughly where the BSI interprets the line to exist.
#define p_dataReady			PORTA7		// this tells the master microcontroller that the analog signal from p_line is ready to be read.
*/

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

// this is used to capture the state of the PORTB register.
volatile uint8_t portBdata = 0;

// this is the max value of timer1
#define timer1_TOP ((uint16_t)0xffff)

// this keeps track of how many times Timer1 has overflowed.
// with a clock frequency of 20 MHz, a 16-bit timer, and a 32-bit overflow counter, this program should be able to count periods as long as 162 days.
volatile uint32_t overflows = 0;
// this is how many seconds are contained in one clock cycle
double clock_period_sec = 1/(double)F_CPU;
// this is how many seconds are contained in one overflow value (if the overflow value is 1, this is how many seconds it took to reach that overflow value of 1).
double overflow_period_sec = (0x10000) / (double)F_CPU;

// this records how many cycles (periods) have occurred on the p_freq_in pin.
volatile uint32_t freq_in_cycles = 0;
// this keeps track of whether or not the counter has been triggered for the first time.
volatile uint8_t triggered = 0;

// this tells us how many clock cycles (from the 20 MHz clock) must pass before we can make a measurement with no more than 1 ppm error.
// the maximum rate at which this will measure frequency is 40 Hz (it will take a frequency measurement no more than 40 times every second).
static uint32_t min_clock_cycles_1ppm = 1e6/2;
// these two variables record the state of the timer and overflow counter at the moment when 
volatile uint32_t ON_time_overflows = 0;
volatile uint16_t ON_time_timer = 12345;
// this is updated with measurements of the input frequency
volatile double freq_in_measurement_Hz = 0;
// this is updated with measurements of the duty cycle (%)
volatile double freq_in_duty_cycle = 0;

//=================================================================
// function declarations
//=================================================================

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


// this gets timer1 up and running
// timer one will take care keeping track of time for measuring the line position.
void init_timer1()
{
	
	TCCR1B |= (1<<CS10);			// timer1 is clokced from the main clock (20 MHz)
	
	TCNT1H = 0;						// reset the timer1 count (both the high and low bytes)
	TCNT1L = 0;						// "
	
	OCR1AH = 0xff;					// set initial high byte of the output compare register
	OCR1AL = 0xff;					// set initial low  byte of the output compare register
	
	TIMSK1 |= (1<<OCIE1A);			// enable the output-compare interrupt for register A
	
}


// this handles when either of the two input pins changes states
ISR(PCINT1_vect)
{
	//-----------------------------------------------------------------
	// First things first: read port and record time. These are time-crucial.
	//-----------------------------------------------------------------
	// read data on PORTB
	portBdata = PINB;
	
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
	
	// if the signal went high,
	if( freq_in_state )
	{
		
		// if the p_freq_in pin has gone from high to low before (has triggered before) then we can make an acurate measurement.
		if( triggered )
		{
			// if the current sample has been going for at least the number of cycles needed to get 1 ppm resolution,
			if( (overflows<<16) + currentTimer1 >= min_clock_cycles_1ppm)
			{	
				// start your next sample by resetting Timer1.
				TCNT1H = 0;						// reset the timer1 count (both the high and low bytes)
				TCNT1L = 0;						// "
				
				// calculate the period over which this sampled was performed.
				double period_sec =  (currentTimer1*clock_period_sec) + (overflows        *overflow_period_sec);
				// record the frequency
				freq_in_measurement_Hz = freq_in_cycles/period_sec;
				// calculate the ON_time in seconds
				double ON_time_sec = (ON_time_timer*clock_period_sec) + (ON_time_overflows*overflow_period_sec);
				// record the duty cycle
				freq_in_duty_cycle = ON_time_sec/period_sec;
			}
			// if it is not yet time to measure the frequency,
			else
			{
				// record that the input signal DID, in fact, have another rising edge.
				freq_in_cycles++;
			}
		}
		// if p_freq_in has never gone high before,  the device was probably just powered up.
		else
		{
			 // record this as the first trigger.
			triggered = 1;
		}
		
	}
	else
	{
		
		//-----------------------------------------------------------------
		// Or record the midpoint
		//-----------------------------------------------------------------
		
		// record when the digital signal went low
		ON_time_timer = currentTimer1;
		ON_time_overflows = overflows;
		
	}
	
	
}


// this function handles when the timer1 has a compare match.
// mainly, this function simply handles changing the state of the device between warmup, active, and waiting
ISR(TIM1_COMPA_vect)
{
	// reset the timer 1 count (high byte first)
	//TCNT1H = 0;
	//TCNT1L = 0;
	// increment the counter that keeps track of how many times Timer1 has overflowed.
	overflows++;
}


// enable and configure the input pin interrupts.
void init_input_interrupts()
{
	// enable interrupts for port B pins (PCINT 11:8).
	GIMSK |= (1<<PCIE1);
	
	// enable interrupts for PCINT 10 specifically.
	// This corresponds to PORTB PB2 - ATtiny24A DIP package pin5.
	// this interrupt will be triggered on a logical state change (rising/falling edge).
	PCMSK1 |= (1<<PCINT10);
}


// this is where the program starts
int main(void)
{
	 // PORTB
	 // set individual pin I/O directions
	set_input(DDRB, p_freq_in);
	
	/*
	// PORTA
	// set individual pin I/O directions
	set_output(DDRA, p_linePosition);
	set_output(DDRA, p_dataReady);
	// initial states
	low(PORTA, p_linePosition);
	low(PORTA, p_dataReady);
	*/
	
	// set up timers and interrupts
	sei();								// enable global interrupts
	init_timer1();						// initialize timer1
	init_input_interrupts();			// initialize input interrupts
	
	// the main() function only serves to setup the program. everything happens based on interrupts, so our work in the main() function is done. we can kick back
	while(1)
    {
		// You do nothing!
		// As if the solution to all your problems will just fall right out of the sky!
	}
}


// fall
// right
// out of the 
// sky
