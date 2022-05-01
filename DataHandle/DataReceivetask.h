#ifndef __DATARECEIVETASK_H__
#define __DATARECEIVETASK_H__
#include "usart3.h"
#include "stm32f4xx.h"

typedef struct
{
    float PCPitch;
    float PCYaw;
    short ReceiveFromTx2BullectCnt;
    short FrictionWheel_speed;
    short DisConnect;
    float PCdistance;
} PC_Receive_t;



typedef struct {
    short Angle;
    short RealSpeed;
    short Current;
} BodanMotorReceive_Typedef;

void PCReceive(unsigned char PCReceivebuffer[]);

void F105_Rst(void);

#endif
