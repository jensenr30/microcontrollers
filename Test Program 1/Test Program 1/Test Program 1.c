/*
 * Test_Program_1.c
 *
 * Created: 10/6/2013 4:30:50 PM
 *  Author: Ryan
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define p_shift_clock PB1 
#define p_shift_data PB2
#define p_enable_matrix PB3
#define p_decade_reset PB4
#define p_decade_clock PB5

#define output_low(port, pin) port &= ~(1<<pin)
#define output_high(port, pin) port |= (1<<pin)
#define set_input(portdir, pin) portdir &= ~(1<<pin)
#define set_output(portdir, pin) portdir |= (1<<pin)
#define delay(time) _delay_ms(time);

int main(void)
{
    while(1)
    {
		/* make sounds
        output_high(PORTB, Speaker);
		_delay_us((int)usperiodby2);
		output_low(PORTB, Speaker);
		_delay_us((int)usperiodby2);
		*/
		output_high(PORTB, LED);
		delay(1000);
		output_low(PORTB, LED);
		delay(1000);
    }
}