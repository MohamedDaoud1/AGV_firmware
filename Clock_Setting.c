	/*
	System Clock Setting:
		1- Using given equations (Page 292 in the datasheet), find the values
		of Q,N,MINT,MFRAC,PSYSDIV
		2- Write these values to registers: PLLFREQ0 & PLLFREQ1
		3- Set MEMTIM0 register as indicated in page 277 in the datasheet.
		4- Write PSYSDIV value to RSCLKCFG register
		5- Select PLL source
		6- Enable PLL
		7- Enable MEMTIM0 Changes
		8- Enable NEWFREQ
	*/

#include "Clock_Setting.h"
#include "TM4C1292NCPDT.h"                    // Device header


void SetSysClock()
{
	// System Clock Setting
	SYSCTL->PLLFREQ0 |= 0x0000001E;		// Setting MINT Value
	SYSCTL->PLLFREQ0 |= 0x00800000;		// Power Up PLL
	
	SYSCTL->MEMTIM0	&= ~(0x01 <<5);							// Clear MEMTIM0 Register
	SYSCTL->MEMTIM0	&= ~(0x01 <<21);						// Clear MEMTIM0 Register
	SYSCTL->MEMTIM0 |= (0x00050005);	// FBCHT/EBCHT Setting
	SYSCTL->MEMTIM0 |= (0x06 << 6);		// FWS Setting
	SYSCTL->MEMTIM0 |= (0x06 << 22);	// EWS Setting
	
	while(!SYSCTL->PLLSTAT);					// Wait until PLL is ready
	
	SYSCTL->RSCLKCFG |= 0x03;					// Update PSYSDIV
	SYSCTL->RSCLKCFG |= (0x01 << 28);	// Enable use PLL bit
	SYSCTL->RSCLKCFG |= (1ul << 31);	// Enable MEMTIM0 Register Changes
	SYSCTL->RSCLKCFG |= (1ul << 30);	// Enable NEWFREQ bit
}
