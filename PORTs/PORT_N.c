/*----------------------------------------------------------------------------
 * Name:    PORT_N.c
 * Purpose: Low Level PORT N Functions & Initialization
 * Note(s): Line follower 2 Reading
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_N.h"

void InitPortN(void)
{
	SYSCTL->RCGCGPIO |= ( 1ul << 12); // Enable clock to GPION
	GPION->DEN |= 0x3F; // Make (PN0, PN1, PN2, PN3, PN4, PN5) pins digital 
	GPION->DIR |= 0x20; // Make (PN5) pin output
}

char ReadLF2(void)
{
	return(GPION->DATA&0x1F);	// Read (PN0, PN1, PN2, PN3, PN4) pins
}
