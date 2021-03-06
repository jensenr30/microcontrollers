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
#include <stdio.h>				// This takes up a SHITLOAD of space.
								// you need to make sure that the following options/flags are in your linker settings when you compile this project:
								//		-Wl,-u,vfprintf -lprintf_flt -lm
								// the above linker options make sprintf() work for FLOATING POINT type numbers.
								// otherwise, it will output garbage when it gets floating point numbers as inputs.
								// if I ever run out of space, I should reuse my float-to-string function that I created in main().
								// because stdio.h (with those linker flags) take up at least half of my available memory on an ATtiny84.
								

#include "shift_out_24_PORTA.h"	// this gives us the function we need to utilize a shift register to shift OUT data.



//=================================================================
// pin definitions
//=================================================================

//-----------------------------------------------------------------
// PORTB
//-----------------------------------------------------------------
#define p_freq_in			PORTB2		// Input digital signal (0 to 5V). We try to measure the frequency of this one.

//-----------------------------------------------------------------
// PORTA
//-----------------------------------------------------------------
// debug pin
#define p_debug				PORTA0		// used for various debugging activities
// UART pins
#define p_UART_Tx			PORTA1		// outputs UART messages at UART_BAUD_XXXXX (frequency measurements output) characters. units of Hz. human readable.
#define p_UART_trig			PORTA2		// (when this transitions from LOW to HIGH, the UART pin will 
// OUTPUT SHIFT REGISTER pins:
#define p_SR_data			PORTA3		// this pin holds the data that must be clocked into the shift register
#define p_SR_RCK			PORTA4		// this pin updates the outputs of the shift register
#define p_SR_SCK			PORTA5		// this pin clocks data into the shift register
// unused pins
#define p_unused_A6			PORTA6
#define p_unused_A7			PORTA7


#define freq_div_bits 3					// there are three bits in the output shift register that are used for selecting the frequency division.
#define freq_div_bit_mask ( (1<freq_div_bits) - 1)

// this is how many seconds are contained in one clock cycle
#define clock_period_sec ( 1/(double)F_CPU )
// this is how many seconds are contained in one overflow value (if the overflow value is 1, this is how many seconds it took to reach that overflow value of 1).
#define overflow_period_sec ( (0x10000) / (double)F_CPU )


// this set the target number of clock cycles to find before calculating the input frequency.
#define freq_calc_min_clock_cycles ( (uint32_t)500000 )		// 40 Hz measurement period (1ppm resolution)
// this is the maximum frequency at which the device will measure frequency.
// this limits the speed at which frequency measurements can be taken
#define freq_calc_max_freq ( (double)F_CPU/(double)freq_calc_min_clock_cycles )

// this is the max value of timer1
#define timer1_TOP ((uint16_t)0xffff)

// this is the reset value of timer1.
// this should IDEALLY be 0, but because doing calculations takes some time out of the measurement period,
// I need to start the timer off with the right number of counts to make up for the lost time
// in doing a few calculations.
// the extra counts equate to some number of microseconds offset.
// this starts the counter with 38 counts already in it. This equates to 1.9 microseconds at 20 MHz.
// NOTE:
#define TCNT1H_reset (0)
#define TCNT1L_reset (38)

// this is the same as the TCNT1X_reset variables, except these are used when trigger has been reset (when a message is sent over UART or when the device starts up)
#define TCNT1H_reset_triggered (0)
#define TCNT1L_reset_triggered (0)


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
	Q12		frequency division selection 0
	Q13		frequency division selection 1
	Q14		frequency division selection 2
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
// variable declaration (and some definitions)
//=================================================================

// this keeps track of whether or not the counter has been triggered for the first time.
// when the counter decides that it needs to change the frequency division, this will be reset to 0.
volatile uint8_t startUp = 1;
// this keeps track of whether or not the device has been triggered at least once using the latest frequency division settings.
volatile uint8_t triggered = 0;
// this keeps track of whether or not the frequency division settings have been changed during the current measurement period.
// 0 indicates	the current measurement is valid and has been made with no change to the frequency division settings.
// 1 indicates	the current measurement is invalid because the frequency division settings were just changed.
// 2 indicates	a measurement is being performed that WILL be valid when it is complete.
volatile uint8_t adjustingFreqDiv = 0;

