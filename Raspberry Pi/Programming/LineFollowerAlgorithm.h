/*
Representation	 Decimal	Extreme_Right	Slight_Right	Center	 Slight_Left	Extreme Left	
		-4			 30			0	0			0	0		  0	1		1	1		1	0		RIGHT
		-3			 28			0	0			0	0		  0	1		1	1		0	0		RIGHT
		-2			 24			0	0			0	0		  0	1		1	0		0	0		RIGHT
		-1			 56			0	0			0	0		  1	1		1	0		0	0		RIGHT
		 0			 48			0	0			0	0		  1	1		0	0		0	0		Neutral
		 1			112			0	0			0	1		  1	1		0	0		0	0		LEFT
		 2			 96			0	0			0	1		  1	0		0	0		0	0		LEFT
		 3			224			0	0			1	1		  1	0		0	0		0	0		LEFT
		 4			480			0	1			1	1		  1	0		0	0		0	0		LEFT
*/

void main()
{
	switch (LineFollowerValue)
	{
		case 30:
			Feedback = -4;
			break;
		case 28:
			Feedback = -3;
			break;
		case 24:
			Feedback = -2;
			break;
		case 56:
			Feedback = -1;
			break;
		case 48:
			Feedback = 0;
			break;
		case 112:
			Feedback = 1;
			break;	
		case 96:
			Feedback = 2;
			break;
		case 224:
			Feedback = 3;
			break;
		case 480:
			Feedback = 4;
			break;
		default:
			Feedback = 100;
			break;
	}
}