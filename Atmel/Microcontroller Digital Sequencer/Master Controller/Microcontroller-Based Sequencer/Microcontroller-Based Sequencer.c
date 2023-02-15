/*
 * Microcontroller-Based Sequencer.c
 *
 * Created: 10/6/2013 4:30:50 PM
 * Last edited: 12/4/2013
 * Author: Ryan P Jensen
 * 
 * The purpose of this program is to control music and peripheral music devices.
 * There is an 8x8 LED matrix that this device will control. 5 pins are allocated to the control of this device.
 * 
 */

//----------------------------------------------------------------------------------------------
// headers and globals
//----------------------------------------------------------------------------------------------

#define F_CPU 8000000UL			// define the clock speed as 8MHz. This clock speed is used in the delay.h header file. This does not actually change the clock speed of the device.
#include <avr/io.h>				// this is the basic Input output of AVR microcontrollers I think
#include <util/delay.h>			// this allows me to use the delay functions
#include <avr/interrupt.h>		// this allows me to use interrupt functions
#include "keys.h"				// this is a proprietary header that have enumerations of different tones

// information about how long the song can be
#define BEATS_PER_MEASURE 8								// this is the number of beats in each measure of music
#define SONG_MEASURES 32								// this is the number of measures int the entire song
#define TOTAL_BEATS SONG_MEASURES*BEATS_PER_MEASURE		// this is the totcal number of beats in the song
#define SONG_BEATS (TOTAL_BEATS)	// this is the number of beats in the song there are. (total_measures * beats/measure)

// this holds all of the song data
unsigned char song[SONG_BEATS];
// this tells us what beat of the song we are currently on.
volatile unsigned int offset = 0;
// this stores the tones that will be played.
unsigned char tones[TOTAL_BEATS];

// this tells you how long each column of LEDs will be on for (in microseconds)
#define MATRIX_DISPLAY_TIME_PER_COLUMN 1500
// this is how long it takes for the device to display for one cycle.
#define MATRIX_DISPLAY_TIME_PER_CYCLE  (MATRIX_DISPLAY_TIME_PER_COLUMN*8)

//----------------------------------------------------------------------------------------------
// pins
//----------------------------------------------------------------------------------------------

// misc pins
#define p_test_LED			PB0	// this is test LED. It is handy to have a test LED to output a bit every once in a while.  :3

// pin that is used for tone outputs
#define p_tone				PB1	// this is the pin that tones will be generated on

// the following pins are for the 8x8 LED Matrix
#define p_shift_clock		PC0	// This is the shift register's clock input (rising edge active)
#define p_shift_data		PC1	// This is the shift register's data input.
								
#define p_enable_matrix		PC2	// This pin enables the LED Matrix. The memory of the decade counter and the shift register is retained even when the display is disabled.
								// To turn off the display, set this pin to 0. To enable the display, set this pin to 1.
								
#define p_decade_reset		PC3	// This is the reset pin of the decade counter. active high. Reset = 1. Allow Counting = 0.
#define p_decade_clock		PC4	// This is the clock pin of the decade counter. Rising Edge Active.
								// The decade counter will overflow and return to the 1st states after the 10th state NOT THE EIGHTH STATE.
								// therefore, in order to properly use the display, it is necessary to reset the decade counter when the ninth state is reached.

// PORTD will be used as the output port.

//----------------------------------------------------------------------------------------------
// define some useful functions
//----------------------------------------------------------------------------------------------

// input output functions are defined here
#define low(port, pin) port &= ~(1<<pin)				// this sets a certain pin low.
#define high(port, pin) port |= (1<<pin)				// this sets a certain pin high.
#define set_input(portdir, pin) portdir &= ~(1<<pin)	// this sets a certain pin as an input.
#define set_output(portdir, pin) portdir |= (1<<pin)	// this sets a certain pin as an output.

// delay functions are renamed
#define delay(time) _delay_ms(time);					// this delays for some time in milliseconds
#define delayus(microseconds) _delay_us(microseconds);	// this delays for some time in microseconds

// functions for the 8x8 LED Matrix
#define matrix_reset_decade() high(PORTC, p_decade_reset); low(PORTC, p_decade_reset);	// this resets the state of the decade counter to the first state
#define matrix_clock_decade() low(PORTC, p_decade_clock); high(PORTC, p_decade_clock);	// this clocks the decade counter
#define matrix_clock_shift()  low(PORTC, p_shift_clock);   high(PORTC, p_shift_clock);	// this clocks the shift register
#define matrix_off() low(PORTC, p_enable_matrix);		// disable matrix display
#define matrix_on() high(PORTC, p_enable_matrix);		// enable matrix display

// function prototypes used in main() (see them described fully below)
void shift(int,int,int);
void matrix_print(int);
void advance_song(int);

//----------------------------------------------------------------------------------------------
// timers and interrupts
//----------------------------------------------------------------------------------------------

