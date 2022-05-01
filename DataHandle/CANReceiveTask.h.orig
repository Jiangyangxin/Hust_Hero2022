#ifndef _CANRECTASK_H
#define _CANRECTASK_H

#include "main.h"

typedef struct{
	short Current;
	short Angle;
	short RealSpeed;       
}M3508Receive_Typedef;

typedef struct{
	short Current;
	short Angle;
	short RealSpeed;       
}GM6020Receive_Typedef;

typedef struct{
	short Current;
	short Angle;
	short RealSpeed;       
}M2006Receive_Typedef;

void CAN1_Receive0Task(CanRxMsg rx_message);
void CAN2_Receive0Task(CanRxMsg rx_message);
void Chassis_DataRecv(unsigned char Data[]);

#endif

