/*
 * Test_Program_1.c
 *
 * Created: 10/6/2013 4:30:50 PM
 *  Author: Ryan
 */ 

//headers
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

//pins
#define p_test_LED PB0
#define p_shift_clock PB1 
#define p_shift_data PB2
#define p_enable_matrix PB3
#define p_decade_reset PB4
#define p_decade_clock PB5

//input output
#define low(port, pin) port &= ~(1<<pin)
#define high(port, pin) port |= (1<<pin)
#define set_input(portdir, pin) portdir &= ~(1<<pin)
#define set_output(portdir, pin) portdir |= (1<<pin)

//delay
#define delay(time) _delay_ms(time);
#define delayus(microseconds) _delay_us(microseconds);

// functions
void shift(int,int,int);

int main(void)
{
	set_output(DDRB, p_shift_clock);
	set_output(DDRB, p_shift_data);
	set_output(DDRB, p_enable_matrix);
	set_output(DDRB, p_decade_reset);
	set_output(DDRB, p_decade_clock);
	set_output(DDRB, p_test_LED);
	//force all outputs to low
	low(PORTB,p_decade_clock);
	low(PORTB,p_decade_reset);
	low(PORTB,p_shift_data);
	low(PORTB,p_shift_clock);
	//enable matrix
	high(PORTB, p_enable_matrix);
	//reset decade counter state
	low(PORTB, p_decade_reset);
	high(PORTB, p_decade_reset);
	low(PORTB, p_decade_reset);
	
	int i = 1;
    while(1)
    {
		//turn on the matrix and shift out i
		high(PORTB, p_enable_matrix);
		shift(i, p_shift_data, p_shift_clock);
		
		//turn on and off the LED and delay
		high(PORTB, p_enable_matrix);
		delay(100);
		low(PORTB, p_test_LED);
		delay(100);
		high(PORTB, p_test_LED);
		
		//increment the number being shifted variable
		i++;
		
		//disable the matrix while switching columns.
		low(PORTB, p_enable_matrix);
		
		//reset the matrix display column when necessary.
		if(i%8 == 1){
			high(PORTB, p_decade_reset);
			low(PORTB, p_decade_reset);
		}
		//otherwise clock the decade counter
		else{
			
			high(PORTB, p_decade_clock);
			low(PORTB, p_decade_clock);
		}
    }
}

//most significant bit first
void shift(int data, int datapin, int clockpin){
	low(PORTB, clockpin);
	
	int x= 128;
	while(x>0){
		if(data&x) high(PORTB, datapin);
		else         low (PORTB, datapin);
		high(PORTB, clockpin);
		low(PORTB, clockpin);
		x = x>>1; // divide by two
	}
	
}