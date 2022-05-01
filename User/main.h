#ifndef __MAIN_H__
#define __MAIN_H__

#define ABS(x) ((x)>0? (x):(-(x)))
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))  //????

//Standard Lib
#include <stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BSPconfig.h"
#include "arm_math.h"
/***********************�弶֧�ְ�**************************/
#include "tim.h"
#include "tim7.h"
#include "can1.h"
#include "can2.h"
#include "usart3.h"
#include "uart5.h"
#include  "iwdg.h"
#include  "MicroSw.h"
#include  "SteeringEngine.h"
#include "Trajector_Equ.h"
#include "Laser.h"
/************************���********************************************/
#include "Remote.h"
#include "Gyro.h"
#include "DeBug.h"
#include "LED.h"
/************************���ݴ���*************************/
#include "CANSendTask.h"
#include "CANReceiveTask.h"
#include "ZeroCheckTask.h"
/************************�ܿ���*********************************/
#include "ActionControlTask.h"
/************************��λ****************************************/
#include "GimbalTask.h"
#include "ShootTask.h"
#include "ChassisTask.h"
#include "Jugement.h"
#include "DataSendTask.h"
#include "DataReceivetask.h"
/************************����****************************************/
#include "Start_Task.h"
#include "LED_Task.h"
/*************************ASSIST************************/
#include "pid.h"
#include "DataScope_DP.h"
#include "algorithmOfCRC.h"
#include "KalmanFilter.h"

typedef struct
{
		unsigned short MagOpen;
		unsigned short MagClose;
	  unsigned short Pitch_init;
	  unsigned short Yaw_init;
		unsigned short Save_Yaw_init;	//���ݳ�ʼ��Yaw_init
		unsigned short Solo_Yaw_init;	//����ģʽ��Yaw_init
} RobotInit_Struct;

void Offline_Check_task(void *pvParameters);
void RobotInit(void);
void delay_ms(unsigned int t);
#endif
