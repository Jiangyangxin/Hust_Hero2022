#ifndef _SHOOT_TASK_H
#define _SHOOT_TASK_H


typedef struct
{
    short PullerCmd;
    short PushrodCmd;//????????
    short PushrodTrig;
    short BulletCnt;
    short BulletNum;
    short TimeCnt;
    short Priod;
    short StuckTimeCnt;
    short RemainNum;
    short Shoot_Finish;
    int   Current_Angle;
} Shoot_typedef;
void Shoot_Fire_Cal(void);

void Shoot_Act_Cal(void);
void Shoot_Cease_Cal(void);
void ShootCur_Pid_Cal(void);

void Pid_BigPullerSpeed(void);
void Pid_BigFrictSpeed(void);
void Pid_BigPullerPos(void);

void SHOOT_task(void *pvParameters);
#endif
