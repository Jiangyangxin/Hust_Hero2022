#include "main.h"
/*--------------------------内部变量----------------------------*/
ZeroCheck_Typedef ZeroCheck_Pitch,ZeroCheck_Yaw,ZeroCheck_Pitch_speed,ZeroCheck_Yaw_speed;
ZeroCheck_Typedef ZeroCheck_SmlPullerPos,ZeroCheck_SmlPullerSpeed;
ZeroCheck_Typedef ZeroCheck_GyroYaw,ZeroCheck_GyroPitch;
ZeroCheck_Typedef	ZeroCheck_BigPushPos,ZeroCheck_BigPullerPos;

int Gimbal_Pitch,Gimbal_Yaw,Gimbal_Pitch_Speed,Gimbal_Yaw_Speed;
int SmlPuller_Pos,SmlPuller_Speed;
float Gyro_Yaw,Gyro_Pitch;
int BigPush_Pos,BigPuller_Pos;
/*--------------------------外部引用变量----------------------------*/
extern M3508Receive_Typedef			BigFrict[2],SmlFrict[2],BigPuller;
extern GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
//extern Gyro_t 									Gyro;
extern M3508Receive_Typedef			BigPuller;
/*--------------------------函数部分----------------------------*/

/**
  * @brief  位置式和速度式过零检测
	           Zero->ActualValue 表示检测量当前值
						 Zero->LastValue 表示检测量上一次值
						 Zero->CountCycle 表示检测量过零时越变值，即计数周期
						 Zero->PreError 表示检测量差值
						 使用此函数前要申明对应检测量结构体的 Zero->CountCycle与Zero->LastValue
  * @param  ZeroCheck_Typedef *Zero  过零检测结构体
  *         float value  待检测量
            short Zerocheck_mode：取值Position或Speed
  * @retval 取决于Zerocheck_mode，分别输出过零检测后位置值或速度值
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
        return Zero->ActualValue - Zero->Circle*Zero->CountCycle;    //例如 6020电机位置值从8192越变到2，则circle-1，那么过零检测后得到的值为 2+8192*1
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
  * @brief  过零检测执行函数
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
  * @brief  过零检测结构体参数初始化
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
  * @brief  云台Yaw轴角速度过零检测值 ，以云台做反馈值
  * @param  None
  * @retval 云台Yaw轴角速度过零检测后输出值
  */
short ZeroCheck_Yaw_SpeedOutPut(void)
{
    return Gimbal_Yaw_Speed;
}
/**
  * @brief  云台Yaw轴角度过零检测值
  * @param  None
  * @retval 云台Yaw轴角度过零检测后输出值
  */
int ZeroCheck_YawOutPut(void)
{
    return Gimbal_Yaw;
}

/**
  * @brief  云台Pitch轴角速度过零检测值,以云台反馈值计算
  * @param  None
  * @retval 云台Pitch轴角速度过零检测后输出值
  */
short ZeroCheck_Pitch_SpeedOutPut(void)
{
    return Gimbal_Pitch_Speed;
}
/**
  * @brief  云台Pitch轴角度过零检测值
  * @param  None
  * @retval 云台Pitch轴角度过零检测后输出值
  */
short ZeroCheck_PitchOutPut(void)
{
    return Gimbal_Pitch;
}

/**
  * @brief  大拨盘角度过零检测值
  * @param  None
  * @retval 大拨盘角度过零检测后输出值
  */
int ZeroCheck_BigPullerPosOutPut(void)
{
    return BigPuller_Pos;// BigPuller_Pos = ZeroCheck(&ZeroCheck_BigPullerPos,BigPuller.Angle,Position);
}
