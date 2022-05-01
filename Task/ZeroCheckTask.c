#include "main.h"
/*--------------------------�ڲ�����----------------------------*/
ZeroCheck_Typedef ZeroCheck_Pitch,ZeroCheck_Yaw,ZeroCheck_Pitch_speed,ZeroCheck_Yaw_speed;
ZeroCheck_Typedef ZeroCheck_SmlPullerPos,ZeroCheck_SmlPullerSpeed;
ZeroCheck_Typedef ZeroCheck_GyroYaw,ZeroCheck_GyroPitch;
ZeroCheck_Typedef	ZeroCheck_BigPushPos,ZeroCheck_BigPullerPos;

int Gimbal_Pitch,Gimbal_Yaw,Gimbal_Pitch_Speed,Gimbal_Yaw_Speed;
int SmlPuller_Pos,SmlPuller_Speed;
float Gyro_Yaw,Gyro_Pitch;
int BigPush_Pos,BigPuller_Pos;
/*--------------------------�ⲿ���ñ���----------------------------*/
extern M3508Receive_Typedef			BigFrict[2],SmlFrict[2],BigPuller;
extern GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
//extern Gyro_t 									Gyro;
extern M3508Receive_Typedef			BigPuller;
/*--------------------------��������----------------------------*/

/**
  * @brief  λ��ʽ���ٶ�ʽ������
	           Zero->ActualValue ��ʾ�������ǰֵ
						 Zero->LastValue ��ʾ�������һ��ֵ
						 Zero->CountCycle ��ʾ���������ʱԽ��ֵ������������
						 Zero->PreError ��ʾ�������ֵ
						 ʹ�ô˺���ǰҪ������Ӧ������ṹ��� Zero->CountCycle��Zero->LastValue
  * @param  ZeroCheck_Typedef *Zero  ������ṹ��
  *         float value  �������
            short Zerocheck_mode��ȡֵPosition��Speed
  * @retval ȡ����Zerocheck_mode���ֱ�����������λ��ֵ���ٶ�ֵ
  */
float ZeroCheck(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode)
{
    Zero->ActualValue=value;

    Zero->PreError=Zero->ActualValue-Zero->LastValue;
    Zero->LastValue=Zero->ActualValue;

    if(Zero->PreError>0.9f*Zero->CountCycle)
    {
        Zero->PreError=Zero->PreError-Zero->CountCycle;
        Zero->Circle++;
    }
    if(Zero->PreError<-0.9f*Zero->CountCycle)
    {
        Zero->PreError=Zero->PreError+Zero->CountCycle;
        Zero->Circle--;
    }

    if(Zerocheck_mode==Position)
        return Zero->ActualValue - Zero->Circle*Zero->CountCycle;    //���� 6020���λ��ֵ��8192Խ�䵽2����circle-1����ô�������õ���ֵΪ 2+8192*1
    else if(Zerocheck_mode==Speed)
        return Zero->PreError;
    else
        return 0;
}

float ZeroCheck_BigPush(ZeroCheck_Typedef *Zero,float value,short Zerocheck_mode)
{
    Zero->ActualValue=value;

    Zero->PreError=Zero->ActualValue-Zero->LastValue;
    Zero->LastValue=Zero->ActualValue;

    if(Zero->PreError>0.9f*Zero->CountCycle)
    {
        Zero->PreError=Zero->PreError-Zero->CountCycle;
        Zero->Circle++;
    }
    if(Zero->PreError<-0.9f*Zero->CountCycle)
    {
        Zero->PreError=Zero->PreError+Zero->CountCycle;
        Zero->Circle--;
    }

    if(Zerocheck_mode==Position)
        return Zero->ActualValue - Zero->Circle*Zero->CountCycle;
    else if(Zerocheck_mode==Speed)
        return Zero->PreError;
    else
        return 0;
}

/**
  * @brief  ������ִ�к���
  * @param  None
  * @retval None
  */
void ZeroCheck_cal(void)
{
    Gimbal_Pitch=ZeroCheck(&ZeroCheck_Pitch,Gimbal_Motor_Pitch.Angle,Position);
    Gimbal_Pitch_Speed=ZeroCheck(&ZeroCheck_Pitch_speed,Gimbal_Motor_Pitch.Angle,Speed);
    Gimbal_Yaw=ZeroCheck(&ZeroCheck_Yaw,Gimbal_Motor_Yaw.Angle,Position);
    Gimbal_Yaw_Speed=ZeroCheck(&ZeroCheck_Yaw_speed,Gimbal_Motor_Yaw.Angle,Speed);
    BigPuller_Pos = ZeroCheck(&ZeroCheck_BigPullerPos,BigPuller.Angle,Position);
}


/**
  * @brief  ������ṹ�������ʼ��
  * @param  None
  * @retval None
  */
void ZeroCheck_Init(void)
{
    ZeroCheck_Pitch.CountCycle=8192;
    ZeroCheck_Pitch.LastValue=Gimbal_Motor_Pitch.Angle;
    ZeroCheck_Pitch.Circle=0;

    ZeroCheck_Yaw.CountCycle=8192;
    ZeroCheck_Yaw.LastValue=Gimbal_Motor_Yaw.Angle;
    ZeroCheck_Yaw.Circle = 0;

    ZeroCheck_Pitch_speed.Circle=0;
    ZeroCheck_Pitch_speed.CountCycle=8192;
    ZeroCheck_Pitch_speed.LastValue=Gimbal_Motor_Pitch.Angle;

    ZeroCheck_Yaw_speed.Circle=0;
    ZeroCheck_Yaw_speed.CountCycle=8192;
    ZeroCheck_Yaw_speed.LastValue=Gimbal_Motor_Yaw.Angle;

    ZeroCheck_BigPullerPos.Circle = 0;
    ZeroCheck_BigPullerPos.CountCycle = 8192;
    ZeroCheck_BigPullerPos.LastValue = BigPuller.Angle;
}

/**
  * @brief  ��̨Yaw����ٶȹ�����ֵ ������̨������ֵ
  * @param  None
  * @retval ��̨Yaw����ٶȹ���������ֵ
  */
short ZeroCheck_Yaw_SpeedOutPut(void)
{
    return Gimbal_Yaw_Speed;
}
/**
  * @brief  ��̨Yaw��Ƕȹ�����ֵ
  * @param  None
  * @retval ��̨Yaw��Ƕȹ���������ֵ
  */
int ZeroCheck_YawOutPut(void)
{
    return Gimbal_Yaw;
}

/**
  * @brief  ��̨Pitch����ٶȹ�����ֵ,����̨����ֵ����
  * @param  None
  * @retval ��̨Pitch����ٶȹ���������ֵ
  */
short ZeroCheck_Pitch_SpeedOutPut(void)
{
    return Gimbal_Pitch_Speed;
}
/**
  * @brief  ��̨Pitch��Ƕȹ�����ֵ
  * @param  None
  * @retval ��̨Pitch��Ƕȹ���������ֵ
  */
short ZeroCheck_PitchOutPut(void)
{
    return Gimbal_Pitch;
}

/**
  * @brief  ���̽Ƕȹ�����ֵ
  * @param  None
  * @retval ���̽Ƕȹ���������ֵ
  */
int ZeroCheck_BigPullerPosOutPut(void)
{
    return BigPuller_Pos;// BigPuller_Pos = ZeroCheck(&ZeroCheck_BigPullerPos,BigPuller.Angle,Position);
}
