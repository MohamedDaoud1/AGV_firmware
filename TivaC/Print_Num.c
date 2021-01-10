/*----------------------------------------------------------------------------
 * Name:    Print_Num.c
 * Purpose: Converting numerical data into charachters to print
 *----------------------------------------------------------------------------*/

#include "TM4C1292NCPDT.h"                    // Device header
#include "Print_Num.h"
#include "PORTs/PORT_A.h"
void PrintHEX(char CHAR)
{
	if((CHAR>>4)>9)
		SER0_PutChar((CHAR>>4)+55);
	else
		SER0_PutChar((CHAR>>4)+'0');
	if((CHAR&0x0F)>9)
		SER0_PutChar((CHAR&0x0F)+55);
	else
		SER0_PutChar((CHAR&0x0F)+'0');
}	

void PrintDEC_Monitor(int16_t data_Pos)
{
	if((data_Pos) < 0)
	{
		data_Pos = data_Pos * -1;
		SER0_PutChar('-');
	}
	uint16_t data_Pos_4,data_Pos_3,data_Pos_2,data_Pos_1,data_Pos_0;
	data_Pos_4 = data_Pos - (data_Pos%10000);
	data_Pos = data_Pos - data_Pos_4;
	data_Pos_3 = data_Pos - (data_Pos%1000);
	data_Pos = data_Pos - data_Pos_3;
	data_Pos_2 = data_Pos - (data_Pos%100);		
	data_Pos = data_Pos - data_Pos_2;
	data_Pos_1 = data_Pos - (data_Pos%10);		
	data_Pos_0 = data_Pos - data_Pos_1;
	data_Pos_3 = data_Pos_3 / 1000;
	data_Pos_2 = data_Pos_2 / 100;
	data_Pos_1 = data_Pos_1 / 10; 
	
	SER0_PutChar(data_Pos_4+'0');
	SER0_PutChar(data_Pos_3+'0');
	SER0_PutChar(data_Pos_2+'0');
	SER0_PutChar(data_Pos_1+'0');
	SER0_PutChar(data_Pos_0+'0');
	SER0_PutChar('\n');
	SER0_PutChar('\r');	
}

void PrintDEC_Plotter(uint16_t data_Pos)
{
	uint16_t data_Pos_3,data_Pos_2,data_Pos_1,data_Pos_0;
	data_Pos_3 = data_Pos - (data_Pos%1000);
	data_Pos = data_Pos - data_Pos_3;
	data_Pos_2 = data_Pos - (data_Pos%100);		
	data_Pos = data_Pos - data_Pos_2;
	data_Pos_1 = data_Pos - (data_Pos%10);		
	data_Pos_0 = data_Pos - data_Pos_1;
	data_Pos_3 = data_Pos_3 / 1000;
	data_Pos_2 = data_Pos_2 / 100;
	data_Pos_1 = data_Pos_1 / 10; 
	
	SER0_PutChar('$');
	SER0_PutChar(data_Pos_3+'0');
	SER0_PutChar(',');
	SER0_PutChar(data_Pos_2+'0');
	SER0_PutChar(',');
	SER0_PutChar(data_Pos_1+'0');
	SER0_PutChar(',');
	SER0_PutChar(data_Pos_0+'0');
	SER0_PutChar(';');
}
