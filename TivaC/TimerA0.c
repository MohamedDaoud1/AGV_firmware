/*
CTL		: Control
CFG		: Configuration
TAMR	: Timer A Mode
TAILR	: Timer A Interval Load
TAPR	: Timer A Prescale
ICR		: Interrupt Clear 
IMR		: Interrupt Mask
MIS		: GPTM Masked Interrupt Status
*/


#include "TM4C1292NCPDT.h"                    // Device header
#include "TimerA0.h"                    // Device header
#include "GlobalVariables.h"


void InitializeTimer0A(void)
{
	/* Desired Freq. = */
	SYSCTL->RCGCTIMER |= 1;	// Enable clock to Timer Block 0	
	TIMER0->CTL = 0;	//	Disable Timer before initialization
	TIMER0->CFG = 0x04;	//	16-Bit Option
	TIMER0->TAMR = 0x02;	//	Periodic mode and down-counter
	TIMER0->TAPR = 250; // Timer A prescaler value
	TIMER0->TAILR = 48;// Timer A interval load value register
	TIMER0->ICR = 0x1;	// Clear the TimerA timeout flag
	TIMER0->IMR |= 0x01;	// Enable Timer A timeout interrupt
	TIMER0->CTL |= 0x01;	// Enable Timer A after initialization
	NVIC->ISER[0] |= (1ul << 19);	// Enable IRQ19 (Timer 0 Interrupt)
	NVIC->IP[19] = 1 << 5; // Set Timer interrupt priority to 1	(TIMER0A)
}

void TIMER0A_Handler(void)
{
	volatile int readback;
	if(TIMER0->MIS & 0x1)
	{
		hundM++;
		// do something
		TIMER0->ICR = 0x01;	// Clear the Timer A timeout flag
		readback = TIMER0->ICR; //	A read ti force clearing of interrupt flag
	}
	else // should not get here, but if we do
	{
		TIMER0->ICR = TIMER0->MIS; // Clear all flags
		readback = TIMER0->ICR;	// A read ti force clearing of interrupt flag
	}
}
