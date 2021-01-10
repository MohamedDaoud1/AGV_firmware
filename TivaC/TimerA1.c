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
#include "TimerA1.h"                    // Device header
#include "GlobalVariables.h"
#include "HandleIncEn.h"
#include "Print_Num.h"
#include "PORTs/PORT_B.h"


/*
	Timer 1A makes an interrupt by a frequency of 10 Hz (10 time per second)(every 100 mSs)
	Interrupt Handler updates Distances values used in speed calculations
*/
/*
	Timer A Interval Load & Prescalar Calculations:
		Desired Interrupt Freq. =  CLK Freq. / (TAILR * TAPR)
		10 Hz = 120 MHz / (250 * TAILR) ==> TAILR = 48000
*/
void InitializeTimer1A(void)
{
	/* Desired Freq. = */
	SYSCTL->RCGCTIMER |= 2;	// Enable clock to Timer Block 1	
	TIMER1->CTL = 0;	//	Disable Timer before initialization
	TIMER1->CFG = 0x04;	//	16-Bit Option
	TIMER1->TAMR = 0x02;//	Periodic mode and down-counter
	TIMER1->TAPR = 250;	// Timer A prescaler value  (120 MHz / 250 = 480 kHz)
	TIMER1->TAILR = 48000;	// Timer A interval load value register
	TIMER1->ICR = 0x1;	// Clear the TimerA timeout flag
	TIMER1->IMR |= 0x01;	// Enable Timer A timeout interrupt
	TIMER1->CTL |= 0x01;	// Enable Timer A after initialization
	NVIC->ISER[0] |= (1ul << 21);	// Enable IRQ21 (Timer 1 Interrupt)
	NVIC->IP[21] = 3 << 5; // Set Timer interrupt priority to 1	(TIMER1A)
}

void TIMER1A_Handler(void)
{
	volatile int readback;
	if(TIMER1->MIS & 0x1)
	{

		hundMill ++;
		UpdateDistances();
		if(hundMill == 10)
		{
			hundMill = 0;
			FindSpeed();
		}
		TIMER1->ICR = 0x01;	// Clear the Timer A timeout flag
		readback = TIMER1->ICR; //	A read to force clearing of interrupt flag
	}
	else // should not get here, but if we do
	{
		TIMER1->ICR = TIMER1->MIS; // Clear all flags
		readback = TIMER1->ICR;	// A read to force clearing of interrupt flag
	}
}
