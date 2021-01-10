/*----------------------------------------------------------------------------
 * Name:    PORT_D.c
 * Purpose: Low Level SSI2 Module Routines & ADC Routines
 * Author:	Mohamed Ashraf Gamal Eldin
 * Date: 	12-06-2018
 *----------------------------------------------------------------------------*/


#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_D.h"

/*----------------------------------------------------------------------------
  Initialize SSI2 pins, Frequency
	  PD3 CLK, PD1 MOSI(Tx), PD0 MISO(Rx), PD2 (SS), POL = 0, PHA=0, clock = 10.43 kHz
 *----------------------------------------------------------------------------*/
 
void InitPortD(void)
{
	 init_SSI2();
	 init_ADC();
}
 
void init_SSI2(void)
{
	SYSCTL->RCGCSSI  |= 0x04; // Enable clock to SSI2
	SYSCTL->RCGCGPIO |= 0x08; // Enable clock to GPIOD for SSI2
	
	// Configure portD 3, 1 for SSI clock and Rx
	
	GPIOD_AHB->AMSEL &= ~0x0F; // Disable analog for these pins
	GPIOD_AHB->DEN |= 0x0F; // Make these pins digital
	GPIOD_AHB->AFSEL |= ( 1ul << 3 | 1ul << 1 | 1ul << 0); // Enable alternate function
	GPIOD_AHB->PCTL &= ~0x0000F0FF; // Assign pins to SSI
	GPIOD_AHB->PCTL |= 0x0000F0FF;  // Assign pins to SSI
	
	// Configure PortF 2 for slave select
	GPIOD_AHB->DIR |= ( 1ul << 2); // Make the pin output
	GPIOD_AHB->DATA |= ( 1ul << 2); // Keep SS idle high
	
	// SPI Master, POL = 0, PHA=0, clock = 10.43 kHz, 16 Bit data
	SSI2->CR1 = 0; // Disable SSI and make it master
	SSI2->CC = 0; // Use system clock
	SSI2->CPSR = 0x32;//0x3C;//0x32; //0x24; // Prescaler divided by 50 (decimals)
	SSI2->CR0 = 0x2A4F;//0xFF4F;//0x2A4F; // 10.43 kHz SSI clock, SPI mode, 16 bit data
	SSI2->CR1 |= 2; // Enable SSI1
}

uint16_t Read_SSI2(void)
{
	GPIOD_AHB->DATA |= 0x04; // Keep SS idle high
	GPIOD_AHB->DATA&= ~0x04; // Assert SS low
	while((SSI2->SR & 2) == 0); // Wait until FIFO not full
		SSI2->DR=0;		// Transmit Byte
	while(SSI2->SR & 0x10); // Wait until transmit complete
	GPIOD_AHB->DATA |= 0x04; // Keep SS idle high
	uint16_t data = SSI2->DR ;		// Get Data
	return ((data & 0x7FE0) >> 5);
}

/*----------------------------------------------------------------------------
  Initialize ADC pins
		PD7 => AIN4
		PD6 => AIN5
		PD5 => AIN6
		PD4 => AIN7
 *----------------------------------------------------------------------------*/
 

void init_ADC(void)
{
	SYSCTL->RCGCADC |= 1; // Enable clock to ADC0
	
	GPIOD_AHB->AFSEL |= 0xF0;	// Enable alternate function
	GPIOD_AHB->DEN &= ~0xF0;	// Disable digital function
	GPIOD_AHB->AMSEL |= 0xF0;	// Enable Analog function
	
	/* Initialize ADC0 */
	ADC0->ACTSS &= ~2;		// Disable SS1 during configuration
	ADC0->EMUX &= ~0xF000;	// Software trigger conversion
	/*
	PD7 => AIN4
	PD6 => AIN5
	PD5 => AIN6
	PD4 => AIN7
	*/
	/* Choose th input channel in ADC_SelectChannel function */ 
	//ADC0->SSMUX3 = 0;		// Get input from channel
	ADC0->SSMUX1 |= (7 << 12);
	ADC0->SSMUX1 |= (6 << 8);
	ADC0->SSMUX1 |= (5 << 4);
	ADC0->SSMUX1 |= (4 << 0);
	ADC0->SSEMUX1 &= ~((1ul << 12) | (1ul << 8) | (1ul << 4) | (1ul << 0));
	ADC0->SSCTL1 |= ((4ul << 12) | (4ul << 8) | (4ul << 4) | (4ul << 0));		// Take one sample at a time, set flag at 1st sample
	ADC0->ACTSS |= 2;		// Enable ADC0 sequencer 1
}

void ADC_SelectChannel(int *Mat)
{
	
	ADC0->PSSI |= 2;	/* Start conversion sequencer 3*/
	while((ADC0->RIS & 0x02) == 0);		/* Wait for sequencer 3 conversion result (interrupt flag polling) */
	Mat[0] = ADC0->SSFIFO1;	/* Read conversion result */
	ADC0->ISC = 2; 	/* Clear completion flag */
	while((ADC0->RIS & 0x02) == 0);		/* Wait for sequencer 3 conversion result (interrupt flag polling) */
	Mat[1] = ADC0->SSFIFO1;	/* Read conversion result */
	ADC0->ISC = 2; 	/* Clear completion flag */
	while((ADC0->RIS & 0x02) == 0);		/* Wait for sequencer 3 conversion result (interrupt flag polling) */
	Mat[2] = ADC0->SSFIFO1;	/* Read conversion result */
	ADC0->ISC = 2; 	/* Clear completion flag */
	while((ADC0->RIS & 0x02) == 0);		/* Wait for sequencer 3 conversion result (interrupt flag polling) */
	Mat[	3] = ADC0->SSFIFO1;	/* Read conversion result */
	ADC0->ISC = 2; 	/* Clear completion flag */
	/*// AIN4 (PD7)
	ADC0->ACTSS &= ~8;	// Disable SS3 during configuration
	ADC0->SSMUX3 = 4;		// Get input from channel AIN4 (PD7)
	ADC0->ACTSS |= 8;		// Enable ADC0 sequencer 3
	Mat[0]=ADC_READ();
	
	// AIN5 (PD6)
	ADC0->ACTSS &= ~8;	// Disable SS3 during configuration
	ADC0->SSMUX3 = 5;		// Get input from channel AIN5 (PD6)
	ADC0->ACTSS |= 8;		// Enable ADC0 sequencer 3
	Mat[1]=ADC_READ();
	
	// AIN6 (PD5)
	ADC0->ACTSS &= ~8;	// Disable SS3 during configuration
	ADC0->SSMUX3 = 6;		// Get input from channel AIN6 (PD5)
	ADC0->ACTSS |= 8;		// Enable ADC0 sequencer 3
	Mat[2]=ADC_READ();
	
	// AIN7 (PD4)
	ADC0->ACTSS &= ~8;	// Disable SS3 during configuration
	ADC0->SSMUX3 = 7;		// Get input from channel AIN7 (PD4)
	ADC0->ACTSS |= 8;		// Enable ADC0 sequencer 3
	Mat[3]=ADC_READ();*/
}

uint16_t ADC_READ(void)
{	
	ADC0->PSSI |= 8;	/* Start conversion sequencer 3*/
	while((ADC0->RIS & 0x08) == 0);		/* Wait for sequencer 3 conversion result (interrupt flag polling) */
	uint16_t result = ADC0->SSFIFO3;	/* Read conversion result */
	ADC0->ISC = 8; 	/* Clear completion flag */
	return result;
}
