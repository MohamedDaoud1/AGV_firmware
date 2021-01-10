
#ifndef GlobalVariables_H
#define GlobalVariables_H

extern int8_t T_MSG_OUT[11];
extern int8_t T_MSG_IN[9];
extern int8_t RxInd;
extern char REC;
extern int32_t REncoder;	// Right Encoder pulses counter
extern int32_t LEncoder;	// Left Encoder pulses counter
extern int8_t SpeedR;
extern int8_t SpeedL;
extern int32_t oldREncoder;	// Right Encoder pulses counter
extern int32_t oldLEncoder;	// Left Encoder pulses counter

extern int16_t CurrentDistanceR;	// Current distance
extern int16_t CurrentDistanceL;	// Current distance

extern double IncEnFactor;	// Factor used in Encoder Signal Conditionin

extern uint64_t hundMill;		// Hundred MilliSecond 
extern uint64_t hundM; 			// Hundred MicroSecond 
/* 
	64 bit Time counter that counts pulses generated every 100 uS. 
	It can count up to 1.844* 10 ^ 15 seconds ( 58.5 Million Year ! ) to overflow.
*/

#endif
