/*----------------------------------------------------------------------------
 * Name:    Serial3.c
 * Purpose: Low Level Serial Routines
 * Note(s): output redirect to UART3
 *----------------------------------------------------------------------------*/

#include <TM4C129.h>
#include "Serial3.h"
#include "Serial.h"
/*
 U A R T          V A L U E S          T A B L E
BAUDRATE	|	IBRD	|	FBRD	|	CLK		|
------------|-----------|-----------|-----------|
38400		|	195		|	 20		|  120 Mhz	|
115200		|	  8		|	 44		|	16 Mhz	|
115200		|	 65		|	  7		|  120 Mhz	|
1000000		|	  5		|	  0		|  120 Mhz	|
1500000		|	  7		|	 32		|  120 Mhz	|

Equation:
Baudrate_Dividor = CLK / (16 * BAUDRATE)
IBRD = integer part of Baudrate_Dividor
FBRD = Fractional part of Baudrate_Dividor * 64
*/

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
      PA4 U3_RX, PA5 U3_TX, Baudrate = 1.5 Mhz @ 120 MHz, 8 N 1
 *----------------------------------------------------------------------------*/
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
  UART3->IBRD = 5;//32;//7;//5;//8;//65; //8;	 // UART Integer Baud Rate Divisor
  UART3->FBRD = 0;//35;//32;//0;//44;//7;//44;	 // UART Fractional Baud Rate Divisor
  UART3->LCRH = (3ul << 5);                     /* 8 bits                   */
  UART3->CC   = 0;                              /* use system clock         */
  UART3->CTL |=  ((1ul << 9) | (1ul << 8));       /* enable RX, TX            */
  UART3->CTL |=   (1ul << 0);                     /* enable UART              */
  
  /* Enable interrupt in NVIC and set priority to 3 */
  NVIC->IP[56] = 3 << 5 ; /* Set interrupt priority to 3 */
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
		SER_PutChar(c);
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
int SER3_PutChar (int c) 
{
  while ((UART3->FR & (1ul << 7)) == 0x00);
  UART3->DR = (c & 0xFF);

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER3_GetChar (void) 
{
  while ((UART3->FR & (1ul << 4)) != 0x00);
  return (UART3->DR & 0xFF);
}
