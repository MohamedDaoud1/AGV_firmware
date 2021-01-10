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
#ifndef TimerA1_H
#define TimerA1_H




extern void InitializeTimer1A(void);
extern void TIMER1A_Handler(void);

#endif