// global variable to count the number of overflows
// volatile means that this variable can change between consecutive statements.
// (because this variable can be modified by an interrupt)
volatile uint8_t timer0_overflow;
// this tells you how many times timer0 must overflow before the offset is incremented
unsigned char speed = 4;

// initialize timer0, interrupt and variable
void timer0_init()
{
	//----------------------------------------------------------------------------------------------
	// timer 0
	//----------------------------------------------------------------------------------------------
	// set up timer with prescaler = 1024
	// if the clock speed is 8MHz, then the clock speed Timer0 gets a source clock of 7.81 kHz
	TCCR0B |= (1 << CS02) | (1 << CS00);
	
	// initialize counter
	TCNT0 = 0;
	
	// enable compare interrupt
	TIMSK0 |= (1 << OCIE0A);
	
	// set the initial compare value of timer0 to max. now there will be an interrupt about 30.5 times per second.
	OCR0A = 0xff;
	
	// enable global interrupts
	sei();
	
	// initialize overflow counter variable
	timer0_overflow = 0;
}

void timer1_init(){
	
	TCCR1A |= (1 << COM1A0);	// timer1 toggles OC1A (pin 1 on PORTB) when TCNT1 reaches OCR1A
	TCCR1B |= (1 << CS10);		// timer1 has a prescaler of 8
	TCCR1B |= (1 << WGM12);		// timer1 works in CTC mode. (Clear Timer on Compare Match)
	TCNT1H = TCNT1L = 0;		// reset the timer1 count.
	OCR1AH = 0x8e;				// set initial high byte of the output compare register
	OCR1AL = 0x08;				// set initial low  byte of the output compare register
	OCR1BH = 0xff;				// set timerB to max so it doesn't interfere with timer A.
	OCR1BL = 0xff;				// ^
}

// this function sets timer1's output-compare registers so that pin PB1 will play the intended tone.
void tone(float freq){
	if(freq == f_same) return;						// if there is no change in the frequency, don't make a change.
	if(freq == f_none){
		TCCR1A &= ~(1 << COM1A0);					// doesn't output anymore
		return;
	}
	TCNT1H = 0;										// reset the timer1 count
	TCNT1L = 0;
	TCCR1A |= (1 << COM1A0);						// timer1 toggles OC1A (pin 1 on PORTB) when TCNT1 reaches OCR1A
	static float basePeriod = 1/((float)F_CPU);		// calculate timer1's base period
	float halfPeriod = 1/(2.0*freq);				// calculate the halfperiod of the target sound to be synthesized
	float cycles = halfPeriod/basePeriod;			// calculate how many cycles timer1 has to iterate through
	int cyclesInt= (int)cycles;						// convert cycles to an integer type
	
	OCR1AH = cyclesInt / 0x100;						// set high byte of the output compare register
	OCR1AL = cyclesInt % 0x100;						// set low  byte of the output compare register
}

// whenever timer0 overflows, this function is evaluated
// this function moves the song along
ISR(TIMER0_COMPA_vect)
{
	// keep a track of number of overflows
	timer0_overflow++;
	if(timer0_overflow >= speed){
		timer0_overflow = 0;			// reset timer0_overflow to 0
		advance_song(1);				// move the song ahead by 1 beat
		PORTD = song[offset];			// set port D to the current song data
		tone(get_freq(tones[offset]));	// play a certain tone
	}
}

//----------------------------------------------------------------------------------------------
// initialize the song data
//----------------------------------------------------------------------------------------------
void init_song_data(){
	tones[0] = n_A4;
	tones[1] = n_B4;
	tones[2] = n_C5;
	tones[3] = n_A4;
	tones[4] = n_E5;
	tones[5] = n_A4;
	tones[6] = n_B4;
	tones[7] = n_C5;
	
	tones[8]  = n_A4;
	tones[9]  = n_B4;
	tones[10] = n_C5;
	tones[11] = n_A4;
	tones[12] = n_F5;
	tones[13] = n_A4;
	tones[14] = n_B4;
	tones[15] = n_C5;
	
	tones[16] = n_A4;
	tones[17] = n_B4;
	tones[18] = n_C5;
	tones[19] = n_A4;
	tones[20] = n_G5;
	tones[21] = n_A4;
	tones[22] = n_B4;
	tones[23] = n_C5;
	
	tones[24] = n_A4;
	tones[25] = n_B4;
	tones[26] = n_C5;
	tones[27] = n_A4;
	tones[28] = n_A5;
	tones[29] = n_A4;
	tones[30] = n_B4;
	tones[31] = n_C5;
	
	//copy and manipulate the song data
	int i = 1;
	for(i=0; i<32 && i<TOTAL_BEATS; i++){
		tones[i+32] = tones[i];
		if(i%8 == 0 || i%8 == 3 || i%8 == 5)
		tones[i+32] -= 2;
	}
	for(i=0; i<32 && i<TOTAL_BEATS; i++){
		tones[i+64] = tones[i];
		if(i%8 == 0 || i%8 == 3 || i%8 == 5)
		tones[i+64] -= 4;
	}
	for(i=0; i<16 && i<TOTAL_BEATS; i++){
		tones[i+96] = tones[i];
		if(i%8 == 0 || i%8 == 3 || i%8 == 5)
		tones[i+96] -= 7;
	}
	for(i=0; i<16 && i<TOTAL_BEATS; i++){
		tones[i+96+16] = tones[i+16];
		if(i%8 == 0 || i%8 == 3 || i%8 == 5)
		tones[i+96+16] -= 5;
	}
	
	
	//tones[8] = n_;
	//tones[9] = n_;
	//tones[10] = n_;
	//tones[11] = n_;
	
	//play first tone
	tone(get_freq(tones[0]));	// play a certain tone
}


