
/*
	Data to be sent:
	- Starting Byte ('R')			=> 1 Bytes
	- Speed (-128 - 127) cm/s 		=> 1 Byte
	- Distance (-32768 - 32767) m	=> 2 Bytes
	- Steering Sensor				=> (10 Bits)
	- Line Follower					=> (10 Bits)
	- Steering Limit Switches		=> 2 Bits
	- Ending Byte ('E')				=> 1 Bytes
	--------------------------------------------
	Total Number of bits = 62 bit 	=> 8 Bytes
*/

/*
	T_MSG_OUT[0]: 'R' Message header
	T_MSG_OUT[1]: Robot Right Wheel Speed
	T_MSG_OUT[2]: Robot Right Wheel Position (part 1/2)
	T_MSG_OUT[3]: Robot Right Wheel Position (part 2/2)	
	T_MSG_OUT[4]: Robot Left Wheel Speed
	T_MSG_OUT[5]: Robot Left Wheel Position (part 1/2)
	T_MSG_OUT[6]: Robot Left Wheel Position (part 2/2)
	T_MSG_OUT[7]: Steering Sensor reading (8/10) most significant bits
	T_MSG_OUT[8]: Steering Sensor reading (2/10) least significant bits & Line follower sensor 1
	T_MSG_OUT[9]: Line follower sensor 2 & Steering Limit switches
	T_MSG_OUT[10]: 'E' End of the message
*/

/*
	T_MSG_IN[0]: 'C' Message header
	T_MSG_IN[1]: Siren, Lamp Control, Motor Directions,
	T_MSG_IN[2]: Steering Motor PWM  (1/2)
	T_MSG_IN[3]: Steering Motor PWM 1 (2/2)
	T_MSG_IN[4]: Right Motor PWM (1/2)
	T_MSG_IN[5]: Right Motor PWM (2/2)
	T_MSG_IN[6]: Left Motor PWM (1/2)
	T_MSG_IN[7]: Left Motor PWM (2/2)
	T_MSG_IN[8]: 'E' End of the message
*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "RB_Protocol.h"
#include "Print_Num.h"
#include "PORTs/PORT_A.h"
#include "PORTs/PORT_B.h"
#include "PORTs/PORT_F.h"
#include "PORTs/PORT_P.h"
#include "PORTs/PORT_E.h"
#include "PORTs/PORT_N.h"
#include "PORTs/PORT_D.h"
#include "PORTs/PORT_K.h"
#include "GlobalVariables.h"


void ClearT_MSG_IN(void)
{
	for(uint8_t i = 0 ; i < 9; i++)
	{
		T_MSG_IN[i] = 0;
	}
	RxInd=0;
}


void CreateMsg(void)
{
	for(char k = 0; k < 8; k++)
	{
		T_MSG_OUT[k] = 0;
	}
	T_MSG_OUT[0] = 'R'; // Readings
	T_MSG_OUT[1] = SpeedR;
	T_MSG_OUT[2] = (CurrentDistanceR >> 8);
	T_MSG_OUT[3] = (CurrentDistanceR & 0xFF);
	T_MSG_OUT[4] = SpeedL;
	T_MSG_OUT[5] = (CurrentDistanceL >> 8);
	T_MSG_OUT[6] = (CurrentDistanceL & 0xFF);
	uint16_t SStrPos = Read_SSI2();
	char LF1 = ReadLF1() &0x0F;
	char LF2 = ReadLF2() &0x1E;
	char LS = (SteeringLS2() << 2 | (SteeringLS1() << 1) | BackLS());
	T_MSG_OUT[7] = (SStrPos >> 2);
	T_MSG_OUT[8] = ((SStrPos & 0x003) << 6) | (LF1 << 1);
	T_MSG_OUT[9] = ((LF2 << 3) | (LS));
	T_MSG_OUT[10] = 'E'; // END OF THE MESSAGE
}

void SendReadings(void)
{
	for(uint8_t c = 0; c < 11 ; c++)
	{
		SER3_PutChar(T_MSG_OUT[c]);
	}
}

void PrintMSG(void)
{
	for(char u = 0; u < 9;u++)
	{
		SER0_PutChar(T_MSG_IN[u]);
	}
	SER0_PutChar('\n');
	SER0_PutChar('\r');
}
	

void HandleInputMSG(void)
{
	PrintMSG();
	if((T_MSG_IN[0] == 'C') && (T_MSG_IN[8] == 'E'))
	{
		REC=0;
		Siren(T_MSG_IN[1] >> 7);
		Lamp((T_MSG_IN[1] >> 6) & 0x01);
		RightMotor(((T_MSG_IN[1] >> 2) & 0x03));
		LeftMotor(((T_MSG_IN[1] >> 4) & 0x03));
		PrintDEC_Monitor(((T_MSG_IN[1] >> 2) & 0x03));
		PrintDEC_Monitor(((T_MSG_IN[1] >> 4) & 0x03));
		SetDuty_PWM1((T_MSG_IN[4] <<8) | T_MSG_IN[5]); // Left Motor
		SetDuty_PWM2((T_MSG_IN[6] <<8) | T_MSG_IN[7]); // Right Motor
		SetDuty_PWM3((T_MSG_IN[2] <<8) | T_MSG_IN[3]);
		SteeringMotor((T_MSG_IN[1]) & 0x03);
		//PrintDEC_Monitor((T_MSG_IN[2] <<8) | T_MSG_IN[3]);
	}
	ClearT_MSG_IN();
}
