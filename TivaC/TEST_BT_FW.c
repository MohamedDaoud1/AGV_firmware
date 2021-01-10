#include "TM4C1292NCPDT.h"                    // Device header
#include "Clock_Setting.h"                    // Clock_Setting header
#include "PORTS/PORT_A.h"
#include "PORTS/PORT_B.h"
#include "PORTS/PORT_C.h"
#include "PORTS/PORT_D.h"
#include "PORTS/PORT_E.h"
#include "PORTS/PORT_F.h"
#include "PORTS/PORT_H.h"
#include "PORTs/PORT_K.h"
#include "PORTS/PORT_N.h"
#include "PORTS/PORT_P.h"
#include "BT_Control.h"
#include "Print_Num.h"
#include "RB_Protocol.h"
#include "GlobalVariables.h"
#include "TimerA0.h"                    // Device header
#include "TimerA1.h"                    // Device header
#include "HandleIncEn.h"


int main(void)
{
	SetSysClock();
	InitPortA();
	InitPortB();
	InitPortC();
	InitPortD();
	InitPortE();
	InitPortF();
	InitPortK();
	InitPortN();
	InitPortP();
	InitPortH();
	InitializeTimer0A();
	InitializeTimer1A();
	IncEnFactor = InitDistanceFactor();
	Stop();
	while(1)
	{
		if(hundM == 500)
		{
			hundM = 0;			
			CreateMsg();
			SendReadings();
		}
		if(REC)
		{
			REC = 0;
			HandleInputMSG();
		}
	}
}
