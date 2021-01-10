/*----------------------------------------------------------------------------
 * Name:    PORT_H.c
 * Purpose: Low Level PORT H Functions & Initialization
 * Note(s): Encoder  B reading
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_H.h"
#include "../Print_Num.h"
#include "../GlobalVariables.h"

/*
PH0:	Interrupt	Channel Z
PH1:	Interrupt	Channel A
PH2:	GPIO		Channel B
*/
void InitPortH(void)
{
  SYSCTL->RCGCGPIO |= 0x0080;	// Enable clock to PORTH
  
  // PORTH Pins 0,1 have special function, need to unlock to modify
  GPIOH_AHB->LOCK = 0x4C4F434B; 	// Unlock commit register
  GPIOH_AHB->CR = 0x02;				// make PORTH 0,1 configurable
  GPIOH_AHB->LOCK = 0x00;			// Lock commit	register
  
  // Configure PORTH for Encoder 1 Input
  GPIOH_AHB->DEN |= 0x07;		// make PORTH (PH0 ,PH1 ,PH2) digital pins
  GPIOH_AHB->DIR &= ~(1 << 0 | 1 << 1 | 1 << 2);		// make PORTH (PH0 ,PH1 ,PH2) input for 3 Channels Encoder
  
   // Configure PORTH 0,1 for rising edge trigger interrupt
  GPIOH_AHB->IS  &= ~0x02;	// make bit 1 edge sensitive (Interrupt Sense Register)
	// Setting a bit in the GPIOIS register configures the 
	// corresponding pin to detect levels, while clearing a 
	// bit configures the corresponding pin to detect edges.
  GPIOH_AHB->IBE &= ~0x02;	// trigger is controlled by IEV (Interrupt Both Edges Register)
  GPIOH_AHB->IEV |= 0x02;	// Rising edge trigger
  GPIOH_AHB->ICR |= 0x02;	// Clear any prior interrupt
  GPIOH_AHB->IM  |= 0x02;	// Unmask interrupt
  
   // Enable interrupt in NVIC and set priority to 3
  NVIC->IP[32] = 2 << 5; // Set interrupt priority to 3  (PORTH)
  
  // ISER[0] = EN0 Covers IRQs from 0 to 31
  // ISER[1] = EN1 Covers IRQs from 32 to 63
  // Set a bit to enable the corresponding interrupt
  NVIC->ISER[1] |= ( 1ul << 0); // Enable IRQ32 (PORTH Interrupt)
  __enable_irq(); // global enable IRQs
}

void GPIOH_Handler(void)
{
	// Quadrature Encoder, if pin PC6 is HIGH increment if LOW decrement
	if((GPIOH_AHB->DATA&0x04))
	{
		LEncoder++;
	}
	else
	{
		LEncoder--;
	}
	volatile int readback;
	GPIOH_AHB->ICR |= 0x02; // Clear interrupt flag
	readback = GPIOH_AHB->ICR; // A read to force clearing of interrupt flag
}
