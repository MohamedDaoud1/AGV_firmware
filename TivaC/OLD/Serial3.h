/*----------------------------------------------------------------------------
 * Name:    Serial3.h
 * Purpose: Low level serial definitions
 * Note(s):
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

#ifndef SERIAL3_H
#define SERIAL3_H

extern void SER3_Initialize(void);
extern int  SER3_GetChar   (void);
extern int  SER3_PutChar   (int c);

#endif
