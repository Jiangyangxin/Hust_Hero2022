#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/*--------------------------全局变量----------------------------*/
HeroStatus_typedef	HeroStatus;
//short last_rc_s1;
short Mouse_Key_Flag = 0;
extern unsigned char	PC_Sendflag;
Key_typedef key_status;
short laser_status=off;
short Frict_status=off;
short Sniper_status=off;
short SOLO_status=off;
short Fire_Init_flag=0;
short lastSniper_status;
short lastSOLO_status;
short Graphic_Init_flag=1;
/*--------------------------引用外部变量----------------------------*/
extern Shoot_typedef 		BigShoot;
extern Pid_Typedef			PidBigPullerSpeed;
extern Pid_Typedef			PidBigFrictSpeed[2],PidSmlFrictSpeed[2];
extern short 						BigFrictSpeed,SmlFrictSpeed;
extern Chassis_typedef 	F105;
extern float GimbalPitPos,GimbalYawPos;
extern short turnHeadExitFlag;
extern RC_Ctl_t	RC_Ctl;
extern M3508Receive_Typedef		BigFrict[2];
extern float slowFac;
extern unsigned char superCapAutoFlag;
extern ZeroCheck_Typedef ZeroCheck_Yaw;
extern unsigned char ShootStatus;//大小装甲的区分 1小0大
extern short DangershootFlag;
extern F405_typedef F405;
/*--------------------------内部变量----------------------------*/
/**
  * @brief  控制模式
  * @param
  * @retval
  */

void SetInputMode(RC_Ctl_t* RC_Ctl)
{
    switch(RC_Ctl->rc.s2)  //这2个键的作用是共享的，不局限于某一个
    {
    case 1:
        HeroStatus.CtrMode=MouseKeyMode;
        break;
    case 2:
        HeroStatus.CtrMode=CeaseMode;
        break;
    case 3:
        HeroStatus.CtrMode=RemoteMode;
        break;
    }
}

/** @brief  断电模式
   *@param  None
   *@retval None
*/
short ShutdownCnt = 0;
void Powerdown_Process(void)
{
    if(Mouse_Key_Flag != 2)
    {
        PC_Sendflag= 0x00;//要确保发送给PC的数据是为0
        Mouse_Key_Flag = 2;
    }
    HeroStatus.GimbalMode = Gimbal_CeaseMode;
    HeroStatus.ChassisMode = Chassis_CeaseMode;
    HeroStatus.ShootMode = Shoot_CeaseMode;
    if((RC_Ctl.rc.ch0 < 400) && (RC_Ctl.rc.ch2 > 1500)) //接近于极限
        ShutdownCnt++;
    if(ShutdownCnt > 100)
        PC_Sendflag= 0x30; //100毫秒
}

/** @brief  遥控器模式
   *@param  None
   *@retval None
*/
extern Pid_Typedef	PidBigPullerSpeed,PidBigPullerPos;
extern short BigPullerSpeed ;
void Remote_Process(RC_Ctl_t* RC_Ctl)
{
    if(Mouse_Key_Flag != 3)
    {
        Mouse_Key_Flag = 3;
        PC_Sendflag= 0x50;
        HeroStatus.GimbalMode  = Gimbal_Act_Mode;
        HeroStatus.ChassisMode = Chassis_Act_Mode;
        HeroStatus.ShootMode = Shoot_Act_Mode;

    }
		
    F105.SuperCapCmd = 0;//超级电容标志位
		Fire_Init_flag=0;
		
	   switch(laser_status)
		{
			case on:
				LASER_ON;
			  break;
			case off:
				LASER_OFF;
			  break;
			default:
				break;
		}
		
		
    switch(RC_Ctl->rc.s1)
    {
    case 1:
    HeroStatus.GimbalMode = Gimbal_Act_Mode;
		HeroStatus.ChassisMode =Chassis_Act_Mode;//辅瞄状态下关闭底盘跟随，方便调试
		HeroStatus.ShootMode = Shoot_CeaseMode;
    laser_status=on;

        break;
    case 3:
       // HeroStatus.ShootMode = Shoot_Act_Mode;
        HeroStatus.GimbalMode =Gimbal_Armor_Mode;
        HeroStatus.ChassisMode = Chassis_Act_Mode;//辅瞄状态下关闭底盘跟随，方便调试
//		    HeroStatus.ChassisMode = Chassis_Sniper_Mode;
        HeroStatus.ShootMode = Shoot_CeaseMode;
    laser_status=off;
        break;


    case 2:
        HeroStatus.GimbalMode = Gimbal_Armor_Mode; //吊射模式Gimbal_Lob_Mode
		    HeroStatus.ChassisMode = Chassis_Act_Mode;
        HeroStatus.ShootMode = Shoot_Act_Mode;
		laser_status=off;
        break;
    default:
        break;
    }
}

