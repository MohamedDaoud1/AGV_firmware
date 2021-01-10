
#include "TM4C1292NCPDT.h"                    // Device header
#include "HandleIncEn.h"
#include "GlobalVariables.h"
#include "Print_Num.h"

/*
	Distance Calculations in Centimeters:
		- Wheel Radius = 10 cm 
		- Encoder PPR = 100 pulse/rev
	Distance = Radius * 2 * Pi *(RightEncoderPulses + LeftEncoderPulses)/ (2 * PPR)
	Distance = 10 * 2 * Pi *(RightEncoderPulses + LeftEncoderPulses)/ (2 * 100)
	Distance = 0.3141 * (RightEncoderPulses + LeftEncoderPulses)
*/


double InitDistanceFactor(void)
{
	double factor;
	factor = pScale *(2*WheelRadius * Pi)/ (PPR);
	return (factor);
}

int16_t FindDistanceR(void)
{
	int16_t Distance;
	Distance = IncEnFactor * REncoder;
	return Distance;
}

int16_t FindDistanceL(void)
{
	int16_t Distance;
	Distance = IncEnFactor * LEncoder;
	return Distance;
}

void UpdateDistances(void)
{
	CurrentDistanceR = FindDistanceR();
	CurrentDistanceL = FindDistanceL();
}
int8_t Speed2 = 0;
void FindSpeed(void)
{
	//int8_t Speed;
	
	//pOldDistance = pCurrentDistance;
	
	//CurrentDistance = FindDistance();
	
	//Speed = 0.6 * Speed + 0.4 * IncEnFactor * ((REncoder - oldREncoder) + (REncoder - oldREncoder));
	SpeedR = IncEnFactor * (REncoder - oldREncoder);
	SpeedL = IncEnFactor * (LEncoder - oldLEncoder);
	//Speed = IncEnFactor * ((REncoder - oldREncoder) + (REncoder - oldREncoder));
	//Speed = 0.6 * Speed2 + 0.4 * Speed;
	oldREncoder = REncoder;
	oldLEncoder = LEncoder;
	//Speed = (0.5 * Speed)/*25 **/ + 0.5 * (pCurrentDistance - pOldDistance);
	//Speed = (pCurrentDistance - pOldDistance);
	
	/*double SpeedTemp;
	
	SpeedTemp = (CurrentDistance - OldDistance)/(CurrentTime - OldTime); // Distance in cm / time in 10 mS
	Speed = SpeedTemp * 100; // Convert Speed to Distance in cm / time in S (cm/s)*/
	//return Speed;
}
