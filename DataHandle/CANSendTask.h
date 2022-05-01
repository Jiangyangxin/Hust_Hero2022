#ifndef __DATASENDTASK_H__
#define __DATASENDTASK_H__

#include "main.h"

void CAN1_FrictSend(short a,short b,short c,short d);
void CAN2_GimbalSend(short pitch,short yaw);
void CAN2_BigPullerSend(short a);
typedef struct{
	short SuperPowerLimit;
	short Chassis_Flag;
	short Armortype;
	
//	char SuperPowerLimit;	//0???????,??0?????????
//	char Chassis_Flag;			//0??????  1?????  2??????  3????
	char DangershootFlag;					//但舱盖
	char Laser_Flag;				//激光
	short Gimbal_100;				//pitch角度
	char Gimbal_Flag;				//0为正常，1为辅瞄
	char Graphic_Init_Flag;	//0为初始化，1为初始化结束
	char Follow_state;			//底盘跟随状态
	char Frict_state;			//底盘跟随状态
	char Send_Pack1;	
}F405_typedef;
#endif

