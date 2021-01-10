/*----------------------------------------------------------------------------
 * Name:    PORT_E.c
 * Purpose: Low Level PORT E Functions & Initialization
 * Note(s): Line follower 1 Reading
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_E.h"

void InitPortE(void)
{
	SYSCTL->RCGCGPIO |= ( 1ul << 4); // Enable clock to GPIOE
	GPIOE_AHB->DEN |= 0x3F; // Make (PE0, PE1, PE2, PE3, PE4, PE5) pins digital 
	GPIOE_AHB->DIR |= 0x20; // Make (PE5) pin output
}

char ReadLF1(void)
{
	return(GPIOE_AHB->DATA&0x1F);	// Read (PE0, PE1, PE2, PE3, PE4) pins
}