//----------------------------------------------------------------------------------------------
// main section of the code
//----------------------------------------------------------------------------------------------


int main(void)
{
	
	//----------------------------------------------------------------------------------------------
	// initial setup
	//----------------------------------------------------------------------------------------------
	
	// set the 8x8 LED Matrix pins as outputs
	set_output(DDRC, p_shift_clock);
	set_output(DDRC, p_shift_data);
	set_output(DDRC, p_enable_matrix);
	set_output(DDRC, p_decade_reset);
	set_output(DDRC, p_decade_clock);
	
	set_output(DDRB, p_tone);
	
	DDRD = 0xff;		// make all pins on port D outputs
	
	// set all shift register and decade counter outputs to low
	low(PORTC,p_decade_clock);
	low(PORTC,p_decade_reset);
	low(PORTC,p_shift_data);
	low(PORTC,p_shift_clock);
	// disable matrix
	matrix_off();
	// reset decade counter state
	matrix_reset_decade();
	
	//infinite loop of turning the LED on and off.
// 	while(1){
// 		high(PORTB,p_test_LED);
// 		delay(1);
// 		low(PORTB, p_test_LED);
// 		delay(1);
// 	}
	
	
	//initialize the timer(s)
	timer0_init();
	timer1_init();
	init_song_data();
// 	set up variables for frequency control
// 	int freqCounter = 0;
// 	int extraTime = 1;
// 	while(1){
// 		if(timer0_overflow >= extraTime){
// 			PORTD ^= (1 << 0);	// toggle LED
// 			TCNT0 = 0;			// reset timer0.
// 			timer0_overflow = 0;	// reset overflow
// 			freqCounter++;
// 			if(freqCounter >= 20){
// 				OCR0A++;			// increment frequency.
// 				freqCounter = 0;
// 				if(OCR0A == 0){
// 					extraTime++;
// 				}
// 			}
// 		}
// 	}
	int i;
	// set the song data
	for(i=0; i<TOTAL_BEATS-8; i+=8){
		song[i+0] = 1;
		song[i+1] = 2;
		song[i+2] = 4;
		song[i+3] = 8;
		song[i+4] = 16;
		song[i+5] = 32;
		song[i+6] = 64;
		song[i+7] = 128;
		if(i%32 == 24){
			song[i+4] |= 1;
			song[i+6] |= 1;
		}
		if(i%32 == 8){
			song[i+4] |= 1;
		}
	}
	PORTD = song[0];
	while(1){
		matrix_print(offset);	// display the current song data
		delay(5);				// delay for a shirt period as to not waste processing power
		
	}
	
}

//most significant bit first
void shift(int data, int datapin, int clockpin){
//void shift(int data, int datapin, int dataport, int clockpin, int clockport){
	low(PORTC, clockpin);
	
	int x= 128;
	while(x>0){
		if(data&x) high(PORTC, datapin);
		else         low (PORTC, datapin);
		high(PORTC, clockpin);
		low(PORTC, clockpin);
		x = x>>1; // divide by two
	}
	
}


// This will print data to the 8x8 LED Matrix. the "int offset" input is what BEAT the user is on.
// the offset integer is used to index into the song[] array.
// this matrix print function will display a measure line as well
void matrix_print(int songOffset){
	
	matrix_off();								// display matrix
	matrix_reset_decade();						// reset decade counter state
	
	unsigned char i;							// this increments through the song[] array
	// loop through each column
	for(i=0; i<BEATS_PER_MEASURE; i++){
		shift(song[offset+i], p_shift_data, p_shift_clock);	// shift out the data for this column
		matrix_on();													// turn on the matrix
		delayus(MATRIX_DISPLAY_TIME_PER_COLUMN);						// delay for a specified period of time (to allow the LEDs to turn on and be visible for some time)
		matrix_off();													// turn the matrix off
		matrix_clock_decade();											// clock the decade counter to advance to the next column of LEDs
	}
}


// this will add beatDiff number of beats to the current song offset.
void advance_song(int beatDiff){
	offset += beatDiff;
	if(offset >= 128) offset = 0;
}


