/*----------------------------------------------------------------------------
 * Name:    PORT_F.c
 * Purpose: Low Level PWM1, PWM2, PWM3 Module Routines
 * Author: 	Mohamed Ashraf Gamal Eldin
 * Date: 	04-05-2018
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_F.h"

/*
PWM Pins in TM4C1292NCPDT:
	Pin | PWM#  | PCTL Register Value	|			RCGCGPIO			|	AFSEL
	---------------------------------------------------------------------------------------
	PF0	| PWM01 | PCTL |= 0x00000006	|	SYSCTL->RCGCGPIO |= 0x0020	|	AFSEL|= 0x0001
	PF1	| PWM02	| PCTL |= 0x00000060	|	SYSCTL->RCGCGPIO |= 0x0020	|	AFSEL|= 0x0002
	---------------------------------------------------------------------------------------
	PF2	| PWM03	| PCTL |= 0x00000600	|	SYSCTL->RCGCGPIO |= 0x0020	|	AFSEL|= 0x0004
	PF3	| PWM04	| PCTL |= 0x00006000	|	SYSCTL->RCGCGPIO |= 0x0020	|	AFSEL|= 0x0008
	---------------------------------------------------------------------------------------
	PG0	| PWM05	| PCTL |= 0x00000006	|	SYSCTL->RCGCGPIO |= 0x0040	|	AFSEL|= 0x0001
	PG1	| PWM06	| PCTL |= 0x00000060	|	SYSCTL->RCGCGPIO |= 0x0040	|	AFSEL|= 0x0002
	---------------------------------------------------------------------------------------
	PK4	| PWM07	| PCTL |= 0x00060000	|	SYSCTL->RCGCGPIO |= 0x0200	|	AFSEL|= 0x0010
	PK5	| PWM08	| PCTL |= 0x00600000	|	SYSCTL->RCGCGPIO |= 0x0200	|	AFSEL|= 0x0020

=========================================================================================================
	
PWM Clock Configuration:
	USEPWM Value Description:
		- 0 The system clock is the source of PWM unit clock.		PWMx->CC &= ~0x0000		then 	|=0x0000 
		- 1 The PWM clock divider is the source of PWM unit clock.	PWMx->CC &= ~0x0100		then 	|=0x0100 
	
	PWMDIV Value Description:
	Value Description
		- 0x0 ==> /2		PWMx->CC &= ~0x0000		then 	|=0x0000 
		- 0x1 ==> /4		PWMx->CC &= ~0x0001		then 	|=0x0001
		- 0x2 ==> /8		PWMx->CC &= ~0x0002		then 	|=0x0002
		- 0x3 ==> /16		PWMx->CC &= ~0x0003		then 	|=0x0003
		- 0x4 ==> /32		PWMx->CC &= ~0x0004		then 	|=0x0004
		- 0x5 ==> /64		PWMx->CC &= ~0x0005		then 	|=0x0005	
		- 0x6 - 0x7 reserved
	
*/

/*----------------------------------------------------------------------------
  Initialize PWM02:
	  PF1	| PWM02	| PCTL |= 0x00000060	|	SYSCTL->RCGCGPIO |= 0x0020	|	AFSEL|= 0x0002
	  Frequency = 15 kHz (System Clock Frequency = 120 MHz)
	  
	  Load_Value = System_Clock/(PWM_Dividor * PWM_Frequency)
	  Load_Value = 120 Mhz / (8 * 15 kHz)
	  Load_Value = 1000
 *----------------------------------------------------------------------------*/
