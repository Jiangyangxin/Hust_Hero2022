/*--------------------------头文件----------------------------*/
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/*--------------------------全局变量----------------------------*/
Pid_Typedef	PidChassisAngle,PidChassisSpeed,PidTwistAngle,PidTwistSpeed;//后面两个是cc加的
short ChassisInitFlag = 0;short TwistInitFlag = 0;
Chassis_typedef F105;
float slowFac = 5;
short SelfProtectSpeed=6200;
short F105WrigCnt,F105WrigT = 1000;
unsigned char superCapAutoFlag = 1;
short F105_DisConect;//功率板掉线检测
/*--------------------------引用外部变量----------------------------*/
extern HeroStatus_typedef	HeroStatus;
extern RC_Ctl_t RC_Ctl;
extern Jugement_t Juge;
extern GM6020Receive_Typedef Gimbal_Motor_Yaw;
extern float chassis_yaw,chassis_gz;
extern RobotInit_Struct Hero;
extern AbsoluteAngle_typedef AbsoluteAngle;
/*--------------------------内部变量----------------------------*/
short LastChassistype;
/**
  * @brief  底盘正常动作
  * @param
  * @retval
  */
int	YawOutPut;

void Chassis_Act_Cal(void)
{
	
	if(LastChassistype==Chassis_SelfProtect_Mode)
	{
	  if((Gimbal_Motor_Yaw.Angle<8192&&Gimbal_Motor_Yaw.Angle>4483)||(Gimbal_Motor_Yaw.Angle<386))
		{
			Hero.Yaw_init=GIM_YAW_INIT_SCALE;
		}
		else
		{
			Hero.Yaw_init=2435;
		}
	}
	else if(LastChassistype!=Chassis_Act_Mode)
		Hero.Yaw_init=GIM_YAW_INIT_SCALE;
		
	
	
   short vx,vy;
    YawOutPut = ZeroCheck_YawOutPut();
//
//		Hero.Yaw_init=GIM_YAW_INIT_SCALE;    
			
	 if(ChassisInitFlag != 1)
    {
        ChassisInitFlag = 1;
        F105.PowerdownCmd = 0;
        PidChassisAngle.SetPoint = 0;//  底盘跟随
    }
    if(HeroStatus.CtrMode == RemoteMode) //遥控器
    {
			if(RC_Ctl.rc.ch0 - 1024> 100)
				F105.carSpeedx=2000;
			else if (RC_Ctl.rc.ch0 - 1024< -100)
				F105.carSpeedx=-2000;
			else 
				F105.carSpeedx=0;
			
			
			if(RC_Ctl.rc.ch1 - 1024> 100)
				F105.carSpeedy=2000;
			else if (RC_Ctl.rc.ch1 - 1024< -100)
				F105.carSpeedy=-2000;
			else 
				F105.carSpeedy=0;
			
			
//        F105.carSpeedx = (RC_Ctl.rc.ch0 - 1024) * RC_Multiple;
//        F105.carSpeedy = (RC_Ctl.rc.ch1 - 1024) * RC_Multiple;//使用了角度值而不是编码器，所以要修改感度
    }
		
    if(HeroStatus.CtrMode == MouseKeyMode)   //键鼠
    {
        vx=2000*(RC_Ctl.key.d - RC_Ctl.key.a);
	      vy=2000*(RC_Ctl.key.w -RC_Ctl.key.s);
	      F105.carSpeedx = vx;//*arm_cos_f32(AbsoluteYaw()*0.0174533f)-vy*arm_sin_f32(AbsoluteYaw()*0.0174533f);//测试用遥控器
        F105.carSpeedy = vy;//*arm_cos_f32(AbsoluteYaw()*0.0174533f)+vx*arm_sin_f32(AbsoluteYaw()*0.0174533f);
    }

   PidChassisAngle.ActPoint = AbsoluteYaw();
		
		F105.carSpeedw = 500*PID_Calc(&PidChassisAngle);

	 
		F105.carSpeedw = LIMIT_MAX_MIN(F105.carSpeedw,2000,-2000);
		LastChassistype=Chassis_Act_Mode;
		
		if(Hero.Yaw_init!=GIM_YAW_INIT_SCALE)
		{
		F105.carSpeedx=-F105.carSpeedx;
    F105.carSpeedy=-F105.carSpeedy;		
		
		}
		
		
}
/**
  * @brief  底盘小陀螺
  * @param
  * @retval
  */
