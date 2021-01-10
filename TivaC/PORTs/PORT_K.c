/*----------------------------------------------------------------------------
 * Name:    PORT_K.c
 * Purpose: Low Level PORT B GPIO Initialization
 * Note(s): None
 *----------------------------------------------------------------------------*/
 

#include <TM4C1292NCPDT.h>
#include "PORT_K.h"

void InitPortK(void)
{
	SYSCTL->RCGCGPIO |= (1ul << 9); // Enable clock to GPIOK
	GPIOK->DEN |= 0x07; // Make (PK0, PK1, PK2) pins digital 
}

char SteeringLS1(void)
{
	if(GPIOK->DATA & 0x01)	// Read PK0 pin
		return (1);
	else
		return (0);

}

char SteeringLS2(void)
{
	if(GPIOK->DATA & 0x02)	// Read PK1 pin
		return (1);
	else
		return (0);
}

char BackLS(void)
{
	if(GPIOK->DATA & 0x04)	// Read PK2 pin
		return (1);
	else
		return (0);
}