void InitPortF(void)
{
	Init_PWM1_PF0_PF1();
	Init_PWM1_PF2();
}
void Init_PWM1_PF0_PF1(void)
{	
	// Enable Peripheral Clocks
	SYSCTL->RCGCPWM |= 0x01;	// Enable clock to PWM0 Module
	SYSCTL->RCGCGPIO |= 0x0020;	// Enable clock to PORTF

	// Enable Port PF0 for PWM0 M0PWM0
	GPIOF_AHB->AFSEL |= 0x01;		// PF0 uses alternate function
	GPIOF_AHB->PCTL &= ~0x0000000F;	// Make PF0 PWM output pin
	GPIOF_AHB->PCTL |=  0x00000006;	// Make PF0 PWM output pin
	GPIOF_AHB->DEN |= 0x01;			// Pin digital
	
	// Enable Port PF1 for PWM0 M0PWM1
	GPIOF_AHB->AFSEL |= 0x02;		// PF1 uses alternate function
	GPIOF_AHB->PCTL &= ~0x000000F0;	// Make PF1 PWM output pin
	GPIOF_AHB->PCTL |=  0x00000060;	// Make PF1 PWM output pin
	GPIOF_AHB->DEN |= 0x02;			// Pin digital
	
	PWM0->_0_CTL = 0;			// Stop Counter

	PWM0->CC &= ~0x0100;	// The PWM clock divider is the source of PWM unit clock
	PWM0->CC |=  0x0100;	// The PWM clock divider is the source of PWM unit clock
	PWM0->CC &= ~0x0002;	// /8 Divider
	PWM0->CC |=  0x0102;	// /8 Divider
	
	PWM0->_0_GENA = 0x0000008C;	// M0PWM0 Outpet set when reload 
								/* Comparator A makes the action. Clear when match PWMCMPA */
	PWM0->_0_GENB = 0x0000080C;	// M0PWM1 Outpet set when reload 
								/* Comparator B makes the action. Clear when match PWMCMPB */							
	PWM0->_0_LOAD = 1000;		// Set Load value for 15kHz (120 MHz/8 * 15kHz)
	PWM0->_0_CMPA = 999;		// Set duty cycle to 0%
	PWM0->_0_CMPB = 999;		// Set duty cycle to 0%
	PWM0->_0_CTL = 1;			// Start Timer
	PWM0->ENABLE |= 0x03;		// Start PWM0 CH0 & CH1
}

void Init_PWM1_PF2(void)
{	
	// Enable Peripheral Clocks
	SYSCTL->RCGCPWM |= 0x01;	// Enable clock to PWM0 Module
	SYSCTL->RCGCGPIO |= 0x0020;	// Enable clock to PORTF

	// Enable Port PF2 for PWM0 M0PWM0
	GPIOF_AHB->AFSEL |= 0x04;		// PF0 uses alternate function
	GPIOF_AHB->PCTL &= ~0x00000F00;	// Make PF0 PWM output pin
	GPIOF_AHB->PCTL |=  0x00000600;	// Make PF0 PWM output pin
	GPIOF_AHB->DEN |= 0x04;			// Pin digital
	
	PWM0->_1_CTL = 0;			// Stop Counter

	PWM0->CC &= ~0x0100;	// The PWM clock divider is the source of PWM unit clock
	PWM0->CC |=  0x0100;	// The PWM clock divider is the source of PWM unit clock
	PWM0->CC &= ~0x0002;	// /8 Divider
	PWM0->CC |=  0x0102;	// /8 Divider
	
	PWM0->_1_GENA = 0x0000008C;	// M1PWM0 Outpet set when reload 
								/* Comparator A makes the action. Clear when match PWMCMPA */							
	PWM0->_1_LOAD = 1000;		// Set Load value for 15kHz (120 MHz/8 * 15kHz)
	PWM0->_1_CMPA = 999;		// Set duty cycle to 0%
	PWM0->_1_CTL = 1;			// Start Timer
	PWM0->ENABLE |= 0x04;		// Start PWM1 CH0
} 
 
  
void SetDuty_PWM3(uint16_t Duty)
{
	/* The decreasing in CMPA value causes the duty cycle to lengthen */
	//if(Duty < 0) Duty = 0;			// Check that the duty cycle is within the range
	if(Duty > 999) Duty = 999;	// Check that the duty cycle is within the range
	PWM0->_0_CMPA = 999 - Duty; 	// Higher duty cycles require smaller value in CMPA
}
 
void SetDuty_PWM2(uint16_t Duty)
{
	/* The decreasing in CMPB value causes the duty cycle to lengthen */
	//if(Duty < 0) Duty = 0;			// Check that the duty cycle is within the range
	if(Duty > 999) Duty = 999;	// Check that the duty cycle is within the range
	PWM0->_0_CMPB = 999 - Duty; 	// Higher duty cycles require smaller value in CMPB
}

void SetDuty_PWM1(uint16_t Duty)
{
	/* The decreasing in CMPA value causes the duty cycle to lengthen */
	//if(Duty < 0) Duty = 0;			// Check that the duty cycle is within the range
	if(Duty > 999) Duty = 999;	// Check that the duty cycle is within the range
	PWM0->_1_CMPA = 999 - Duty; 	// Higher duty cycles require smaller value in CMPA
}
