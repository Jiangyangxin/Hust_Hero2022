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
	char DangershootFlag;					//���ո�
	char Laser_Flag;				//����
	short Gimbal_100;				//pitch�Ƕ�
	char Gimbal_Flag;				//0Ϊ������1Ϊ����
	char Graphic_Init_Flag;	//0Ϊ��ʼ����1Ϊ��ʼ������
	char Follow_state;			//���̸���״̬
	char Frict_state;			//���̸���״̬
	char Send_Pack1;	
}F405_typedef;
#endif