// this keeps track of how many times Timer1 has overflowed.
// with a clock frequency of 20 MHz, a 16-bit timer, and a 32-bit overflow counter, this program should be able to count periods as long as 162 days.
volatile uint32_t overflows = 0;
// this records how many cycles (periods) have occurred on the p_freq_in pin.
volatile uint16_t freq_in_cycles = 0;
// this specifies how to divide the input frequency
// the amount that the frequency will be divided before being presented at the input of the ATtiny84 is given as:
// freq_meas = freq_in / ( 2 ^ (2*freq_div) );
// This is a table of valid values for freq_div:
//=======================================================
//	setting	| division  |	division (alternate form)
//==========|===========|================================
//	0		|	1		|	2^0		(no division)
//	1		|	4		|	2^2
//	2		|	16		|	2^4
//	3		|	64		|	2^6
//	4		|	256		|	2^8
//	5		|	1024	|	2^10
//	6		|	4096	|	2^12
//	> 6		|	4096	|	2^12	(max division from the 74**4040)
volatile uint8_t freq_div = 0;
// this is the maximum valid frequency division setting
#define freq_div_max ((uint8_t)6)
#define freq_div_min ((uint8_t)0)

// this is the maximum frequency that *should* be applied to the ATtiny84A.
// the device will tolerate frequencies higher than this, but the display operation will suffer past the 35 kHz mark.
// therefore, the frequency input should be divided down to something below this max setting.
#define freq_meas_max	( (double) 10e3 )		// if the frequency applied to the input of the ATtiny84 goes above this frequency, the freq-div should be increased.
#define freq_meas_min	( (double) 1e3  )		// if the frequency applied to the input of the ATtiny84 goes below this frequency, the freq-div should be decreased.
#define overflows_max	( (uint32_t) 2*freq_calc_max_freq ) // if the program ever has more than this many overflows, the frequency division should be decreased.
											// note: the number of overflows is chosen as twice as many as the maximum measurement frequency to ensure things are stable.

//-----------------------------------------------------------------
// these variables keep track of the frequency measured at the ATtiny84 input pin.
// NOTE: this may not be the actual frequency! This could be a divided-down version of the input frequency.
// the "real" frequency that we want to measure is the frequency that is being applied to the input of the binary counter.
// this program controls the frequency division (see above).
// from the frequency division settings, the real input frequency is calculated.
//-----------------------------------------------------------------
// these two variables record the state of the timer and overflow counter at the moment when 
//uint16_t OFF_time_timer = 12345;
//volatile uint32_t OFF_time_overflows = 0;
// this is updated with measurements of the input frequency
double freq_meas_Hz = 0;
// this is the calculated period based on the freq_meas_Hz 
double period_meas_s = 0;
// this is updated with measurements of the duty cycle (%)
//volatile double freq_in_duty_cycle = 0;

// this is a dummy variable used to debug the shift register
//uint32_t data;

uint8_t sevenseg[10] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};


// this keeps track of which digit is being displayed (The persistence of vision technique is used. Only one 7-segment display is lit at a time.)
uint8_t digit = 0;
// this is the data that is displayed
// default data is all underlines
uint8_t disp_data[6];// = {0x08,0x08,0x08,0x08,0x08,0x08};

// this keeps track of whether or not we need to transmit the frequency data out the UART port.
volatile uint8_t transmitFrequencyMeasurementOverUART = 1;
// this is how many bits per second the UART should be transmitting at

// these definitions choose what baud the FreqCount84 UART is transmitting at.
// uncomment the one you want (and ONLY one)
//#define UART_BAUD_9600 (1)
#define UART_BAUD_19200 (1)
//#define UART_BAUD_115200 (1)

#define UART_MESSAGE_BYTES (13)					// 1.23456e+001\n   is thirteen characters
volatile char UART_str[UART_MESSAGE_BYTES];		// this stores the data that we want to send out the UART.
volatile uint8_t UART_byte = 0;					// keeps track of which byte is being transmitted
volatile uint8_t UART_bit  = 0;					// keeps track of which bit  is being transmitted
volatile uint8_t UART_transmitting = 0;			// this indicates whether or not a message is currently being sent out the UART.

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
	TCCR1B |= (1<<CS10);				// timer1 is clocked from the main clock (20 MHz)
	
	//TCNT1H = 0;						// reset the timer1 count (both the high and low bytes)
	//TCNT1L = 0;						// "
	
	//OCR1AH = 0xff;					// set high byte of the output compare register
	//OCR1AL = 0xff;					// set low  byte of the output compare register
	
	//TIMSK1 |= (1<<OCIE1A);			// enable the output-compare interrupt for register A
	TIMSK1 |= (1<<TOIE1);				// enable overflow-interrupt enable of timer 1.
}


