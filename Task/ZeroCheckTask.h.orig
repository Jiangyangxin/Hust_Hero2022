#ifndef __ZEROCHECKTASK_H__
#define __ZEROCHECKTASK_H__

#include "main.h"
#define Position 1
#define Speed    2

typedef struct 
{
	float Circle;           //转过圈数
	float CountCycle;       //转过一圈的总计数周期
	float LastValue;        //检测过零量上一次的值	
	float ActualValue;      //检测过零量当前值
	float PreError;         //检测量判断差值
}
ZeroCheck_Typedef;

float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode);
void ZeroCheck_cal(void);
void ZeroCheck_Init(void);

int ZeroCheck_YawOutPut(void);
short ZeroCheck_PitchOutPut(void);
int ZeroCheck_BodanPosOutPut(void);
int ZeroCheck_BodanSpeedOutPut(void);
float ZeroCheck_GyroYawOutPut(void);
float ZeroCheck_GyroPitchOutPut(void);
short ZeroCheck_Pitch_SpeedOutPut(void);
short ZeroCheck_Yaw_SpeedOutPut(void);
int ZeroCheck_BigPushPosOutPut(void);
int ZeroCheck_BigPullerPosOutPut(void);
float ZeroCheck_BigPush(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode);

#endif

