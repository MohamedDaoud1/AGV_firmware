/*----------------------------------------------------------------------------
 * Name:    SSI2.c
 * Purpose: Low Level SSI2 Module Routines
 * Author: Mohamed Ashraf Gamal Eldin
 * Date: 20-03-2018
 *----------------------------------------------------------------------------*/


#include <TM4C129.h>
#include "SSI2.h"

/*----------------------------------------------------------------------------
  Initialize SSI2 pins, Frequency
	  PD3 CLK, PD1 MOSI(Tx), PD0 MISO(Rx), POL = 0, PHA=0, clock = 10.43 kHz
 *----------------------------------------------------------------------------*/
 
void init_SSI2(void)
{
	SYSCTL->RCGCSSI  |= 0x04; // Enable clock to SSI2
	SYSCTL->RCGCGPIO |= 0x08; // Enable clock to GPIOD for SSI2
	SYSCTL->RCGCGPIO |= 0x20; // Enable clock to GPIOF for slave select
	
	// Configure portD 3, 1 for SSI clock and Rx
	GPIOD_AHB->AMSEL &= ~0x0F; // Disable analog for these pins
	GPIOD_AHB->DEN |= 0x0F; // Make these pins digital
	GPIOD_AHB->AFSEL |= 0x0F; // Enable alternate function
	GPIOD_AHB->PCTL &= ~0x0000FFFF; // Assign pins to SSI
	GPIOD_AHB->PCTL |= 0x0000FFFF;  // Assign pins to SSI
	
	// Configure PortF 2 for slave select
	GPIOF_AHB->DEN |= 0x04; // Make the pin digital
	GPIOF_AHB->DIR |= 0x04; // Make the pin output
	GPIOF_AHB->DATA |= 0x04; // Keep SS idle high
	
	// SPI Master, POL = 0, PHA=0, clock = 10.43 kHz, 16 Bit data
	SSI2->CR1 = 0; // Disable SSI and make it master
	SSI2->CC = 0; // Use system clock
	SSI2->CPSR = 0x32; //0x24; // Prescaler divided by 50 (decimals)
	SSI2->CR0 = 0x2A4F; // 10.43 kHz SSI clock, SPI mode, 16 bit data
	SSI2->CR1 |= 2; // Enable SSI1
}

uint16_t Read_SSI2(void)
{
	GPIOF_AHB->DATA |= 0x04; // Keep SS idle high
	GPIOF_AHB->DATA&= ~0x04; // Assert SS low
	while((SSI2->SR & 2) == 0); // Wait until FIFO not full
		SSI2->DR=0;		// Transmit Byte
	while(SSI2->SR & 0x10); // Wait until transmit complete
	GPIOF_AHB->DATA |= 0x04; // Keep SS idle high
	uint16_t data = SSI2->DR ;		// Get Data
	return (data);
}