void disable_timer1_interrupts()
{
	TIMSK1 &= ~(1<<TOIE1);				// disable overflow-interrupt enable of timer 1.
}
void enable_timer1_interrupts()
{
	TIMSK1 |= (1<<TOIE1);				// enable overflow-interrupt enable of timer 1.
}



// enable and configure the input pin interrupts.
void init_input_interrupts()
{
	// enable interrupts for port B pins (PCINT 11:8).
	GIMSK |= (1<<PCIE1);
	
	// enable interrupts for PCINT 10 specifically.
	// This corresponds to PORTB PB2 - ATtiny84 DIP package pin5.
	// this interrupt will be triggered on a logical state change (rising/falling edge).
	PCMSK1 |= (1<<PCINT10);
	
	// enable interrupts for port A pins (PCINT 0:7)
	GIMSK |= (1<<PCIE0);
	
	// enable interrupts for PCINT 2 specifically
	// this corresponds to PORTA PA2 - ATtiny84 DIP package pin 11
	// this interrupt will be triggered on a logical state change (rising/falling edge).
	PCMSK0 |= (1<<PCINT2);
}


void disable_input_interrupts()
{
	// disable interrupts for port A pins (PCINT 0:7)
	GIMSK &= ~(1<<PCIE0);
	// disable interrupts for port B pins (PCINT 11:8).
	GIMSK &= ~(1<<PCIE1);
}


// this function handles when the timer1 has a compare match.
// mainly, this function simply handles changing the state of the device between warmup, active, and waiting
ISR(TIM1_OVF_vect)
{
	// increment the counter that keeps track of how many times Timer1 has overflowed.
	overflows++;
	// TODO: remove this little debugging thingy
	//toggle(PORTA, p_debug);
	
	// if the number of overflows goes above what should be allowed,
	// AND if the frequency division CAN be decreased
	if( (overflows >= overflows_max) && (freq_div > freq_div_min) )
	{
		// decrease the frequency division
		freq_div--;
		
		// reset the frequency measurement variables
		// this basically aborts the current frequency measurement and prepares to do another one.
		TCNT1H = TCNT1H_reset;						// reset the timer1 count (both the high and low bytes)
		TCNT1L = TCNT1L_reset;						// this is a non zero value. see definitions of the reset values for better information
		freq_in_cycles = 0;
		overflows = 0;
	}
	
}



// interrupt service routine for Port A
// this handles when the UART_trig pin changes state
ISR(PCINT0_vect)
{
	uint8_t portAdata = PINA;
	
	// if the UART trigger pin went high,
	if( portAdata & (1<<p_UART_trig) )
	{
		transmitFrequencyMeasurementOverUART = 1;
	}
	
}


void UART_bit_delay()
{
	#ifdef UART_BAUD_9600
		// this is setup for 9600 baud
		_delay_loop_1(250);
		_delay_loop_1(250);
		_delay_loop_1(194);
	#endif
	
	#ifdef UART_BAUD_19200
	// this is setup for 9600 baud
	_delay_loop_1(250);
	_delay_loop_1(97);
	#endif
	
	// I think this needs some work. 11500 does not seem to work well currently.
	#ifdef UART_BAUD_115200
		_delay_loop_1(58);
	#endif
}



// this will transmit a single character out the UART port
void UART_transmit_character(char TxChar)
{
	// start bit
	low(PORTA,p_UART_Tx);
	UART_bit_delay();
		
	uint8_t b;
	for(b=0; b<8; b++)
	{
		if(TxChar & (1<<b))
		{
			high(PORTA,p_UART_Tx);
			UART_bit_delay();
		}
		else
		{
			low(PORTA,p_UART_Tx);
			UART_bit_delay();
		}
	}
	
	// stop bit
	high(PORTA,p_UART_Tx);
	UART_bit_delay();
}


void transmit_frequency_measurement_UART(double freq_to_report)
{
	// diable all interrupts
	disable_input_interrupts();
	disable_timer1_interrupts();
	
	// given that you are shutting down the frequency-measuring side of things, you need to realize that you could be missing things.
	// you must record that you were not paying attention to the input frequency during this time.
	// when you are done printing data to the UART, you will need to try to establish trigger again.
	triggered= 0;
	
	// transmit a very important message
	//UART_transmit_character('F');
	//UART_transmit_character('U');
	//UART_transmit_character('C');
	//UART_transmit_character('K');
	//UART_transmit_character(10 );
	
	// create the string that will be printed over UART
	char *str = "1.23456e+000";
	sprintf(str, "%.5e", freq_to_report);//freq_meas_Hz);
	
	str[UART_MESSAGE_BYTES-1] = 10;		// newline
	
	uint8_t i;
	for(i=0; i<UART_MESSAGE_BYTES; i++)
	{
		UART_transmit_character(str[i]);
	}
	
	// restore the interrupts to their former glory
	enable_timer1_interrupts();
	init_input_interrupts();
	
}


