/*
 * Barcode_Scanner_Interpreter.c
 *
 * Created: 10/5/2014 3:35:17 PM
 *  Author: Ryan Jensen
 * 
 * This program is designed to interpret a barcode scanner.
 * This program is meant to be compiled using Atmel Studio for the ATtiny24A 8-bit microprocessor.
 * 
 * 
 * I don't have any information about the make or model of the barcode scanner.
 * I do, however, have a youtube video that shows the barcode scanner and where to buy it.
 * http://www.youtube.com/watch?v=MQRYXy-h3Gs
 *
 * This program is licensed under the following license:
 * Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)
 * Go ahead and tweak it, so long as you follow the simple rules outlined in the above license.
 * for more information, go to http://creativecommons.org/licenses/by-nc-sa/4.0/
 */



//=================================================================
// included header files
//=================================================================

#define F_CPU 8000000			// define the clock speed to be 8 MHz (this is done for the delay functions).
								// It is recommended to use the internal 8MHz RC oscillator.
#include <avr/io.h>				// standard input/output functions
#include <util/delay.h>			// include the use of the delay functions _delay_ms() and _delay_us()
#include <avr/interrupt.h>		// this allows me to use interrupt functions


//=================================================================
// pin definitions
//=================================================================

// PORTB
#define p_sweep				PORTB1		// this is the square wave signal that tells us what direction the laser is sweeping.
										// The rising/falling edges tells us when it changes its sweep direction.
#define p_data				PORTB0		// this pin gets the pulses from the barcode scanner indicating the black lines.
#define p_line				PORTB2		// this is the pin that is used to output a pwm signal that is converted into an analog voltage (via lowpass) to convey information about the position of the black tape in the optical range of the scanner.

// PORTA
#define p_linePosition		PORTA3		// this pin shows (in time) roughly where the BSI interprets the line to exist.
#define p_dataReady			PORTA7		// this tells the master microcontroller that the analog signal from p_line is ready to be read.


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

// if scanDirection is 0, then the barcode scanner laser is scanning left to right.  --->
// if scanDirection is 1, then the barcode scanner laser is scanning right to left.  <---
// this variable tells us what direction the barcode scanner is scanning.
volatile char sweepDirection = 0;	// by default, the scan direction is invalid.
// this holds the input data from PORTB
volatile char portBdata = 0;
// this keeps track of whether or not a valid pulse was found
volatile char foundValidPulse = 0;


// this is the minimum size a pulse must be to be considered a change from white to black (in microseconds)
#define PULSE_TIME_MIN 400
// this records where the line is (in 0-255 space)
// 0 is all the way left
// 255 is all the way right
volatile uint8_t line;


// the phase describes what the microprocessor is currently doing.
// phase_wait   = waiting for a change of sweep direction (waiting for the p_sweep pin to change states either low-to-high or high-to-low)
// phase_warmup = found the state change of p_sweep, now waiting the data_warmup time for data to be active.
// phase_active = currently reading data from the p_data pin and attempting to interpret it as a line to follow.
#define phase_wait		0
#define phase_warmup	1
#define phase_active	2
volatile char phase = phase_wait;		// default (upon start-up) is 0


