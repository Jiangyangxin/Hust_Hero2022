#ifndef _GIMBAL_TASK_H
#define _GIMBAL_TASK_H

#include "main.h"

#define GIM_YAW_INIT_SCALE   	6540//2435
#define GIM_PIT_MAX_SCALE    	2114
#define GIM_PIT_INIT_SCALE   	4770
#define GIM_PIT_MIN_SCALE     709
#define GIM_YAW_MAX_ANGLE    180	//30 
#define GIM_YAW_MIN_ANGLE    -180	//-30

#define GIM_YAW_TURN_ANGLE		80

typedef struct
{
    float Pitch;
	  float Yaw;
	  short PitchMotorPosInit;
    short YawMotorPosInit;
  	float correctPit;
    float correctYaw;
	  float TureYaw;
} AbsoluteAngle_typedef;

void Gimbal_Act_Cal(void);
void Gimbal_Lob_Cal(void);
void Gimbal_Armor_Cal(void);
void Gimbal_Cease_Cal(void);

void Pid_YawGyroPosSpeed(void);
void Pid_PitchGyroPosSpeed(void);
void Pid_PitchYaw_ArmorPos(void);
void Pid_LobInit(void);

void GimbalCur_Pid_Cal(void);

void GIMBAL_task(void *pvParameters);
void GIMBAL_Rst(void);


float AbsolutePitch(void);
float AbsoluteYaw(void);
void	AbsoluteAngleInit(void);


void Gimbal_System_identification(float Gimbal_x,float Gimbal_y);
void get_F(void);
void T_change(void);


#endif