// interrupt service routine for Port B
// this handles when freq_meas changes state
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
	uint16_t currentOverflows = overflows;
	
	//-----------------------------------------------------------------
	// Next up: figure out what the state of the pin is
	//-----------------------------------------------------------------
	
	// grab the state of the freq_in pin.
	uint8_t freq_in_state = ( (portBdata & (1<<p_freq_in))  >> p_freq_in );
	
	// if the signal went low,
	if( !freq_in_state )
	{
		// if you have not triggered yet, you need to record when your first trigger was.
		if(!triggered)
		{
			// reset all variables
			TCNT1H = TCNT1H_reset_triggered;						// reset the timer1 count (both the high and low bytes)
			TCNT1L = TCNT1L_reset_triggered;						// this is a non zero value. see definitions of the reset values for better information
			overflows = 0;
			freq_in_cycles = 0;
			//OFF_time_timer = 0;
			//OFF_time_overflows = 0;
			triggered = 1;
		}
		// if you HAVE triggered,
		else
		{
			// record that the input signal DID, in fact, have another falling edge.
			freq_in_cycles++;
			
			// calculate the number of cycles that have taken place since the beginning of the measurement period
			uint32_t timer1_cycles = ((uint32_t)currentOverflows<<16) + (uint32_t)currentTimer1;
			
			// if the current sample has been going for the necessary amount of time
			// (or if the frequency division settings were just changed)
			if( timer1_cycles >= freq_calc_min_clock_cycles )
			{
				// start your next sample by resetting Timer1.
				TCNT1H = TCNT1H_reset;						// reset the timer1 count (both the high and low bytes)
				TCNT1L = TCNT1L_reset;						// this is a non zero value. see definitions of the reset values for better information
				
				// calculate the period over which this sampled was performed.
				double period_sec =  (currentTimer1*clock_period_sec) + (overflows        *overflow_period_sec);
				// record the frequency
				freq_meas_Hz = freq_in_cycles/period_sec;
				// calculate the period
				period_meas_s = 1/freq_meas_Hz;
				// calculate the OFF_time in seconds
				//double OFF_time_sec = (OFF_time_timer*clock_period_sec) + (OFF_time_overflows*overflow_period_sec);
				// record the duty cycle
				//freq_in_duty_cycle = OFF_time_sec/period_sec;
				
				// this is for debugging to show where the frequency measurement happens
				toggle(PORTA,p_debug);
				
				// reset all variables (Timer1 was already reset above)
				freq_in_cycles = 0;
				overflows = 0;
				//OFF_time_timer = 0;
				//OFF_time_overflows = 0;
			
				// if the frequency that was just measured is greater than the max frequency you would like to see at the input of the ATtiny84,
				// AND the freq_div CAN be increased
				if(freq_meas_Hz > freq_meas_max && freq_div < freq_div_max)
				{
					// attempt to lower it
					freq_div++;
				}
				// if the frequency that was just measured is less than the min frequency you would like to see at the input of the ATtiny84,
				// AND the freq_div CAN be decreased
				if(freq_meas_Hz < freq_meas_min && freq_div > freq_div_min)
				{
					// attempt to lower it
					freq_div--;
				}
			} // if(you can make a freq measurement)
		}// if(you have triggered)
	}// if the input was a <rising/falling> edge
	
	// if the input state change was not in the right polarity,
	else
	{
		
		//-----------------------------------------------------------------
		// Or record the midpoint
		//-----------------------------------------------------------------
		
		// record when the digital signal went low
		//OFF_time_timer = currentTimer1;
		//OFF_time_overflows = overflows;
		
	}
	
	
}// end the ISR for freq measurement input



