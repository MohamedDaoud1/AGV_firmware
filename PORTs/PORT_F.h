/*----------------------------------------------------------------------------
 * Name:    PORT_F.h
 * Purpose: Low Level PWM1, PWM2, PWM3 Module Routines
 * Author: 	Mohamed Ashraf Gamal Eldin
 * Date: 	04-05-2018
 *----------------------------------------------------------------------------*/

#ifndef PORT_F_H
#define PORT_F_H
 
extern void InitPortF(void);
extern void Init_PWM1_PF0_PF1(void);
extern void Init_PWM1_PF2(void);
void SetDuty_PWM1(uint16_t Duty);
void SetDuty_PWM2(uint16_t Duty);
void SetDuty_PWM3(uint16_t Duty);

#endif
