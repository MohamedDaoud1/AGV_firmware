/*----------------------------------------------------------------------------
 * Name:    PORT_A.c
 * Purpose: Low Level PORT A Functions & Initialization
 * Note(s): output redirect to UART Modules
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "PORT_A.h"
#include "../BT_Control.h"
#include "../RB_Protocol.h"
#include "../Print_Num.h"
#include "../GlobalVariables.h"

/*
 U A R T          V A L U E S          T A B L E
BAUDRATE	|	IBRD	|	FBRD	|	CLK		|
------------|-----------|-----------|-----------|
38400		|	195		|	 20		|  120 Mhz	|
115200		|	  8		|	 44		| 	16 Mhz	|
115200		|	 65		|	  7		|  120 Mhz	|
500000		|	 15		|	  0		|	 120 Mhz	|
1000000		|	  5		|	  0		|  120 Mhz	|
1500000		|	  7		|	 32		|  120 Mhz	|

Equation:
Baudrate_Dividor = CLK / (16 * BAUDRATE)
IBRD = integer part of Baudrate_Dividor
FBRD = Fractional part of Baudrate_Dividor * 64
*/

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate:
    |	UART	|	Rx	|	Tx	|	Baudrate	|			Function		|
	-------------------------------------------------------------------------
	|	UART0	|	PA0	|	PA1	|	1500000		|		PC Communication	|
	|	UART2	|	PA6	|	PA7	|	38400		|	Bluetooth Communication	|
	|	UART3	|	PA4	|	PA5	|	1000000		|	Raspberry Communication	|
 *----------------------------------------------------------------------------*/

 void InitPortA(void)
 {
	SER0_Initialize();	/* Initialize UART0 */
	SER2_Initialize();	/* Initialize UART2 */
	SER3_Initialize();	/* Initialize UART3 */
 }
 
 
 
 void SER0_Initialize (void) 
{
  SYSCTL->RCGCGPIO |=   ( 1ul << 0);             /* enable clock for GPIOs    */
  GPIOA_AHB->DEN   |=  (( 1ul << 0) | ( 1ul << 1));
  GPIOA_AHB->AFSEL |=  (( 1ul << 0) | ( 1ul << 1));
  GPIOA_AHB->PCTL  &= ~((15ul << 0) | (15ul << 4));
  GPIOA_AHB->PCTL  |=  (( 1ul << 0) | ( 1ul << 4));

  SYSCTL->RCGCUART |=  (1ul << 0);               /* enable clock for UART0    */
  SYSCTL->SRUART   |=  (1ul << 0);               /* reset UART0 */
  SYSCTL->SRUART   &= ~(1ul << 0);               /* clear reset UART0 */
  while ((SYSCTL->PRUART & (1ul << 0)) == 0);    /* wait until UART0 is ready */

  UART0->CTL  =   0;                             /* disable UART              */
  UART0->IM   =   0;                             /* no interrupts             */
  UART0->IBRD =   5;//65; //8;					// UART Integer Baud Rate Divisor
  UART0->FBRD =   0;//7;//44;					// UART Fractional Baud Rate Divisor
  UART0->LCRH =   (3ul << 5);                     /* 8 bits                   */
  UART0->CC   =   0;                              /* use system clock         */
  UART0->CTL |=  ((1ul << 9) | (1ul << 8));       /* enable RX, TX            */
  UART0->CTL |=   (1ul << 0);                     /* enable UART              */
}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER0_PutChar (int c) 
{
  while ((UART0->FR & (1ul << 7)) == 0x00);
  UART0->DR = (c & 0xFF);
  //return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER0_GetChar (void) 
{
  while ((UART0->FR & (1ul << 4)) != 0x00);
  return (UART0->DR & 0xFF);
}


//===========================


void SER2_Initialize (void) 
{
  SYSCTL->RCGCGPIO |=   ( 1ul << 0);             /* enable clock for Port A GPIO    */
  GPIOA_AHB->DEN   |=  (( 1ul << 6) | ( 1ul << 7));
  GPIOA_AHB->AFSEL |=  (( 1ul << 6) | ( 1ul << 7));
  GPIOA_AHB->PCTL  &= ~((15ul << 24) | (15ul << 28));	// Clear PCTL register for pins PA6 & PA7
  GPIOA_AHB->PCTL  |=  (( 1ul << 24) | ( 1ul << 28));	// Load PCTL register with 1 for each pin (0x11)

  SYSCTL->RCGCUART |=  (1ul << 2);               /* enable clock for UART2    */
  SYSCTL->SRUART   |=  (1ul << 2);               /* reset UART2 */
  SYSCTL->SRUART   &= ~(1ul << 2);               /* clear reset UART2 */
  while ((SYSCTL->PRUART & (1ul << 2)) == 0);    /* wait until UART2 is ready */

  UART2->CTL  =   0;                             /* disable UART              */
  //UART2->IM   =   0;                             /* no interrupts             */
  UART2->IM   |= (1ul << 4);                      /* Enable RX interrupt             */
  UART2->IBRD = 195;//32;//7;//5;//8;//65; //8;	 // UART Integer Baud Rate Divisor
  UART2->FBRD = 20;//35;//32;//0;//44;//7;//44;	 // UART Fractional Baud Rate Divisor
  UART2->LCRH = (3ul << 5);                     /* 8 bits                   */
  UART2->CC   = 0;                              /* use system clock         */
  UART2->CTL |=  ((1ul << 9) | (1ul << 8));       /* enable RX, TX            */
  UART2->CTL |=   (1ul << 0);                     /* enable UART              */
  
  /* Enable interrupt in NVIC and set priority to 3 */
  NVIC->IP[33] = 2 << 5 ; /* Set interrupt priority to 3 */
  NVIC->ISER[1] |= (1ul << 1); /* Enable IRQ33 (En(1) bit number 1) for UART2 */
  __enable_irq(); /* Global enable IRQs*/
}


void UART2_Handler(void)
{
	volatile int readback;
	char c;
	if (UART2->MIS & 0x0010)
	{
		c = UART2->DR;				/* Read the received data */
		UART2->ICR = 0x0010;		/* Clear Rx interrupt flag */
		readback = UART2->ICR;		/* A read to force clearing of interrupt flag */
		//SER0_PutChar(c);
		//SER3_PutChar(c);
		if(GPIOB_AHB->DATA & 0x01)
			BT_TakeAction(c);
	}
	else /* should not get here. But if it does */
	{
		UART2->ICR = UART2->MIS;	/* Clear all interrupt flags */
		readback = UART2->ICR;		/* A read to force clearing of interrupt flag */
	}
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER2_PutChar (int c) 
{
  while ((UART2->FR & (1ul << 7)) == 0x00);
  UART2->DR = (c & 0xFF);
  //return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER2_GetChar (void) 
{
  while ((UART2->FR & (1ul << 4)) != 0x00);
  return (UART2->DR & 0xFF);
}

//===============================

 void SER3_Initialize (void) 
{
  SYSCTL->RCGCGPIO |=   ( 1ul << 0);             /* enable clock for Port A GPIO    */
  GPIOA_AHB->DEN   |=  (( 1ul << 4) | ( 1ul << 5));
  GPIOA_AHB->AFSEL |=  (( 1ul << 4) | ( 1ul << 5));
  GPIOA_AHB->PCTL  &= ~((15ul << 16) | (15ul << 20));	// Clear PCTL register for pins PA4 & PA5
  GPIOA_AHB->PCTL  |=  (( 1ul << 16) | ( 1ul << 20));	// Load PCTL register with 1 for each pin (0x11)

  SYSCTL->RCGCUART |=  (1ul << 3);               /* enable clock for UART3    */
  SYSCTL->SRUART   |=  (1ul << 3);               /* reset UART3 */
  SYSCTL->SRUART   &= ~(1ul << 3);               /* clear reset UART3 */
  while ((SYSCTL->PRUART & (1ul << 3)) == 0);    /* wait until UART3 is ready */

  UART3->CTL  =   0;                             /* disable UART              */
  //UART3->IM   =   0;                             /* no interrupts             */
  UART3->IM   |= (1ul << 4);                      /* Enable RX interrupt             */
  UART3->IBRD = 7;								 // UART Integer Baud Rate Divisor
  UART3->FBRD = 32;								 // UART Fractional Baud Rate Divisor
  UART3->LCRH = (3ul << 5);                     /* 8 bits                   */
  UART3->CC   = 0;                              /* use system clock         */
  UART3->CTL |=  ((1ul << 9) | (1ul << 8));       /* enable RX, TX            */
  UART3->CTL |=   (1ul << 0);                     /* enable UART              */
  
  /* Enable interrupt in NVIC and set priority to 3 */
  NVIC->IP[56] = 1 << 5 ; /* Set interrupt priority to 1 */
  NVIC->ISER[1] |= (1ul << 24); /* Enable IRQ56 (En(1) bit number 24) for UART3 */
	
  __enable_irq(); /* Global enable IRQs*/
}

void UART3_Handler(void)
{
	volatile int readback;
	char c;
	if (UART3->MIS & 0x0010)
	{
		c = UART3->DR;				/* Read the received data */
		UART3->ICR = 0x0010;		/* Clear Rx interrupt flag */
		readback = UART3->ICR;		/* A read to force clearing of interrupt flag */
		//PrintDEC_Monitor(c);
		//SER0_PutChar(c);
		if(!(GPIOB_AHB->DATA & 0x01))
		{
			T_MSG_IN[RxInd] = c; 
			RxInd++;
			if(c == 'E')
			{
				REC = 1;		
			}			
		}
		//PrintHEX(c);
		//SER0_PutChar('\n');
		//SER0_PutChar('\r');
	}
	else /* should not get here. But if it does */
	{
		UART3->ICR = UART3->MIS;	/* Clear all interrupt flags */
		readback = UART3->ICR;		/* A read to force clearing of interrupt flag */
	}
}

/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER3_PutChar (int c) 
{
  while ((UART3->FR & (1ul << 7)) == 0x00);
  UART3->DR = (c & 0xFF);
  //return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER3_GetChar (void) 
{
  while ((UART3->FR & (1ul << 4)) != 0x00);
  return (UART3->DR & 0xFF);
}
