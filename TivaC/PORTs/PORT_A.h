/*----------------------------------------------------------------------------
 * Name:    PORT_A.h
 * Purpose: Low Level PORT A Functions & Initialization
 * Note(s): output redirect to UART Modules
 *----------------------------------------------------------------------------*/
#ifndef PORT_A_H
#define PORT_A_H


extern void InitPortA(void);

extern void SER0_Initialize(void);
extern int  SER0_GetChar   (void);
extern void SER0_PutChar   (int c);

extern void SER2_Initialize(void);
extern int  SER2_GetChar   (void);
extern void  SER2_PutChar   (int c);

extern void SER3_Initialize(void);
extern int  SER3_GetChar   (void);
extern void  SER3_PutChar   (int c);

#endif
