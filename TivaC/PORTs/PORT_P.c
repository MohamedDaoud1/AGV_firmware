/*----------------------------------------------------------------------------
 * Name:    PORT_P.c
 * Purpose: Low Level PORT P Functions & Initialization
 * Note(s): Motor Drivers Direction Control
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_P.h"

void InitPortP(void)
{
	SYSCTL->RCGCGPIO |= ( 1ul << 13); // Enable clock to GPIOP
	GPIOP->DEN |= 0x3F; // Make (PP0, PP1, PP2, PP3, PP4, PP5) pins digital 
	GPIOP->DIR |= 0x3F; // Make (PP0, PP1, PP2, PP3, PP4, PP5) pins output
	GPIOP->DATA = 0;
}

void SteeringMotor(uint8_t Dir)
{
	GPIOP->DATA &= ~(3 << 0);	// Clear previous direction/braking
	GPIOP->DATA |= (Dir << 0);	// Set new direction/braking
}

void RightMotor(uint8_t Dir)
{
	GPIOP->DATA &= ~(3 << 2);	// Clear previous direction/braking
	GPIOP->DATA |= (Dir << 2);	// Set new direction/braking
}

void LeftMotor(uint8_t Dir)
{
	GPIOP->DATA &= ~(3 << 4);	// Clear previous direction/braking
	GPIOP->DATA |= (Dir << 4);	// Set new direction/braking
}