/** @brief  键鼠模式
						w,s,a,d					前后左右行进
						shift						电容加速
						ctrl						小陀螺
						press_l					射击（单击）
						press_r					辅瞄
						q, e						左旋，右旋
						x							  紧急解除热量限制
						b								对接（单击）
						g								对角SOLO
						v								狙击模式
						z								转头
						r								慢速爬坡
   *@param  None
   *@retval None
*/
void Mouse_Key_Process(RC_Ctl_t* RC_Ctl)
{
    if(Mouse_Key_Flag != 1)
    {
        HeroStatus.GimbalMode = Gimbal_Act_Mode;
        HeroStatus.ChassisMode = Chassis_Act_Mode;
        HeroStatus.ShootMode = Shoot_Act_Mode;
        PC_Sendflag= 0x50;
        Mouse_Key_Flag = 1;
    }

    /**********************************超级电容控制*****************************/
    if(RC_Ctl->key.shift == 1)
        F105.SuperCapCmd = 1;
    else
        F105.SuperCapCmd = 0;

//    key_status.z_rise_flag = RC_Ctl->key.z - key_status.pre_key_z; //差值
//    key_status.pre_key_z = RC_Ctl->key.z;
//    if(key_status.z_rise_flag == 1)
//    {
//        (superCapAutoFlag == 1)?(superCapAutoFlag = 0):(superCapAutoFlag = 1); //取反
//    }

    /**********************************慢速爬坡*****************************/
    if(RC_Ctl->key.r == 1)
        slowFac = 2.5;
    else
        slowFac = 5;
    /**********************************陀螺控制*****************************/
    if(RC_Ctl->key.ctrl == 1)
    {
        if(HeroStatus.ChassisMode == Chassis_Act_Mode||HeroStatus.ChassisMode==Chassis_Solo_Mode) //如果本来就是陀螺模式就不用选择了
            HeroStatus.ChassisMode = Chassis_SelfProtect_Mode;
    }
    else //没有用陀螺模式
    {   if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode&&SOLO_status==on)
			     HeroStatus.ChassisMode=Chassis_Solo_Mode;
			  else if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode)
            HeroStatus.ChassisMode = Chassis_Act_Mode; //如果已经用来陀螺模式就禁止
    }

		
    switch(RC_Ctl->rc.s1)
    {
    case 1:

        break;
    case 3:
			
     HeroStatus.ChassisMode = Chassis_SelfProtect_Mode;			
        break;

    case 2:

        break;
    default:
        break;
    }		
		
		
		
		
		
		
		
    /**********************************发射控制*****************************/
    if(RC_Ctl->mouse.press_l==1) //一旦键鼠模式开启发射模式，则无法停止
    {
			Fire_Init_flag=1; //当退出键鼠模式的时候，这个值才会归0 
    }
		if(Fire_Init_flag==1)
		{
		    HeroStatus.ShootMode = Shoot_Act_Mode;
		}
		else
		{
        HeroStatus.ShootMode = Shoot_CeaseMode;

    }
    /**********************************辅瞄控制*****************************/
    if(RC_Ctl->mouse.press_r == 1)
    {
        if(HeroStatus.GimbalMode == Gimbal_Act_Mode||HeroStatus.GimbalMode==Gimbal_Lob_Mode)
        {
            laser_status=off; //辅瞄模式关激光
            HeroStatus.GimbalMode = Gimbal_Armor_Mode;
            PC_Sendflag= 0x10; //发送信号给PC让它来控制发弹
        }
    }
    else
    {
        if(HeroStatus.GimbalMode == Gimbal_Armor_Mode)
        {
            laser_status=on;
            HeroStatus.GimbalMode = Gimbal_Act_Mode;
            PC_Sendflag= 0x50; //送信号给PC不让它来控制发弹
        }
    }
		
		
		
		/**********************************紧急解除热量限制*****************************/
		key_status.x_rise_flag = RC_Ctl->key.x - key_status.pre_key_x;
    key_status.pre_key_x = RC_Ctl->key.x;
		
		
		if(key_status.x_rise_flag == 1&&RC_Ctl->key.c==1)
			{
			
				if(DangershootFlag==0)
					DangershootFlag=1;
				else 
					DangershootFlag=0;
      }
			
			if(key_status.x_rise_flag == 1&&RC_Ctl->key.c!=1)
			{
							DangershootFlag=0;
			}

		
		/**********************************激光控制*****************************/
		key_status.f_rise_flag = RC_Ctl->key.f - key_status.pre_key_f;
    key_status.pre_key_f = RC_Ctl->key.f;
    if(key_status.f_rise_flag == 1)
			{
			laser_status=(laser_status==on)?off:on;
      }
		switch(laser_status)
		{
			case on:
				LASER_ON;
			  break;
			case off:
				LASER_OFF;
			  break;
			default:
				break;
		}
		/**********************************狙击模式*****************************/
		key_status.v_rise_flag = RC_Ctl->key.v - key_status.pre_key_v;
    key_status.pre_key_v = RC_Ctl->key.v;
		
    if(key_status.v_rise_flag == 1){
			Sniper_status=(Sniper_status==on)?off:on;
    }
		if(Sniper_status!=lastSniper_status){
			switch(Sniper_status)
			{
				case on:
					HeroStatus.ChassisMode = Chassis_Sniper_Mode;//Chassis_Sniper_Mode
				  HeroStatus.GimbalMode =  Gimbal_Act_Mode;//Gimbal_Lob_Mode
					break;
				case off:
					HeroStatus.ChassisMode = Chassis_Act_Mode;
				  HeroStatus.GimbalMode  = Gimbal_Act_Mode;
					break;
				default:
					break;
			}
		}
		lastSniper_status=Sniper_status;
		/**********************************转头*****************************/
    key_status.z_rise_flag = RC_Ctl->key.z - key_status.pre_key_z;
    key_status.pre_key_z = RC_Ctl->key.z;
    if(key_status.z_rise_flag == 1)
    {
         ZeroCheck_Yaw.Circle+=1; //修改过零检测的值，相当于设定角度模拟值+8192
     }

		/**********************************切换对角SOLO模式*****************************/ 
	  key_status.g_rise_flag = RC_Ctl->key.g - key_status.pre_key_g;
    key_status.pre_key_g = RC_Ctl->key.g;
		  if(key_status.g_rise_flag == 1)
    {
		   SOLO_status=(SOLO_status==on)?off:on;
		}
		 	if(SOLO_status!=lastSOLO_status)
		{
			switch(SOLO_status)
			{
				case on:
					HeroStatus.ChassisMode = 	Chassis_Solo_Mode;
					break;
				case off:
					HeroStatus.ChassisMode = Chassis_Act_Mode;
					break;
				default:
					break;
			}
		}
    lastSOLO_status=SOLO_status; 
		
		/**********************************UI控制*****************************/ 
		
		key_status.b_rise_flag = RC_Ctl->key.b - key_status.pre_key_b;
    key_status.pre_key_b = RC_Ctl->key.b;
			if(key_status.b_rise_flag == 1)
	{
			
			switch(F405.Graphic_Init_Flag)
		{
			case 0:
			  F405.Graphic_Init_Flag = 1;
			  break;
			case 1:
				F405.Graphic_Init_Flag = 0;
			  break;
			default:
				break;
		}
	}
