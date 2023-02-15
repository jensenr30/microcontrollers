#include "debug.h"
#include "GPIO.h"
#include "UART.h"



//=============================================================================
// this function happens when something FATAL happens. The program cannot continue, and it will freeze here indefinitely.
// this can be used as a breakpoint when you are debugging to see WHY the error happened.
//=============================================================================
void error(char *message)
{
	pin_on(DEBUG_ERROR_LED_GPIO, DEBUG_ERROR_LED);
	printf("ERROR!  ");
	printn(message);
	while (1) {;} // freeze program
}


//=============================================================================
// this function is called when something bad happens, but it isn't the end of the world, and the xylophone will keep on playing.
// it is intended to be used as a breakpoint when you are debugging WHY the warning happened.
//=============================================================================
void warning(char *message)
{
	pin_on(DEBUG_WARNING_LED_GPIO, DEBUG_WARNING_LED);
	printf("WARNING:  ");
	printn(message);
	return;
}
