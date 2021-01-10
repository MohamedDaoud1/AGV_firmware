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
#ifndef TimerA0_H
#define TimerA0_H




extern void InitializeTimer0A(void);
extern void TIMER0A_Handler(void);

#endif
