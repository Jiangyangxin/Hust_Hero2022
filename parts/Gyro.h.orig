#ifndef _GYRO_H
#define _GYRO_H
/**
  ******************************************************************************
  * @file    
  * @author DengJian
  * @version
  * @date    
  * @brief
  *
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
#include "main.h"
#include "ZeroCheck.h"
#define GyroReceiveSize 18

#define GYRO_BUF_SIZE 18

#define GyroCntCycle 360
/********************************** 任务句柄 *************************************/

/********************************** 内核对象句柄 *********************************/

/******************************* 全局变量声明 ************************************/
typedef struct GYRO{
		float GY;
		float GZ;
		float Pitch;
		float Yaw;
	
	  volatile char recvd[2];
    volatile char DisconnectCnt;
}Gyro_t;




typedef union
{
	float fdata;
	unsigned long idata;
}FloatlongType;

typedef union
{
	float fdata;
	unsigned char sdata[4];
}FloatUint8Type;
/******************************* 函数声明 ****************************************/
void GyroRecv_DataHandle(unsigned char gyroBuffer[]);
void NewGyro_DataHandle(CanRxMsg rx_message);
/**********************************************************************************/

#endif
