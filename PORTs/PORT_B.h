/*----------------------------------------------------------------------------
 * Name:    PORT_B.h
 * Purpose: Low Level PORT B GPIO Initialization
 * Note(s): None
 *----------------------------------------------------------------------------*/
 
#ifndef PORT_B_H
#define PORT_B_H

extern void InitPortB(void);
//extern char DriveMode(void);
extern void Siren(uint8_t State);
extern void Lamp(uint8_t State);


#endif
