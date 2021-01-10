
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
	T_MSG_OUT[1]: Robot Speed
	T_MSG_OUT[2]: Robot Position (part 1/2)
	T_MSG_OUT[3]: Robot Position (part 2/2)
	T_MSG_OUT[4]: Steering Sensor reading (8/10) most significant bits
	T_MSG_OUT[5]: Steering Sensor reading (2/10) least significant bits & Line follower sensor 1
	T_MSG_OUT[6]: Line follower sensor 2 & Steering Limit switches
	T_MSG_OUT[7]: 'E' End of the message
*/

/*
	T_MSG_IN[0]: 'C' Message header
	T_MSG_IN[1]: Siren, Lamp Control, Motor Directions,
	T_MSG_IN[2]: Motor PWM 1 (1/2)
	T_MSG_IN[3]: Motor PWM 1 (2/2)
	T_MSG_IN[4]: Motor PWM 2 (1/2)
	T_MSG_IN[5]: Motor PWM 2 (2/2)
	T_MSG_IN[6]: Motor PWM 3 (1/2)
	T_MSG_IN[7]: Motor PWM 3 (2/2)
	T_MSG_IN[8]: 'E' End of the message
*/

#ifndef RB_Protocol_H
#define RB_Protocol_H


extern void RxBuffer(char c);
extern void ClearT_MSG_IN(void);
//extern void CreateMsg(void);
extern void SendReadings(void);
extern void HandleInputMSG(void);
extern void PrintMSG(void);
extern void CreateMsg(void);
#endif