// the valid data from p_data will come some period of time after the p_sweep pin changes states.
// data_deadzone describes how long it takes from the change of the p_sweep signal to when valid data appears from the p_data pin.
#define data_warmup		((uint16_t)1000)	// 1 ms  (1,000 timer cycles at 1 MHz)
// data_active describes how long the valid data lasts in a given sweep.
// in other words, over a period where p_sweep stays either high or low (in one sweep half-cycle, when the laser is either going right-to-left, or left-to-right) how long
#define data_active		((uint16_t)14000)	// 14 ms (14,000 timer cycles at 1 MHz)
// this is the max value of timer1
#define timer1_TOP ((uint16_t)0xffff)


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
	
	TCCR1B |= (1<<CS11);			// timer1 has a prescaler of 8
	
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
	
	// this records the current state of the data pin on portB
	static char currentData = 0;
	// this records what the last value of the p_data pin was when this function was last called.
	static char lastData = 0;
	
	// this records what the last value of the p_sweep pin was when this function was last called.
	static char lastSweepDirection = 0;
	
	// these keep track of when the pulses occur.
	static uint16_t rEdge = 0;
	
	// extract data  bit from portBdata
	currentData =    ( (portBdata & (1<<p_data))  >> p_data );
	// extract sweep bit
	sweepDirection = ( (portBdata & (1<<p_sweep)) >> p_sweep);
	
	//-----------------------------------------------------------------
	// handle sweep pin
	//-----------------------------------------------------------------
	
	// if a change in sweepDirection just occurred, then enter phase_warmup.
	if(sweepDirection != lastSweepDirection)
	{
		// reset timer1 (high byte first)
		TCNT1H = 0;
		TCNT1L = 0;
		// set compareA value to data_warmup time (high byte first)
		OCR1AH = (data_warmup & 0xff00)>>8;
		OCR1AL = (data_warmup & 0x00ff);
		
		// enter the warmup phase
		phase = phase_warmup;
		
		// reset the foundValidPulse variable
		foundValidPulse = 0;
		// reset the edge variables
		rEdge = 0;
		
		// if the linePosition signal is still on, turn it off. This is just to make sure the linePosition doesn't roll over into the next cycle of data.
		low(PORTA, p_linePosition);
		
		// sensitize the data pin.
		// it is now time to start looking for data on that pin.
		PCMSK1 |= (1<<PCINT8);
		
	}
	
	//-----------------------------------------------------------------
	// handle data pin
	//-----------------------------------------------------------------
	
	// if the data pin value has changed,
	if((currentData != lastData) && phase == phase_active)
	{
		// if the data pin just went high,
		if(currentData)
		{
			// if you have not yet found a valid pulse,
			if(!foundValidPulse)
			{
				// record where the most recent rising edge was located
				rEdge = currentTimer1;
			}
			// otherwise, if you have already found a valid pulse,
			else
			{
				// record where the middle of the barcode scanner line was.
				uint32_t middlePosition = rEdge + currentTimer1;
				// calculate the line value [0, 255] based on the middlePosition of the line inside the range of [0, data_active]
				// then store that value in the OCRA so that the duty cycle of the PWM waveform is representative of the line's position.
				OCR0A = (uint8_t)( (middlePosition<<7)/data_active );
				// if the sweep direction is one way, the duty cycle has to be inverted
				if(sweepDirection) OCR0A = 255-OCR0A;
				// turn off the linePosition signal to signal that the edge of the line has been found.
				low(PORTA, p_linePosition);
				// turn off the dataReady signal.
				// Because the duty cycle of the linePosition has just changed, the analog signal needs some time to settle.
				// therefore, the data isn't ready right away.
				// this pin is turned high when the program enters the waiting period.
				low(PORTA,p_dataReady);
				
				// record the currentTimer1 just in case this is the start of the next valid pulse
				rEdge = currentTimer1;
				// desensitize the data pin.
				// we don't need to be bothering the processor with unnecessary interrupts now that we have found the line.
				// the data pin will re-sensitize when the sweep direction changes state.
				PCMSK1 &= ~(1<<PCINT8);
			}
		}
		// if the data pin just went low
		else
		{
			// if the falling edge (currentTimer1) occurrs at least [PULSE_TIME_MIN] later than rEdge, then the pulse is wide enough to be considered a valid pulse.
			if(currentTimer1 >= PULSE_TIME_MIN + rEdge)
			{
				// turn on the linePosition indicating that a valid pulse has been found (thus indicating that a line has been found)
				high(PORTA, p_linePosition);
				// record that a valid pulse was found.
				foundValidPulse = 1;
			}
		}
		
	}
	
	// record the current sweep direction as the last one
	lastSweepDirection = sweepDirection;
	// record the current data pin state as the last one
	lastData = currentData;
}


// this function handles when the timer0 has a compare match.
// mainly, this function simply handles changing the state of the device between warmup, active, and waiting
ISR(TIM1_COMPA_vect)
{
	// reset the timer 1 count (high byte first)
	TCNT1H = 0;
	TCNT1L = 0;
	
	// if the current phase was "warmup", then we must switch to the active phase now
	if(phase == phase_warmup)
	{
		// set the timer to interrupt in data_active time.
		// the data_active phase lasts only this long before the waiting stage begins.
		OCR1AH = ((data_active+data_warmup) & 0xff00)>>8;
		OCR1AL = ((data_active+data_warmup) & 0x00ff);
		
		// switch to the active phase
		phase = phase_active;
	}
	// if the current phase was "active", then the active phase has expired and we are now going to WAIT for the next change in sweep direciton.
	else if(phase == phase_active)
	{
		// if the active-data period just ended, enter the waiting-phase.
		// the waiting phase will take as long as it needs to (until the sweep direction changes).
		// this phase doesn't have a specified length, but it is usually a few milliseconds or so.
		phase = phase_wait;
		// turn on the dataReadyPin if a valid pulse was found.
		// Surely, by now, the PWM-to-analog signal has approached its steady-state value.
		// ( it takes approximately a millisecond is the minimum value with two passive low-pass RC filters with 10kohm resistors and 0.01 microfarad ceramic caps).
		// this will happen right after the active phase, so in all likelyhood,
		if(foundValidPulse)
			high(PORTA,p_dataReady);
	}
}


void init_input_interrupts()
{
	// enable interrupts for port B pins (PCINT 11:8).
	GIMSK |= (1<<PCIE1);
	
	// enable interrupts for PCINT 8 and 9 specifically (PORTB pins B0, B1) (ATtiny24A DIP package pins 2 and 3).
	// this interrupt will be triggered on a logical state change (rising/falling edge)
	PCMSK1 |= (1<<PCINT8)|(1<<PCINT9);
}


// this is where the program starts
int main(void)
{
	 // PORTB
	 // set individual pin I/O directions
	set_input(DDRB, p_data);
	set_input(DDRB, p_sweep);
	set_output(DDRB, p_line);
	// set initial states
	low(PORTB, p_line);
	
	// PORTA
	// set individual pin I/O directions
	set_output(DDRA, p_linePosition);
	set_output(DDRA, p_dataReady);
	// initial states
	low(PORTA, p_linePosition);
	low(PORTA, p_dataReady);
	
	// set up timers and interrupts
	sei();								// enable global interrupts
	init_timer0();						// initialize timer0
	init_timer1();						// initialize timer1
	init_input_interrupts();			// initialize input interrupts
	
	// the main() function only serves to setup the program. everything happens based on interrupts, so our work in the main() function is done. we can kick back
	while(1)
    {
		// do nothing, as if the solution to all your problems will fall right out of the sky
	}
}
