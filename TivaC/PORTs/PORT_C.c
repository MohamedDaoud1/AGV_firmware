/*----------------------------------------------------------------------------
 * Name:    PORT_C.c
 * Purpose: Low Level PORT C Functions & Initialization
 * Note(s): Encoder  A reading
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_C.h"
#include "../Print_Num.h"
#include "../GlobalVariables.h"
#include "../HandleIncEn.h"

/*
PC4:	Interrupt	Channel Z
PC5:	Interrupt	Channel A
PC6:	GPIO		Channel B
*/
void InitPortC(void)
{
  SYSCTL->RCGCGPIO |= 0x04; // Enable clock for portC
  // PORTC Pin 4 & 5 have special function, need to unlock to modify
  GPIOC_AHB->LOCK = 0x4C4F434B; 	// Unlock commit register
  GPIOC_AHB->CR = 0x20;				// make PORTC 1 configurable
  GPIOC_AHB->LOCK = 0x00;			// Lock commit	register
  
  // Configure PORTC for Encoder 1 Input
  GPIOC_AHB->DIR &= ~0x70;		// make PORTC (PC4 ,PC5 ,PC6) input for 3 Channels Encoder
  GPIOC_AHB->DEN |= 0x70;		// make PORTC (PC4 ,PC5 ,PC6) digital pins
  //  GPIOE_AHB->PUR |= 0x06;		// enable pull up for PORTE
  
   // Configure PORTC 1 for rising edge trigger interrupt
  GPIOC_AHB->IS  &= ~0x20;	// make bit 1 edge sensitive (Interrupt Sense Register)
	// Setting a bit in the GPIOIS register configures the 
	// corresponding pin to detect levels, while clearing a 
	// bit configures the corresponding pin to detect edges.
  GPIOC_AHB->IBE &= ~0x20;	// trigger is controlled by IEV (Interrupt Both Edges Register)
  GPIOC_AHB->IEV |= 0x20;	// Rising edge trigger
  GPIOC_AHB->ICR |= 0x20;	// Clear any prior interrupt
  GPIOC_AHB->IM  |= 0x20;	// Unmask interrupt
  
   // Enable interrupt in NVIC and set priority to 3
  NVIC->IP[2] = 2 << 5; // Set interrupt priority to 3  (PORTC)
  
  // ISER[0] = EN0 Covers IRQs from 0 to 31
  // ISER[1] = EN1 Covers IRQs from 32 to 63
  // Set a bit to enable the corresponding interrupt
  NVIC->ISER[0] |= ( 1ul << 2); // Enable IRQ2 (PORTC Interrupt)
  __enable_irq(); // global enable IRQs
}


void GPIOC_Handler(void)
{
	// Quadrature Encoder, if pin PC6 is HIGH increment if LOW decrement
	if((GPIOC_AHB->DATA&0x40))
	{
		REncoder++;
	}
	else
	{
		REncoder--;
	}

	volatile int readback;
	GPIOC_AHB->ICR |= 0x20; // Clear interrupt flag
	readback = GPIOC_AHB->ICR; // A read to force clearing of interrupt flag
}
