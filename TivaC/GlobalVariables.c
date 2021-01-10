
#include "TM4C1292NCPDT.h"                    // Device header
#include "GlobalVariables.h"

int8_t T_MSG_OUT[11];
int8_t T_MSG_IN[9];
int8_t RxInd = 0;
int8_t SpeedR = 0;
int8_t SpeedL = 0;
char REC =0;

int32_t REncoder = 0;	// Right Encoder pulses counter
int32_t LEncoder = 0;	// Left Encoder pulses counter
int32_t oldREncoder = 0;	// Right Encoder pulses counter
int32_t oldLEncoder = 0;	// Left Encoder pulses counter

int16_t CurrentDistanceR = 0;	// Current distance
int16_t CurrentDistanceL = 0;	// Current distance

double IncEnFactor = 0;	// Factor used in Encoder Signal Conditionin
uint64_t hundMill = 0;		// Hundred MilliSecond 
uint64_t hundM = 0;		// Hundred MicroSecond 
/* 
	64 bit Time counter that counts pulses generated every 100 uS. 
	It can counts up to 1.844* 10 ^ 15 seconds ( 58.5 Million Year ! ) to overflow.
*/
