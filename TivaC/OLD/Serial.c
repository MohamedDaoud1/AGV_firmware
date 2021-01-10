/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: Low Level Serial Routines
 * Note(s): output redirect to UART0
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2014 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <TM4C129.h>
#include "Serial.h"

/*
 U A R T          V A L U E S          T A B L E
BAUDRATE	|	IBRD	|	FBRD	|	CLK		|
------------|-----------|-----------|-----------|
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
      PA0 U0_RX, PA1 U0_TX, 1.5 Mhz @ 120 MHz, 8 N 1
 *----------------------------------------------------------------------------*/
void SER_Initialize (void) 
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
int SER_PutChar (int c) 
{
  while ((UART0->FR & (1ul << 7)) == 0x00);
  UART0->DR = (c & 0xFF);

  return (c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_GetChar (void) 
{
  while ((UART0->FR & (1ul << 4)) != 0x00);
  return (UART0->DR & 0xFF);
}
