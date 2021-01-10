#include "TM4C1292NCPDT.h"                    // Device header
#include "Clock_Setting.h"                    // Clock_Setting header
#include "Serial.h"

#define byte unsigned char
#define DOUT 			(GPIOF_AHB->DATA & 0x04)		// Digital Read
#define CLK_Tog  	(GPIOF_AHB->DATA ^= 0x02)		// Digital Write (toggling)
#define CLK_HIGH	(GPIOF_AHB->DATA |= 0x02)		// Digital Write (Drive High)
#define CLK_LOW		(GPIOF_AHB->DATA &= ~(0x02))		// Digital Write (Drive Low)
#define gain 128
#define times 10
unsigned char GAIN;		// amplification factor

long OFFSET = 0;	// used for tare weight
float SCALE = 1;	// used to return weight in grams, kg, ounces, whatever

/*
	SysTick is a 24 Bit counter used to generate time delay
	Load Value = ( Fvco /  4 )  * Required Time 
*/

void Begin(void);
char is_ready(void);
void set_gain(void);
long Read(void);
void tare(void);
void set_gain(void); 
long read_average(void);
uint32_t Pos1=0,Pos2=0;
uint16_t data_Pos_3,data_Pos_2,data_Pos_1,data_Pos_0;

void PrintDEC(uint16_t data_Pos)
{
	data_Pos_3 = data_Pos - (data_Pos%1000);
	data_Pos = data_Pos - data_Pos_3;
	data_Pos_2 = data_Pos - (data_Pos%100);		
	data_Pos = data_Pos - data_Pos_2;
	data_Pos_1 = data_Pos - (data_Pos%10);		
	data_Pos_0 = data_Pos - data_Pos_1;
	data_Pos_3 = data_Pos_3 / 1000;
	data_Pos_2 = data_Pos_2 / 100;
	data_Pos_1 = data_Pos_1 / 10; 
	
	//SER_PutChar('$');
	SER_PutChar(data_Pos_3+'0');
	//SER_PutChar(',');
	SER_PutChar(data_Pos_2+'0');
	//SER_PutChar(',');
	SER_PutChar(data_Pos_1+'0');
	//SER_PutChar(',');
	SER_PutChar(data_Pos_0+'0');
	//SER_PutChar(';');
	SER_PutChar('\n');
	SER_PutChar('\r');	
}
void Begin() 
{
	set_gain();
}

void set_gain() 
{
  switch (gain) 
  {
    case 128:		// channel A, gain factor 128
    GAIN = 1;
    break;
    case 64:		// channel A, gain factor 64
    GAIN = 3;
    break;
    case 32:		// channel B, gain factor 32
    GAIN = 2;
    break;
  }
  CLK_LOW;
  Read();
}

long Read() 
{
	// wait for the chip to become ready
  while (DOUT) {}
  unsigned long value = 0;
  uint32_t data[3] = { 0 };
  uint32_t filler = 0x00;

  // pulse the clock pin 24 times to read the data
  // Shift the data and store 24 bit in one array
  for(byte i = 0; i < 3; i++)
  {
    for(byte n = 0; n < 8; ++n)
    {
      CLK_HIGH;
      data[2-i]  |= DOUT << (7 - n);
      CLK_LOW;	
    }
  }	
  // set the channel and the gain factor for the next reading using the clock pin
  for (unsigned int i = 0; i < GAIN; i++) 
  {
    CLK_HIGH;
    CLK_LOW;
  }

  // Replicate the most significant bit to pad out a 32-bit signed integer
  if (data[2] & 0x80) 
  {
    filler = 0xFF;
  } 
  else 
  {
    filler = 0x00;
  }
  // Construct a 32-bit signed integer
  value = ( (filler) << 24
	| (data[2]) << 16
	| (data[1]) << 8
	| (data[0]) );
  return (value);
}

void tare() 
{
  double sum = read_average();
  OFFSET=sum;
}

long read_average() 
{
  long sum = 0;
  for (byte i = 0; i < times; i++) 
  {
    sum += Read();
    //yield();
  }
  return sum / times;
}

float calibration_factor = 110000; //-7050 worked for my 440lb max scale setup




int main(void)
{
	
	//SetSysClock();
	SYSCTL->RCGCGPIO |= 0x0020;	// Enable clock to PORTF
	GPIOF_AHB->DIR |= 0x02;			// Pin F1 digital
	//GPIOF_AHB->DIR |= 0x04;			// Pin  F2 digital
	GPIOF_AHB->DEN |= 0x06;			// Pins F1 & F2 digital
	GPIOF_AHB->DATA ^= 0x02;
	//SysTick->LOAD = 0xB71B00; 	// Count 0.1 S
	SysTick->LOAD = 0x186A00; 	// 0.1 S
	SysTick->CTRL = 0x05;				// Use System Clock & Enable SysTick
	SER_Initialize();
	Begin();
  tare();	//Reset the scale to 0
	
	while(1)
	{
		while(!(SysTick->CTRL & 0x10000)); // Wait until the flag goes high
		SCALE=calibration_factor; //Adjust to this calibration factor
		long Reading = (read_average() - OFFSET)/ SCALE;
		PrintDEC(Reading);
	}
	return 0;
}
