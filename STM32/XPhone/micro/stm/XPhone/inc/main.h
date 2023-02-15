#ifndef MAIN_H_DEF
	#define MAIN_H_DEF
	#include "stm32f4xx.h"
	#include "stm32f4xx_nucleo_144.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include "key.h"
	// primary things about hardware
	#define STM_PROGRAM 	1				// define STM_PROGRAM if it should compile for an stm32 microprocessor for the actual XPhone hardware.
	
	// clock system
	#define CPU_FREQ ((uint32_t)100000000)			// 100 MHz (10 ns timebase)
	
	KeyTimeType currentTime;	// this is used to store the current time of the song. It is initialized to a value that SongTime will never be, so that the while() wait loops fails and sets it right away.
	
	#define wait_100ns() asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP")
	
	void XPhone_online();
	
	#endif // MAIN_H_DEF