void Chassis_SelfProtect_Cal()
{
  if(ChassisInitFlag != 2)
    {
        ChassisInitFlag = 2;
        F105.PowerdownCmd = 0;
    } 
	short vx,vy;
	Hero.Yaw_init=GIM_YAW_INIT_SCALE; 
		
	 if(HeroStatus.CtrMode == MouseKeyMode){
        vx=2000*(RC_Ctl.key.d - RC_Ctl.key.a);
	      vy=2000*(RC_Ctl.key.w -RC_Ctl.key.s);
	 }
	 else if(HeroStatus.CtrMode ==  RemoteMode){
        vx = (RC_Ctl.rc.ch0 - 1024) * RC_Multiple;
        vy = (RC_Ctl.rc.ch1 - 1024) * RC_Multiple;//使用了角度值而不是编码器，所以要修改感度
	 }
//	 F105.carSpeedx = (vx*arm_cos_f32(AbsoluteAngle.TureYaw*0.0174533f)-vy*arm_sin_f32(AbsoluteYaw()*0.0174533f));
//   F105.carSpeedy = (vy*arm_cos_f32(AbsoluteAngle.TureYaw*0.0174533f)+vx*arm_sin_f32(AbsoluteYaw()*0.0174533f));//  0.0174533= 1/360 *2*pi
	 
	 F105.carSpeedx = (vx*arm_cos_f32(AbsoluteAngle.Yaw*0.0174533f)-vy*arm_sin_f32(AbsoluteAngle.Yaw*0.0174533f));
   F105.carSpeedy = (vy*arm_cos_f32(AbsoluteAngle.Yaw*0.0174533f)+vx*arm_sin_f32(AbsoluteAngle.Yaw*0.0174533f));	 
	 
//			  F105.carSpeedx = (RC_Ctl.key.a - RC_Ctl.key.d) * KEY_Multiple * (-1.5f) * slowFac;
//        F105.carSpeedy = (RC_Ctl.key.w -	RC_Ctl.key.s) * KEY_Multiple * 1.2f * slowFac;
   F105.carSpeedw	= SelfProtectSpeed;
	 
	 LastChassistype=Chassis_SelfProtect_Mode;
}
/**
  * @brief  底盘狙击模式
  * @param
  * @retval
  */
void Chassis_Sniper_Cal(void)
{
	Hero.Yaw_init=GIM_YAW_INIT_SCALE; 
    if(ChassisInitFlag != 3)
    {
        ChassisInitFlag = 3;
        F105.PowerdownCmd = 0;
        PidChassisAngle.SetPoint = ZeroCheck_YawOutPut();
    }

    if(HeroStatus.CtrMode == MouseKeyMode)
    {

        F105.carSpeedx=2000*(RC_Ctl.key.d - RC_Ctl.key.a);
	      F105.carSpeedy=2000*(RC_Ctl.key.w -RC_Ctl.key.s);
			//取消了底盘跟随，完全由键盘驱动
    }
		
		 if(HeroStatus.CtrMode == RemoteMode) //遥控器
    {
			if(RC_Ctl.rc.ch0 - 1024> 100)
				F105.carSpeedx=2000;
			else if (RC_Ctl.rc.ch0 - 1024< -100)
				F105.carSpeedx=-2000;
			else 
				F105.carSpeedx=0;
			
			
			if(RC_Ctl.rc.ch1 - 1024> 100)
				F105.carSpeedy=2000;
			else if (RC_Ctl.rc.ch1 - 1024< -100)
				F105.carSpeedy=-2000;
			else 
				F105.carSpeedy=0;
		
				if(HeroStatus.GimbalMode==Gimbal_Lob_Mode)	
				{
					F105.carSpeedw=0;
				}
    }
		
		F105.carSpeedw =(RC_Ctl.key.q - RC_Ctl.key.e) * 2500;
		F105.carSpeedw = LIMIT_MAX_MIN(F105.carSpeedw,2000,-2000);
		
		LastChassistype=Chassis_Sniper_Mode;
}
/**
  * @brief  底盘对角模式
  * @param
  * @retval
  */
