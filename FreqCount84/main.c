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
#include "shift_out_24_PORTA.h"		// this gives us the function we need to utilize a shift register to shift OUT data.

//=================================================================
// pin definitions
//=================================================================

// PORTB
#define p_freq_in			PORTB2		// Input digital signal (0 to 5V). We try to measure the frequency of this one.



// PORTA
// OUTPUT SHIFT REGISTER pins:
#define p_SR_data			PORTA3		// this pin holds the data that must be clocked into the shift register
#define p_SR_RCK			PORTA4		// this pin updates the outputs of the shift register
#define p_SR_SCK			PORTA5		// this pin clocks data into the shift register

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
// Shift register output pins guide
//=================================================================
// The shift register is serial-in parallel-out.
// The shift register is 24 bits long.
// Q0 is the first output of the shift register
// Q23 is the last output of the shift register
/*
	Q0		seven-segment A  anode for all digits
	Q1		seven-segment B  anode for all digits
	Q2		seven-segment C  anode for all digits
	Q3		seven-segment D  anode for all digits
	Q4		seven-segment E  anode for all digits
	Q5		seven-segment F  anode for all digits
	Q6		seven-segment G  anode for all digits
	Q7		seven-segment DP anode for all digits
	
	Q8		unit indicator	MegaHertz
	Q9		unit indicator	Hertz
	Q10		unit indicator	Seconds
	Q11		unit indicator	Minutes
	Q12		
	Q13		
	Q14		
	Q15		
	
	Q16		digit 0	-	transistor pulling digit 0's common cathode low (most  significant digit) (left-most  digit)
	Q17		digit 1	-	transistor pulling digit 1's common cathode low
	Q18		digit 2	-	transistor pulling digit 2's common cathode low
	Q19		digit 3	-	transistor pulling digit 3's common cathode low
	Q20		digit 4	-	transistor pulling digit 4's common cathode low
	Q21		digit 5	-	transistor pulling digit 5's common cathode low (least significant digit) (right-most digit)
	Q22		
	Q23		
*/


//=================================================================
// numerical definitions
//=================================================================

// this is how many digits the display has
#define NUMBER_OF_DIGITS (6)
// this is how long each digit is displayed for (units of milliseconds)
#define DIGIT_DISPLAY_PERIOD (2)
// this indicates the value that should be used to indicate that the first significant figure has not been found yet.
// this is not arbitrary. It has to be greater than the number of significant figures you would ever expect to support.
#define FIRST_SIGFIG_NOT_FOUND_YET ((uint8_t)255)

// this describes where to start the search for the first significant figure.
// this is based on how many digits you have
// in general, if you have an N-digit display, you should make your decade start 10^(N-1) for peak efficiency.
# define DECADE_START ((double)10e5)

//=================================================================
// variable declaration and 
//=================================================================

// this keeps track of whether or not the counter has been triggered for the first time.
uint8_t triggered = 0;
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
// this is the calcualted period based on the frequency 
double period_measurement_s = 0;
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
	TCCR1B |= (1<<CS10);			// timer1 is clokced from the main clock (20 MHz)
	
	//TCNT1H = 0;						// reset the timer1 count (both the high and low bytes)
	//TCNT1L = 0;						// "
	
	//OCR1AH = 0xff;					// set high byte of the output compare register
	//OCR1AL = 0xff;					// set low  byte of the output compare register
	
	//TIMSK1 |= (1<<OCIE1A);			// enable the output-compare interrupt for register A
	TIMSK1 |= (1<<TOIE1);				// enable overflow-interrupt enable of timer 1.
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


