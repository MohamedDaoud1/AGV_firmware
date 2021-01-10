/*----------------------------------------------------------------------------
 * Name:    PORT_B.c
 * Purpose: Low Level PORT B GPIO Initialization
 * Note(s): None
 *----------------------------------------------------------------------------*/
 
/*
	PB0:	Choosing between manual & automatic modes (Input -> DIR = 0)
		If PB0 is HIGH => Automatic
		If PB0 is LOW  => Manual
	PB3:	Siren Control (Output -> DIR = 1)
	PB2:	Lamp Control  (Output -> DIR = 1)
*/

#include <TM4C1292NCPDT.h>
#include "PORT_B.h"
#include "../Print_Num.h"

void InitPortB(void)
{
	SYSCTL->RCGCGPIO |= 0x02; // Enable clock to GPIOB
	GPIOB_AHB->DEN |= 0x0D; // Make (PB0, PB2, PB3) pins digital 
	GPIOB_AHB->DIR |= 0x0C; // Make (PB3, PB2) pins output
	GPIOB_AHB->DATA &= ~0x08;
	GPIOB_AHB->DATA |= 0x04;
}

void Siren(uint8_t State)
{
	if(State)
		GPIOB_AHB->DATA |= 0x08;	// Set PB3 HIGH
	else
		GPIOB_AHB->DATA &= ~0x08;	// Set PB3 LOW
}

void Lamp(uint8_t State)
{
	if(State && (GPIOB_AHB->DATA))
		GPIOB_AHB->DATA &= ~0x04;	// Set PB2 LOW (NC)
	else
		GPIOB_AHB->DATA |= 0x04;	// Set PB2 HIGH (NC)
}