//    /**********************************切换单轴模式*****************************/
//    if(RC_Ctl->key.g == 1)
//    {
//        if((HeroStatus.ChassisMode == Chassis_Act_Mode) && (HeroStatus.GimbalMode == Gimbal_Act_Mode))
//        {
//            HeroStatus.ChassisMode = Chassis_Sniper_Mode;
//            HeroStatus.GimbalMode = Gimbal_SingleMode;
//        }
//    }
//    else
//    {
//        if((HeroStatus.ChassisMode == Chassis_Single_Mode) && (HeroStatus.GimbalMode == Gimbal_SingleMode))
//        {   //不清楚为什么只有一次进入就行，现在明白，是因为只有按下相应的键才能进入这额函数，所以不需要延时判断。
//            HeroStatus.ChassisMode = Chassis_Act_Mode;
//            HeroStatus.GimbalMode = Gimbal_Act_Mode;
//        }
//    }

}




void GraphicsUpdate(void)
{
   	F405.Chassis_Flag=HeroStatus.ChassisMode;
	  F405.Follow_state=AbsoluteYaw();
	  F405.Gimbal_100=AbsolutePitch()*100;
	  F405.Gimbal_Flag=HeroStatus.GimbalMode;
	  F405.Laser_Flag=laser_status;//关0开1
	  F405.DangershootFlag=DangershootFlag;
    F405.Frict_state=Frict_status;
	F405.SuperPowerLimit=F105.SuperCapCmd;

}

uint32_t ActionControl_high_water;
void ActionControl_task(void *pvParameters)
{

    while (1) {

        SetInputMode(&RC_Ctl);
        switch(HeroStatus.CtrMode)
        {
        case MouseKeyMode:
            Mouse_Key_Process(&RC_Ctl);
            break;
        case RemoteMode:
            Remote_Process(&RC_Ctl);
            break;
        case CeaseMode:
            Powerdown_Process();
            break;
        default:
            break;
        }
				GraphicsUpdate();

        vTaskDelay(2); 

#if INCLUDE_uxTaskGetStackHighWaterMark
        ActionControl_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

