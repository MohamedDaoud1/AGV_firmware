/*----------------------------------------------------------------------------
 * Name:    PORT_P.c
 * Purpose: Low Level PORT P Functions & Initialization
 * Note(s): Motor Drivers Direction Control
 *----------------------------------------------------------------------------*/

#ifndef PORT_P_H
#define PORT_P_H
 
extern void InitPortP(void);
extern void SteeringMotor(uint8_t Dir);
extern void RightMotor(uint8_t Dir);
extern void LeftMotor(uint8_t Dir);

#endif
