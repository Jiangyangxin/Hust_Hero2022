#ifndef _CHASSISTASK_H
#define _CHASSISTASK_H

#define RC_Multiple		5.0f
#define KEY_Multiple	660
#define GimbalAble 0
#define GimbalDisable 1
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

    short IsShootAble;
    short bulletSpeedmax;
	  float bulletSpeed;
    float Limit_Power_k;
    short RobotRed;
    short HP;
    short Last_HP;
} Chassis_typedef;

void Chassis_Act_Cal(void);
void Chassis_SelfProtect_Cal(void);
void Chassis_Cease_Cal(void);
void Chassis_Single_Cal(void);
void Chassis_Solo_Cal(void);
void Pid_ChassisAngle(void);
void CAN1_ChassisSend(Chassis_typedef* Chassis);
void Chassis_Cal(void);
void CHASSIS_task(void *pvParameters);
#endif
