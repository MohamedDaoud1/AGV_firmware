#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
	T_MSG_IN[0]: 'R' Message header
	T_MSG_IN[1]: Robot Speed
	T_MSG_IN[2]: Robot Position (part 1/2)
	T_MSG_IN[3]: Robot Position (part 2/2)
	T_MSG_IN[4]: Steering Sensor reading (8/10) most significant bits
	T_MSG_IN[5]: Steering Sensor reading (2/10) least significant bits & Line follower sensor 1
	T_MSG_IN[6]: Line follower sensor 2 & Steering Limit switches
	T_MSG_IN[7]: 'E' End of the message
*/

/*
	T_MSG_OUT[0]: 'C' Message header
	T_MSG_OUT[1]: Motor Directions, Siren, Lamp Control
	T_MSG_OUT[2]: Motor PWM 1 (1/2)
	T_MSG_OUT[3]: Motor PWM 1 (2/2)
	T_MSG_OUT[4]: Motor PWM 2 (1/2)
	T_MSG_OUT[5]: Motor PWM 2 (2/2)
	T_MSG_OUT[6]: Motor PWM 3 (1/2)
	T_MSG_OUT[7]: Motor PWM 3 (2/2)
	T_MSG_OUT[8]: 'E' End of the message
*/
char MsgInInx = 0; 
/*signed*/ char T_MSG_IN[8];
unsigned char T_MSG_OUT[9];

char RobotSpeed;
int RobotPosition;
int SteeringSensor;
char LineFollower1, LineFollower2, LS;

void ClearInMsg(void)
{
	MsgInInx = 0;
	for(char i = 0; i < 8; i++)
		T_MSG_IN[i] = 0;
}
void HandleInputMSG(void)
{
	if((T_MSG_IN[0] == 'R') && (T_MSG_IN[7] == 'E'))
	{
		RobotSpeed = 0.4 * T_MSG_IN[1] + 0.6 * RobotSpeed;
		RobotPosition = ((T_MSG_IN[2] << 8) | T_MSG_IN[3]);
		SteeringSensor = ((T_MSG_IN[4] << 2) | (T_MSG_IN[5] >> 6));
		LineFollower1 = (T_MSG_IN[5] >> 1) & 0x1F;
		LineFollower2 = (T_MSG_IN[6] >> 3) & 0x1F;
		LS = (T_MSG_IN[6]) & 0x07;
	}
	printf("Speed: %d\n\r",RobotSpeed);
	printf("Position: %d\n\r",RobotPosition);
	printf("Steering: %d\n\r",SteeringSensor);
	printf("Line Follower 1: %d\n\r",LineFollower1);
	printf("Line Follower 2: %d\n\r",LineFollower2);
	printf("Limit Switches: %d\n\r",LS);
	printf("======================\n\r");
	ClearInMsg();
}

unsigned char Siren = 0;
unsigned char Lamp = 1;
unsigned char MotorsDirections = 0x15;
unsigned int PWM1 = 0x320;
unsigned int PWM2 = 0x384;
unsigned int PWM3 = 0x058;
void CreateMsg(void)
{
	PWM1 = rand() % 1000;
	PWM2 = rand() % 1000;
	PWM3 = rand() % 1000;
	Siren ^= 0x01;
	Lamp ^= 0x01;
	MotorsDirections ^= 0x3F;
	//printf("PWM1 = %d\n\r", PWM1);
	//printf("Siren = %d\n\r", Siren);
	T_MSG_OUT[0] = 'C';
	T_MSG_OUT[1] = ((Siren << 7) | (Lamp << 6) | MotorsDirections);
	T_MSG_OUT[2] = (PWM1 >> 8);
	T_MSG_OUT[3] = (PWM1 & 0x00FF);
	T_MSG_OUT[4] = (PWM2 >> 8);
	T_MSG_OUT[5] = (PWM2 & 0x00FF);
	T_MSG_OUT[6] = (PWM3 >> 8);
	T_MSG_OUT[7] = (PWM3 & 0x00FF);
	T_MSG_OUT[8] = 'E';
}

//-------------------------
//----- SETUP UART 0 -----
//-------------------------
//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
int uart0_filestream = -1;

//OPEN THE UART
//The flags (defined in fcntl.h):
//	Access modes (use 1 of these):
//		O_RDONLY - Open for reading only.
//		O_RDWR - Open for reading and writing.
//		O_WRONLY - Open for writing only.
//
//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
//											immediately with a failure status if the output can't be written immediately.
//
//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the 
//	terminal device to become the controlling terminal for the process.

int main()
{
	srand(time(NULL));
	uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	//CONFIGURE THE UART
	//	The flags (defined in /usr/include/termios.h - 
	//	see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, 
	//	B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, 
	//	B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end 
	//	of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B1000000 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = 0;//IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;//ECHO;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
	
	
	unsigned char tx_buffer[20];
	unsigned char *p_tx_buffer;
	p_tx_buffer = &T_MSG_OUT[0];
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	*p_tx_buffer++;
	
	
	unsigned char REC = 0;
	
	//*p_tx_buffer++;

	
	//----- CHECK FOR ANY RX BYTES -----
	while(1)
	{
		if ( (uart0_filestream != -1) && REC)
		{
			
			REC = 0;
			CreateMsg();
			int count = write(uart0_filestream, &T_MSG_OUT[0], (p_tx_buffer - &T_MSG_OUT[0]));		//Filestream, bytes to write, number of bytes to write
			if (count < 0)
			{
				printf("UART TX error\n");
			}
		}
		
		if (uart0_filestream != -1)
		{
			// Read up to 255 characters from the port if they are there
			unsigned char rx_buffer[256];
			int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)
			if (rx_length < 0)
			{
				//An error occured (will occur if there are no bytes)
			}
			else if (rx_length == 0)
			{
				//No data waiting
			}
			else
			{
				//Bytes received
				rx_buffer[rx_length] = '\0';
				//printf("%i bytes read : \n", rx_length);
				//printf("MsgInInx: %d\n\r",MsgInInx);
				char comp = (char)rx_length + MsgInInx;
				//printf("rx_length + MsgInInx = %d \n\r", (comp));
				/*for(char i = 0; i < rx_length; i++)
					printf("%X \n\r", rx_buffer[i]);
				*/
				for(char i = MsgInInx; i < comp; i++)
					T_MSG_IN[i] = rx_buffer[i-MsgInInx];
				//printf("---\n\r");
				MsgInInx = MsgInInx + rx_length;
				/*for(char i = 0; i < MsgInInx; i++)
					printf("%X \n\r", T_MSG_IN[i]);
				*/
				if(MsgInInx == 8)
				{					
					HandleInputMSG();
					REC = 1;
				}
				
				//printf("\n\r");
				//printf("%i bytes read : %s\n", rx_length, rx_buffer);
				//printf("%i bytes read : %c - %X\n", rx_length, rx_buffer[0],rx_buffer[0]);
			}
			//printf("%d\n\r",rx_length);
		}
	}
	//----- CLOSE THE UART -----
	close(uart0_filestream);
	printf("DONE\n\r");
}

