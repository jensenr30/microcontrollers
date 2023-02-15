/*
 * Noise_Generator.c
 *
 * Created: 11/5/2013 4:05:30 PM
 *  Author: Ryan Jensen
 */ 
//----------------------------------------------------------------------------------------------
// headers and globals
//----------------------------------------------------------------------------------------------

#define F_CPU 8000000UL			// define the clock speed as 8MHz. This clock speed is used in the delay.h header file. This does not actually change the clock speed of the device.
#include <avr/io.h>				// this is the basic Input output of AVR microcontrollers I think
#include <util/delay.h>			// this allows me to use the delay functions
#include <avr/interrupt.h>		// this allows me to use interrupt functions
#include <stdlib.h>				// this is included for the rand() function

//----------------------------------------------------------------------------------------------
// main section of the code
//----------------------------------------------------------------------------------------------

int main(void)
{
	
	DDRA = 0xff;			// set all port A pins as outputs
	
	// infinite loop of random output
    while(1)
    {
        PORTA = rand()%256;		//assign a random value to port A
    }
}