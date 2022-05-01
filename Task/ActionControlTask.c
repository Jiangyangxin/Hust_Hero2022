#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/*--------------------------ȫ�ֱ���----------------------------*/
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
/*--------------------------�����ⲿ����----------------------------*/
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
extern unsigned char ShootStatus;//��Сװ�׵����� 1С0��
extern short DangershootFlag;
extern F405_typedef F405;
/*--------------------------�ڲ�����----------------------------*/
/**
  * @brief  ����ģʽ
  * @param
  * @retval
  */

void SetInputMode(RC_Ctl_t* RC_Ctl)
{
    switch(RC_Ctl->rc.s2)  //��2�����������ǹ���ģ���������ĳһ��
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

/** @brief  �ϵ�ģʽ
   *@param  None
   *@retval None
*/
short ShutdownCnt = 0;
void Powerdown_Process(void)
{
    if(Mouse_Key_Flag != 2)
    {
        PC_Sendflag= 0x00;//Ҫȷ�����͸�PC��������Ϊ0
        Mouse_Key_Flag = 2;
    }
    HeroStatus.GimbalMode = Gimbal_CeaseMode;
    HeroStatus.ChassisMode = Chassis_CeaseMode;
    HeroStatus.ShootMode = Shoot_CeaseMode;
    if((RC_Ctl.rc.ch0 < 400) && (RC_Ctl.rc.ch2 > 1500)) //�ӽ��ڼ���
        ShutdownCnt++;
    if(ShutdownCnt > 100)
        PC_Sendflag= 0x30; //100����
}

/** @brief  ң����ģʽ
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
		
    F105.SuperCapCmd = 0;//�������ݱ�־λ
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
		HeroStatus.ChassisMode =Chassis_Act_Mode;//����״̬�¹رյ��̸��棬�������
		HeroStatus.ShootMode = Shoot_CeaseMode;
    laser_status=on;

        break;
    case 3:
       // HeroStatus.ShootMode = Shoot_Act_Mode;
        HeroStatus.GimbalMode =Gimbal_Armor_Mode;
        HeroStatus.ChassisMode = Chassis_Act_Mode;//����״̬�¹رյ��̸��棬�������
//		    HeroStatus.ChassisMode = Chassis_Sniper_Mode;
        HeroStatus.ShootMode = Shoot_CeaseMode;
    laser_status=off;
        break;


    case 2:
        HeroStatus.GimbalMode = Gimbal_Armor_Mode; //����ģʽGimbal_Lob_Mode
		    HeroStatus.ChassisMode = Chassis_Act_Mode;
        HeroStatus.ShootMode = Shoot_Act_Mode;
		laser_status=off;
        break;
    default:
        break;
    }
}

/** @brief  ����ģʽ
						w,s,a,d					ǰ�������н�
						shift						���ݼ���
						ctrl						С����
						press_l					�����������
						press_r					����
						q, e						����������
						x							  ���������������
						b								�Խӣ�������
						g								�Խ�SOLO
						v								�ѻ�ģʽ
						z								תͷ
						r								��������
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

    /**********************************�������ݿ���*****************************/
    if(RC_Ctl->key.shift == 1)
        F105.SuperCapCmd = 1;
    else
        F105.SuperCapCmd = 0;

//    key_status.z_rise_flag = RC_Ctl->key.z - key_status.pre_key_z; //��ֵ
//    key_status.pre_key_z = RC_Ctl->key.z;
//    if(key_status.z_rise_flag == 1)
//    {
//        (superCapAutoFlag == 1)?(superCapAutoFlag = 0):(superCapAutoFlag = 1); //ȡ��
//    }

    /**********************************��������*****************************/
    if(RC_Ctl->key.r == 1)
        slowFac = 2.5;
    else
        slowFac = 5;
    /**********************************���ݿ���*****************************/
    if(RC_Ctl->key.ctrl == 1)
    {
        if(HeroStatus.ChassisMode == Chassis_Act_Mode||HeroStatus.ChassisMode==Chassis_Solo_Mode) //���������������ģʽ�Ͳ���ѡ����
            HeroStatus.ChassisMode = Chassis_SelfProtect_Mode;
    }
    else //û��������ģʽ
    {   if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode&&SOLO_status==on)
			     HeroStatus.ChassisMode=Chassis_Solo_Mode;
			  else if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode)
            HeroStatus.ChassisMode = Chassis_Act_Mode; //����Ѿ���������ģʽ�ͽ�ֹ
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
		
		
		
		
		
		
		
    /**********************************�������*****************************/
    if(RC_Ctl->mouse.press_l==1) //һ������ģʽ��������ģʽ�����޷�ֹͣ
    {
			Fire_Init_flag=1; //���˳�����ģʽ��ʱ�����ֵ�Ż��0 
    }
		if(Fire_Init_flag==1)
		{
		    HeroStatus.ShootMode = Shoot_Act_Mode;
		}
		else
		{
        HeroStatus.ShootMode = Shoot_CeaseMode;

    }
    /**********************************�������*****************************/
    if(RC_Ctl->mouse.press_r == 1)
    {
        if(HeroStatus.GimbalMode == Gimbal_Act_Mode||HeroStatus.GimbalMode==Gimbal_Lob_Mode)
        {
            laser_status=off; //����ģʽ�ؼ���
            HeroStatus.GimbalMode = Gimbal_Armor_Mode;
            PC_Sendflag= 0x10; //�����źŸ�PC���������Ʒ���
        }
    }
    else
    {
        if(HeroStatus.GimbalMode == Gimbal_Armor_Mode)
        {
            laser_status=on;
            HeroStatus.GimbalMode = Gimbal_Act_Mode;
            PC_Sendflag= 0x50; //���źŸ�PC�����������Ʒ���
        }
    }
		
		
		
		/**********************************���������������*****************************/
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

		
		/**********************************�������*****************************/
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
		/**********************************�ѻ�ģʽ*****************************/
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
		/**********************************תͷ*****************************/
    key_status.z_rise_flag = RC_Ctl->key.z - key_status.pre_key_z;
    key_status.pre_key_z = RC_Ctl->key.z;
    if(key_status.z_rise_flag == 1)
    {
         ZeroCheck_Yaw.Circle+=1; //�޸Ĺ������ֵ���൱���趨�Ƕ�ģ��ֵ+8192
     }

		/**********************************�л��Խ�SOLOģʽ*****************************/ 
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
		
		/**********************************UI����*****************************/ 
		
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
//    /**********************************�л�����ģʽ*****************************/
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
//        {   //�����Ϊʲôֻ��һ�ν�����У��������ף�����Ϊֻ�а�����Ӧ�ļ����ܽ������������Բ���Ҫ��ʱ�жϡ�
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
	  F405.Laser_Flag=laser_status;//��0��1
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