// this is where the program starts
int main(void)
{
	
	//-----------------------------------------------------------------
	// PORTB
	//-----------------------------------------------------------------
	// set individual pin I/O directions
	set_input(DDRB, p_freq_in);
	
	
	//-----------------------------------------------------------------
	// PORTA
	//-----------------------------------------------------------------
	// set individual pin I/O directions
	set_output(DDRA, p_debug);
	set_output(DDRA, p_SR_data);
	set_output(DDRA, p_SR_SCK);
	set_output(DDRA, p_SR_RCK);
	set_output(DDRA, p_UART_Tx);
	
	set_input(DDRA, p_UART_trig);
	
	// set the initial values of these pins
	low(PORTA, p_debug);
	low(PORTA, p_SR_data);
	low(PORTA, p_SR_SCK);
	low(PORTA, p_SR_RCK);
	high(PORTA, p_UART_Tx);
	
	// enable pull-up resistor for the UART trigger pin
	high(PORTA, p_UART_trig);
	
	
	// unused pins set as inputs
	set_input(DDRA,p_unused_A6);
	set_input(DDRA,p_unused_A7);
	// unused pins have pull-up-resistor enabled
	high(DDRA,p_unused_A6);
	high(DDRA,p_unused_A7);
	
	
	
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
	
	double currentFreqIn = 0;				// this keeps track of the current frequency that will be displayed
	double currentPeriodIn = 0;			// this keeps track of the current period    that will be displayed
	double dispNumber = 0;				// this keeps track of what number we want to display (frequency/period gets shoved in here and the number gets encoded for the 7-segment display the same way in both cases.)
	
	
	//-----------------------------------------------------------------
	// the LED displays are handled in this while loop.
	//-----------------------------------------------------------------
	while(1)
    {
		// if the unit is EVER triggered, it will no longer be in start up mode.
		if(triggered)
		{
			startUp = 0;
		}
		
		// if you need to transmit the frequency measurement over UART,
		// and if you are not currently transmitting frequency measurements,
		if(transmitFrequencyMeasurementOverUART)
		{
			// then do it. send the last frequency measurement over UART
			transmit_frequency_measurement_UART(currentFreqIn);
			// don't keep doing it.
			transmitFrequencyMeasurementOverUART = 0;
		}
		
		// if you are not in start-up mode (i.e. you are making measurements)
		if(!startUp)
		{
			
			// check if we are starting a new frame (a new set of six display digits)
			if(digit >= NUMBER_OF_DIGITS)
			{
				digit = 0;
				double decade;
				
				
				
				// grab the current frequency and period
				currentFreqIn =   freq_meas_Hz  * (double)( (uint32_t)1 << 2*freq_div );
				currentPeriodIn = period_meas_s / (double)( (uint32_t)1 << 2*freq_div );
				
				// determine what unit we want to use
				if     (currentFreqIn >= 1e6)
				{
					unit = 0;							// use units of MHz
					dispNumber = currentFreqIn/1.0e6;		// display MHz
				}
				else if(currentFreqIn >= 1)
				{
					unit = 1;							// use units of Hz
					dispNumber = currentFreqIn;			// display hertz
				}
				else if(currentPeriodIn <= 60)
				{
					unit = 2;							// use units of Seconds
					dispNumber = currentPeriodIn;			// display seconds
				}
				else
				{
					unit = 3;							// use units of Minutes
					dispNumber = currentPeriodIn/60.0;	// display minutes
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
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, 1<<digit,			'm');
			// select what division factor you want to apply to the input signal before it is applied to the ATtiny84.
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 4, freq_div,			'm');
			// select the unit LED indicator you want to light
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 4, 1<<unit,				'm');
			// shift out the data for the current digit of the seven-segment display
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, disp_data[digit],	'm');
			
			// update the output registers
			low(PORTA,p_SR_RCK);
			high(PORTA,p_SR_RCK);
			
			// this sets (roughly) the refresh rate of the display.
			_delay_ms(DIGIT_DISPLAY_PERIOD);
			
			// We have just displayed a digit. It is time to
			digit++;
		}
		
		
		// if you have just started up and have not seen any transitions,
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
			
			// shift out data that controls everything
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 2, 0,						'm');	// shift out two empty bits
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 6, (uint16_t)1<<digit,		'm');	// shift out the digit   data
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 1, 0,						'm');	// shift out one empty bit
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 3, (uint16_t)freq_div,		'm');	// shift out the frequency division data
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 4, (uint16_t)1<<unit,		'm');	// shift out the unit    data
			shift_out_24_PORTA(p_SR_SCK, p_SR_data, 8, (uint16_t)1<<s,			'm');	// shift out the segment data
			
			// update the output registers
			low(PORTA,p_SR_RCK);
			high(PORTA,p_SR_RCK);
			_delay_ms(30);
			
			// increment the segment counter
			s++;
		}
	}
}


// fall
// right
// out of the 
// sky
