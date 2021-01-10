#include "TM4C1292NCPDT.h"                    // Device header
#include "BT_Control.h"
#include "Print_Num.h"
#include "PORTs/PORT_A.h"
#include "PORTs/PORT_B.h"
#include "PORTs/PORT_K.h"
#include "PORTs/PORT_F.h"
#include "PORTs/PORT_P.h"
/*
	Driving Motors:
		PWM1 & PWM2
	Forward Directions:
		RightMotor = 0x02
		LeftMotor = 0x01
	Backward Directions:
		RightMotor = 0x01
		LeftMotor = 0x02
	
	Steering Motor:
		PWM3
	Right Direction:
		SteeringMotor = 0x01
	Left Direction:
		SteeringMotor = 0x02
*/

void BT_TakeAction(char Msg)
{
	switch(Msg)
	{
		case 'F':
			SetDuty_PWM1(999);
			SetDuty_PWM2(999);
			SetDuty_PWM3(0);
			SteeringMotor(0x00);
			RightMotor(0x02);
			LeftMotor(0x01);
			break;
		case 'B':
			SetDuty_PWM1(999);
			SetDuty_PWM2(999);
			SetDuty_PWM3(0);
			SteeringMotor(0x00);
			RightMotor(0x01);
			LeftMotor(0x02);
			break;
		case 'L':
			SetDuty_PWM1(0);
			SetDuty_PWM2(0);
			if(!SteeringLS1())
			{
				SetDuty_PWM3(999);
				SteeringMotor(0x02);
			}
			else
			{
				SetDuty_PWM3(0);
				SteeringMotor(0x00);
			}
			RightMotor(0x00);
			LeftMotor(0x00);
			break;
		case 'R':
			SetDuty_PWM1(0);
			SetDuty_PWM2(0);
			if(!SteeringLS2())
			{
				SER0_PutChar(55);
				SetDuty_PWM3(999);
				SteeringMotor(0x01);
			}
			else
			{
				SetDuty_PWM3(0);
				SteeringMotor(0x00);
			}
			RightMotor(0x00);
			LeftMotor(0x00);
			break;
		case 'S':
			SetDuty_PWM1(0);
			SetDuty_PWM2(0);
			SetDuty_PWM3(0);
			SteeringMotor(0x00);
			RightMotor(0x00);
			LeftMotor(0x00);
			break;
		case 'W':
			Siren(1);
			break;
		case 'w':
			Siren(0);
			break;
		case 'V':
			Lamp(1);			
			break;
		case 'v':
			Lamp(0);
			break;	
		default: break;
	}
	
}

void Stop()
{
	SetDuty_PWM1(0);
	SetDuty_PWM2(0);
	SetDuty_PWM3(0);
	SteeringMotor(0x00);
	RightMotor(0x00);
	LeftMotor(0x00);
	Lamp(0);
	Siren(0);
}
