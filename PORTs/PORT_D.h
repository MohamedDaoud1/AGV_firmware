/*----------------------------------------------------------------------------
 * Name:    PORT_D.c
 * Purpose: Low Level SSI2 Module Routines & ADC Routines
 * Author:	Mohamed Ashraf Gamal Eldin
 * Date: 	12-06-2018
 *----------------------------------------------------------------------------*/
#ifndef PORT_D_H
#define PORT_D_H
 
 extern void InitPortD(void);
 extern void init_SSI2(void);
 extern uint16_t Read_SSI2(void);
 extern void init_ADC(void);
 extern void ADC_SelectChannel(int *Mat);
 extern uint16_t ADC_READ(void);
 
 #endif