void Chassis_Solo_Cal(void)
{
	if(ChassisInitFlag != 4)
    {
        ChassisInitFlag = 4;
        PidChassisAngle.SetPoint = ZeroCheck_YawOutPut();
    }
		
		Hero.Yaw_init=Hero.Solo_Yaw_init;
	  short vx,vy;

			if(HeroStatus.CtrMode == MouseKeyMode)
			{
			vx=2000*(RC_Ctl.key.d - RC_Ctl.key.a);
			vy=2000*(RC_Ctl.key.w - RC_Ctl.key.s);
			}
			
			else if(HeroStatus.CtrMode ==  RemoteMode)
			{
			vx=(RC_Ctl.rc.ch0 - 1024) * RC_Multiple;//(RC_Ctl.rc.ch0 - 1024) * RC_Multiple*0.1；
			vy=(RC_Ctl.rc.ch1 - 1024) * RC_Multiple;//
			}
  
//   F105.carSpeedx = (vx*arm_cos_f32(AbsoluteAngle.TureYaw*0.0174533f)-vy*arm_sin_f32(AbsoluteAngle.TureYaw*0.0174533f));
//   F105.carSpeedy = (vy*arm_cos_f32(AbsoluteAngle.TureYaw*0.0174533f)+vx*arm_sin_f32(AbsoluteAngle.TureYaw*0.0174533f));	
  
	 F105.carSpeedx = (vx*arm_cos_f32(AbsoluteAngle.TureYaw*0.0174533f)-vy*arm_sin_f32(AbsoluteAngle.TureYaw*0.0174533f));
   F105.carSpeedy = (vy*arm_cos_f32(AbsoluteAngle.TureYaw*0.0174533f)+vx*arm_sin_f32(AbsoluteAngle.TureYaw*0.0174533f));			

    PidChassisAngle.SetPoint = 0;//  底盘跟随
	  PidChassisAngle.ActPoint = AbsoluteYaw();		

//		PidChassisSpeed.SetPoint = -PID_Calc(&PidChassisAngle);
//		PidChassisSpeed.ActPoint = -0.002f* F105.carSpeedw;
	  
	  F105.carSpeedw = 500*PID_Calc(&PidChassisAngle);
    F105.carSpeedw = LIMIT_MAX_MIN(F105.carSpeedw,2000,-2000);
			
		LastChassistype=Chassis_Solo_Mode;	
}	



/**
  * @brief  底盘掉电模式
  * @param
  * @retval
  */
void Chassis_Cease_Cal(void)
{
		Hero.Yaw_init=GIM_YAW_INIT_SCALE; 
    if(ChassisInitFlag != 5)
        ChassisInitFlag = 5;

    F105.carSpeedx = 0;
    F105.carSpeedy = 0;
    F105.carSpeedw = 0;
    F105.PowerdownCmd = 0;
    F105.SuperCapCmd = 0;

		LastChassistype=Chassis_CeaseMode;
}

/**
  * @brief  底盘跟随pid初始化
  * @param
  * @retval
  */

void Pid_ChassisAngle(void)
{
    PidChassisAngle.SetPoint = 0;
    PidChassisAngle.P = 0.2;
    PidChassisAngle.I = 0.00001;
    PidChassisAngle.D = 0.15;
    PidChassisAngle.IMax = 1000.0f;

//    PidChassisSpeed.SetPoint = 0;
//    PidChassisSpeed.P = 1.2;
//    PidChassisSpeed.I = 0;
//    PidChassisSpeed.D = 0;
//    PidChassisSpeed.IMax = 100.0f;


}
/**
  * @brief  底盘数据发送
  * @param
  * @retval
  */

void Chassis_Cal(void)
{

      	CAN1_ChassisSend(&F105);//  发送F105结构体给底盘
//	}
}
uint32_t Chassis_high_water;

void CHASSIS_task(void *pvParameters)
{
    Pid_ChassisAngle();
//    F105.carSpeedx = 0;
//    F105.carSpeedy = 0;
//    F105.carSpeedw = 0;
    while (1) {
        ZeroCheck_cal();
        if(HeroStatus.ChassisMode == Chassis_CeaseMode)
            Chassis_Cease_Cal();
        else if(HeroStatus.ChassisMode == Chassis_Act_Mode)
            Chassis_Act_Cal();
        else if(HeroStatus.ChassisMode ==  Chassis_Sniper_Mode)
            Chassis_Sniper_Cal();
        else if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode)
            Chassis_SelfProtect_Cal();
				else if(HeroStatus.ChassisMode == Chassis_Solo_Mode)
            Chassis_Solo_Cal();
        Chassis_Cal();
        vTaskDelay(1); /* 延时 500 个 tick */

#if INCLUDE_uxTaskGetStackHighWaterMark
        Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

/**********************************************************************************************************
*函 数 名: F105_Rst
*功能说明: 功率板掉线复位
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
unsigned int Heat_LimitTick;
void F105_Rst()
{
    Heat_LimitTick++;
    if(Heat_LimitTick%50 ==1 )
        F105.IsShootAble = 1;
    else
        F105.IsShootAble =0;
    F105.Limit_Power_k = 0.25;
}