// this handles when either of the two input pins changes states
ISR(PCINT1_vect)
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
		triggered = 1;
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
			// calculate the period
			period_measurement_s = 1/freq_in_measurement_Hz;
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
ISR(TIM1_OVF_vect)
{
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
	set_output(DDRA, p_SR_data);
	set_output(DDRA, p_SR_SCK);
	set_output(DDRA, p_SR_RCK);
	// set the initial values of these pins
	low(PORTA, p_SR_data);
	low(PORTA, p_SR_SCK);
	low(PORTA, p_SR_RCK);
	
	// set up timers and interrupts
	sei();								// enable global interrupts
	init_timer1();						// initialize timer1
	init_input_interrupts();			// initialize input interrupts

	uint8_t sigfig = FIRST_SIGFIG_NOT_FOUND_YET;	// records which significant figure of the number we are currently processing
	uint8_t stopEncoding = 0;			// this keeps track of when to stop encoding the number from double into seven-segment format
	uint16_t digit = 0;					// keeps track of which digit is currently being displayed
	uint16_t s = 0;						// keeps track of which segment is being displayed (only used in the startup/debug screen)
	uint16_t unit = 0;					// tells us which unit indicator we should light.
										// 0 = Megahertz
										// 1 = Hertz
										// 2 = Seconds
										// 3 = Minutes
	
	double currentFreq = 0;				// this keeps track of the current frequency that will be displayed
	double currentPeriod = 0;			// this keeps track of the current period    that will be displayed
	double dispNumber = 0;				// this keeps track of what number we want to display (frequency/period gets shoved in here and the number gets encoded for the 7-segment display the same way in both cases.)
	
	//-----------------------------------------------------------------
	// the LED displays are handled in this while loop.
	//-----------------------------------------------------------------
	while(1)
    {
		if(triggered)
		{
			// check if we are starting a new frame (a new set of six display digits)
			if(digit >= NUMBER_OF_DIGITS)
			{
				digit = 0;
				double decade;
			
				// grab the current frequency and period
				currentFreq = freq_in_measurement_Hz;
				currentPeriod = period_measurement_s;
				
				// determine what unit we want to use
				if     (currentFreq >= 1e6)
				{
					unit = 0;							// use units of MHz
					dispNumber = currentFreq/1.0e6;		// display MHz
				}
				else if(currentFreq >= 1)
				{
					unit = 1;							// use units of Hz
					dispNumber = currentFreq;			// display hertz
				}
				else if(currentPeriod <= 60)
				{
					unit = 2;							// use units of Seconds
					dispNumber = currentPeriod;			// display seconds
				}
				else
				{
					unit = 3;							// use units of Minutes
					dispNumber = currentPeriod/60.0;	// display minutes
				}
				
				// default is a bogus value to indicate that the first significant figure has not been found yet
				sigfig = FIRST_SIGFIG_NOT_FOUND_YET;
				// make sure the for loop starts encoding
				stopEncoding = 0;
				for(decade = DECADE_START; !stopEncoding; decade/=10.0)
				{
					// if you have found the first significant digit,
					if(dispNumber >= decade && sigfig == FIRST_SIGFIG_NOT_FOUND_YET)
					{
						// start processing the number into 
						sigfig = 0;
					}
					// if you have found at least the first significant digit,
					if(sigfig < NUMBER_OF_DIGITS)
					{
						// record what this digit was by putting it in the disp_data
						disp_data[sigfig] = (uint8_t)(dispNumber/decade);
						// subtract the digit that has already been accounted for
						dispNumber -= (double)disp_data[sigfig]*decade;
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
					
					// If you have encoded the necessary number of digits,
					if(sigfig == NUMBER_OF_DIGITS)
					{
						// quit encoding digits
						stopEncoding = 1;
					}
				
				}
			}
			
			
			// select the digit you want to display
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, 1<<digit, 'm');
			// select the unit LED indicator you want to light
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, 1<<unit, 'm');
			// shift out the data for the current digit of the seven-segment display
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, disp_data[digit], 'm');
			
			// update the output registers
			low(PORTA,p_SR_RCK);
			high(PORTA,p_SR_RCK);
			
			// this sets (roughly) the refresh rate of the display.
			_delay_ms(DIGIT_DISPLAY_PERIOD);
			
			// We have just displayed a digit. It is time to
			digit++;
		}
		
		// if you have not triggered yet,
		// (this is basically just something to do when the unit first starts up)
		else
		{
			// limit s to the number of segments
			if(s >= 8){
				s = 0;
				// when the segment overflows, increment the digit
				digit++;
			}
			// limit d to the number of digits
			if(digit >= 6){
				digit = 0;
				// when the digit overflows, increment the unit
				unit++;
			}
			// limit u to the number of units
			if(unit >= 4){
				unit = 0;
			}
			
			// shift out some debugging info
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, (uint16_t)1<<digit, 'm');		// shift out the digit   data
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, (uint16_t)1<<unit, 'm');		// shift out the unit    data
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, (uint16_t)1<<s, 'm');		// shift out the segment data
			
			// update the output registers
			low(PORTA,p_SR_RCK);
			high(PORTA,p_SR_RCK);
			_delay_ms(50);
			
			// increment the segment counter
			s++;
		}
	}
}


// fall
// right
// out of the 
// sky
