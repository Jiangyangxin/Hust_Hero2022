#ifndef _CHASSISTASK_H
#define _CHASSISTASK_H

#define RC_Multiple		0.1f
#define KEY_Multiple	660

#include "main.h"

typedef struct
{
	float carSpeedx;
	float carSpeedy;
	float carSpeedw;
	unsigned char SuperCapCmd;
	unsigned char PowerdownCmd;
	float CapReaimPower;
	short CapStartCnt;
}Chassis_typedef;

void Chassis_Act_Cal(void);
void Chassis_SelfProtect_Cal(void);
void Chassis_Cease_Cal(void);
void Chassis_Single_Cal(void);

void Pid_ChassisAngle(void);
void CAN1_ChassisSend(Chassis_typedef* Chassis,short ChassisRemainPower);
void Chassis_Cal(void);
void CHASSIS_task(void *pvParameters);
#endif
