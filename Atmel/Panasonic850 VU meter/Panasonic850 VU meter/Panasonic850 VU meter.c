/*
 * Panasonic850_VU_meter.c
 *
 * Created: 12/20/2013 2:16:52 PM
 *  Author: Ryan
 *
 * ABOUT
 * 
 * This program uses the Panasonic 850 calculator (circa 1972) as a
 * VU meter using the numbers 1, 3, and 8. Connections have been made
 * to the calculator to facilitate indirect control of the calculator's
 * vacuum fluorescent display. The microcontroller has the ability to
 * emulate key presses (The microcontroller has access to emulate the
 * 1, 3, 8, and clear keys). I thought that having indirect access to
 * the calculator's display would be easier, but there are some
 * drawbacks to doing this. Basically, you need to serial shift numbers
 * into the calculator's "stack" of numbers. This configuration
 * essentially makes the calculator into a shift register for numbers
 * in a string. Quite interesting...
 * 
 */ 

//---------------------------------------------------------------------------------------------------
// HEADERS.
//---------------------------------------------------------------------------------------------------
#include <avr/io.h>			// input/output function of the microcontroller.

#define F_CPU 8000000UL		// definition for delay.h. This does NOT change the microcontroller's clock frequency.  :P
#include <util/delay.h>		// delay functions.

#define calcLatency 20000	// the latency of the calculator in microseconds.

//---------------------------------------------------------------------------------------------------
// PORT AND PIN CONFIG.
//---------------------------------------------------------------------------------------------------
#define myPort		PORTA
#define myPortDDR	DDRA
#define p_clear		PA0
#define p_1			PA1
#define p_3			PA2
#define p_8			PA3
#define p_sig_in	PA4

//---------------------------------------------------------------------------------------------------
// FUNCTIONS.
//---------------------------------------------------------------------------------------------------
// this function will pulse (turn on and off) an input to the calculator.
// the input is NOT the number you want to print. is it pin number that needs to be pulsed.
void calculator(int pin){
	// make sure the pin is valid.
	if(pin == p_clear || pin == p_1 || pin == p_3 || pin == p_8){
		myPort |=  (1 << pin);	// turn the pin on.
		_delay_us(calcLatency);		// wait for some time.
		myPort &= ~(1 <<pin);	// turn the pin off.
		_delay_us(calcLatency);
	}
}

//---------------------------------------------------------------------------------------------------
// MAIN.
//---------------------------------------------------------------------------------------------------
int main(void)
{
	
	//---------------------------------------------------------------------------------------------------
	// SETTING INPUT/OUTPUT CHARACTERISTICS FOR THE PINS.
	//---------------------------------------------------------------------------------------------------
	myPortDDR |=  (1 << p_clear);	// clear is an output.
	myPortDDR |=  (1 << p_1);		// 1	 is an output.
	myPortDDR |=  (1 << p_3);		// 3	 is an output.
	myPortDDR |=  (1 << p_8);		// 8	 is an output.
	myPortDDR &= ~(1 << p_sig_in);	// the input audio signal is and INPUT (obviously  :P).
	
	//---------------------------------------------------------------------------------------------------
	// MAIN CODE LOOP.
	//---------------------------------------------------------------------------------------------------
    int testDelay = 250;
	while(1)
    {
        calculator(p_clear);
		_delay_ms(testDelay);
		calculator(p_1);
		_delay_ms(testDelay);
		calculator(p_3);
		_delay_ms(testDelay);
		calculator(p_8);
		_delay_ms(testDelay);
    }
}