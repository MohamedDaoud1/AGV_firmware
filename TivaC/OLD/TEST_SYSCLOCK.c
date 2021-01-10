#include "TM4C1292NCPDT.h"                    // Device header
#include "Clock_Setting.h"                    // Clock_Setting header
#include "PWM_Enable.h"


/*
	SysTick is a 24 Bit counter used to generate time delay
	Load Value = ( Fvco /  4 )  * Required Time 
*/
int main(void)
{
	SetSysClock();
	SYSCTL->RCGCGPIO |= 0x0020;	// Enable clock to PORTF
	GPIOF_AHB->DIR |= 0x02;			// Pin F1 digital
	GPIOF_AHB->DIR |= 0x04;			// Pin  F2 digital
	GPIOF_AHB->DEN |= 0x06;			// Pins F1 & F2 digital
	GPIOF_AHB->DATA ^= 0x02;
	//SysTick->LOAD = 0xB71B00; 	// Count 0.1 S
	SysTick->LOAD = 0x000078; 	// Freq. = 1 Mhz
	SysTick->CTRL = 0x05;				// Use System Clock & Enable SysTick
	while(1)
	{
		while(!(SysTick->CTRL & 0x10000)); // Wait until the flag goes high
		GPIOF_AHB->DATA ^= 0x02; // Toggle PF1 Pin
	}
	return 0;
}
