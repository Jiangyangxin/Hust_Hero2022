#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/*--------------------------ȫ�ֱ���----------------------------*/
Pid_Typedef	PidYawPos, PidPitchPos, PidYawSpeed, PidPitchSpeed;
Pid_Typedef	PidYawArmorPos,PidPitchArmorPos;
Pid_Typedef PidPitchArmorSpeed,PidYawArmorSpeed;

Pid_Typedef PidLobYawPos,PidLobYawSpeed,PidLobPitchPos,PidLobPitchSpeed;

Fuzzle_Typedef Testpitchpid;

unsigned char	PC_Sendflag;
float GimbalPitPos,GimbalYawPos;
float ISGimbalAble=GimbalAble;//��̨ʹ�ܱ�־λ
AbsoluteAngle_typedef AbsoluteAngle;
short LastGimbaltype;
float LastGimbalPitPos,LastGimbalYawPos;
short PitchCur,YawCur;
short GIM_PIT_MAX_ANGLE =  35;
short GIM_PIT_MIN_ANGLE = -16;
/*--------------------------�����ⲿ����----------------------------*/
extern RobotInit_Struct Hero;
extern Gyro_t Gyro;
extern short BigFrictSpeed;
extern HeroStatus_typedef	HeroStatus;
extern RC_Ctl_t RC_Ctl;
extern GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
extern Key_typedef key_status;
extern Chassis_typedef F105;
extern Pid_Typedef	PidChassisAngle;
extern Pid_Typedef PidBigFrictSpeed[2];
extern ZeroCheck_Typedef ZeroCheck_Pitch,ZeroCheck_Yaw,ZeroCheck_Pitch_speed,ZeroCheck_Yaw_speed;
/*--------------------------�ڲ�����----------------------------*/
short turnHeadCnt = 0;
short turnHeadStep = 0;
short turnHeadExitFlag = 0;
short GimbalInitFlag = 0;
/**********************************************************************************************************
*�� �� ��: Gimbal_Act_Cal
*����˵��: ��̨�����˶�ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Act_Cal(void)
{


    if(GimbalInitFlag != 1)
    {
        PC_Sendflag=0x00;
        GimbalPitPos = 0;
        GimbalYawPos = Gyro.Yaw;
        GimbalInitFlag = 1;
    }

		//����
		if(LastGimbaltype==Gimbal_Armor_Mode)
		{
				 GimbalPitPos=LastGimbalPitPos;
		     GimbalYawPos=LastGimbalYawPos;

		}


    if(HeroStatus.CtrMode == RemoteMode)
    {
        GimbalPitPos -= -(RC_Ctl.rc.ch3 - 1024) * 0.0002f;
        GimbalYawPos += -(RC_Ctl.rc.ch2 - 1024) * 0.0002f;
    }
    if(HeroStatus.CtrMode == MouseKeyMode)
    {
        GimbalPitPos += RC_Ctl.mouse.y * (-0.004f);  //�Ǹ��ݰ���ʱ��������
        GimbalYawPos += RC_Ctl.mouse.x * (-0.004f);
			  GimbalPitPos += RC_Ctl.mouse.z * (0.001f);
    }
    //������
//    if(RC_Ctl.key.q == 1)
//        GimbalYawPos += 0.15f;
//    if(RC_Ctl.key.e == 1)
//        GimbalYawPos -= 0.15f;

//		get_F();
//		T_change();
//		Gimbal_System_identification(0,20);//����Ҫ�����˶�ʱ���ı��־λ Gimbal_System_start_flag
	
    //��λ
    GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,GIM_PIT_MAX_ANGLE,GIM_PIT_MIN_ANGLE);
		PidPitchPos.SetPoint = GimbalPitPos;
		PidYawPos.SetPoint   = GimbalYawPos; 
		Testpitchpid.SetPoint=GimbalPitPos;
	  LastGimbaltype=Gimbal_Act_Mode;

}



/**********************************************************************************************************
*�� �� ��: Gimbal_Lob_Cal
*����˵��: ����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Lob_Cal(void)
{
    if(GimbalInitFlag != 4)
    {
        PC_Sendflag=0x40;
//        GimbalPitPos = 0;
//        GimbalYawPos = AbsoluteAngle.Yaw ;
        GimbalInitFlag = 4;
			  GimbalYawPos=0;
			  GimbalPitPos=30;
    }
		//����
		if(LastGimbaltype==Gimbal_Armor_Mode)
		{
				 GimbalPitPos=LastGimbalPitPos;
		     GimbalYawPos=LastGimbalYawPos;
		}
    if(HeroStatus.CtrMode == RemoteMode)
    {
        GimbalPitPos -= -(RC_Ctl.rc.ch3 - 1024) * 0.0002f;
        GimbalYawPos += -(RC_Ctl.rc.ch2 - 1024) * 0.0002f;
    }
    if(HeroStatus.CtrMode == MouseKeyMode)
    {
        GimbalPitPos += RC_Ctl.mouse.y * (-0.004f);  //�Ǹ��ݰ���ʱ��������
        GimbalYawPos += RC_Ctl.mouse.x * (-0.004f);
			  GimbalPitPos += RC_Ctl.mouse.z * (0.001f);
//   			GimbalPitPos += RC_Ctl.key.w * (0.01f);  //�Ǹ��ݰ���ʱ��������
//			  GimbalPitPos -= RC_Ctl.key.s * (0.01f);	
//			  GimbalYawPos += RC_Ctl.key.a * (0.01f);
//			  GimbalYawPos -= RC_Ctl.key.d * (0.01f);			
    }
    //��λ
    GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,GIM_PIT_MAX_ANGLE,GIM_PIT_MIN_ANGLE);
		GimbalYawPos = LIMIT_MAX_MIN(GimbalYawPos,GIM_YAW_MAX_ANGLE,GIM_YAW_MIN_ANGLE);//ȡ���˵��̸��棬Ҫ��YAW������λ
		
		PidLobPitchPos.SetPoint = GimbalPitPos;
		PidLobYawPos.SetPoint   = GimbalYawPos; 		 
	  LastGimbaltype=Gimbal_Lob_Mode;
}

extern PC_Receive_t PC_Receive;
/**********************************************************************************************************
*�� �� ��: Gimbal_Armor_Cal
*����˵��: ��̨����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
extern Trajector hero_traj; //��������
float Recent_angle = 0;
void Gimbal_Armor_Cal(void)
{
     
    if(GimbalInitFlag != 2)
    {
        GimbalPitPos = AbsolutePitch();
        GimbalYawPos = Gyro.Yaw;
        GimbalInitFlag = 2;
        PC_Sendflag=0x10;

    }
    //Trajector_Equ(0.1,19.1,-0.5);
    //����ֵ����������,����ڵ���
    //Recent_angle = -PC_Receive.PCYaw - hero_traj.Add_GimbalPitchPos;
    Recent_angle = PC_Receive.PCYaw ;
    GimbalPitPos = PC_Receive.PCPitch;
//    while(ABS(Gyro.Yaw - Recent_angle)>=180 )
//    {
//        if(Gyro.Yaw<0)
//        {
//            Recent_angle = Recent_angle - 360; 
//        }
//        else
//        {
//            Recent_angle = Recent_angle + 360;
//        }
//    }

    GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,GIM_PIT_MAX_ANGLE,GIM_PIT_MIN_ANGLE);
		GimbalYawPos =Recent_angle;
    PidPitchArmorPos.SetPoint = GimbalPitPos;
    PidYawArmorPos.SetPoint = GimbalYawPos;
		
		 LastGimbalPitPos= GimbalPitPos;
		 LastGimbalYawPos=GimbalYawPos;
		
		LastGimbaltype=Gimbal_Armor_Mode;
}




/**********************************************************************************************************
*�� �� ��: GimbalCur_Pid_Cal
*����˵��: ��ȡ����ģʽ��Pit���Yaw�����ĵ���ֵ��������CAN2
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

float testpitchPosout;
void GimbalCur_Pid_Cal(void)
{
 if(HeroStatus.GimbalMode != Gimbal_CeaseMode)
  {
			if(HeroStatus.GimbalMode == Gimbal_Act_Mode)//�����˶�ģʽ
			{
//			PidPitchPos.ActPoint = AbsolutePitch(); 
								 Testpitchpid.ActPoint=AbsolutePitch();
//								 testpitchPosout= FuzzyPID_Calc(&Testpitchpid);
			//					 PidPitchPos.ActPoint   = Gyro.Pitch;

			           PidPitchSpeed.SetPoint = LIMIT_MAX_MIN(FuzzyPID_Calc(&Testpitchpid),7,-7);
							   PidPitchSpeed.ActPoint = Gyro.GY;
//			PidPitchSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidPitchPos),7,-7);
			PitchCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidPitchSpeed),30000,-30000);



			PidYawPos.ActPoint = Gyro.Yaw;
			PidYawSpeed.ActPoint = - Gyro.GZ;
			PidYawSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidYawPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidYawSpeed),30000,-30000);
			}

			else if(HeroStatus.GimbalMode == Gimbal_Armor_Mode) //����ģʽ
			{
			PidPitchArmorPos.ActPoint = AbsolutePitch();
			PidPitchArmorSpeed.ActPoint = Gyro.GY;
			PidPitchArmorSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidPitchArmorPos),7,-7);
			PitchCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidPitchArmorSpeed),30000,-30000);

			PidYawArmorPos.ActPoint = Gyro.Yaw;
			PidYawArmorSpeed.ActPoint = - Gyro.GZ;
			PidYawArmorSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidYawArmorPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidYawArmorSpeed),30000,-30000);
			}
			
		else if(HeroStatus.GimbalMode == Gimbal_Lob_Mode) //����ģʽ�£�ȡ�����̸��棬�õ��ֵΪʵ��ֵ
			{
			PidLobPitchPos.ActPoint = AbsolutePitch();
			PidLobPitchSpeed.ActPoint = Gyro.GY;
			PidLobPitchSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidLobPitchPos),7,-7);
			PitchCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidLobPitchSpeed),30000,-30000);

			PidLobYawPos.ActPoint = AbsoluteAngle.Yaw;
			PidLobYawSpeed.ActPoint = - Gyro.GZ;
			PidLobYawSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidLobYawPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidLobYawSpeed),30000,-30000);
			}

			CAN2_GimbalSend(PitchCur,YawCur);
 }

    else
    {
        PitchCur = 0;
        YawCur = 0;
        Gimbal_Cease_Cal();
    }
}

/**********************************************************************************************************
*�� �� ��: Gimbal_Cease_Cal
*����˵��: ��̨����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Cease_Cal(void)
{
    if(GimbalInitFlag != 3)
        GimbalInitFlag = 3;
    if((RC_Ctl.rc.ch0<400)&&(RC_Ctl.rc.ch3<400))
        PC_Sendflag=0xff;
    PidPitchSpeed.SetPoint = 0;
    PidYawSpeed.SetPoint = 0;
    PidPitchPos.SetPoint = Gyro.Pitch;
    PidYawPos.SetPoint = Gyro.Yaw;
	  
		LastGimbaltype=Gimbal_CeaseMode;
    CAN2_GimbalSend(0,0);
}


/**********************************************************************************************************
*�� �� ��: Pid_PitchGyroPosSpeed
*����˵��: Pitch�������ǣ��ٶȻ���λ�û�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

void Pid_PitchGyroPosSpeed(void)
{
  	PidPitchPos.SetPoint = GIM_PIT_INIT_SCALE;// 0.0f;
    PidPitchPos.P = -0.32f;//-0.3
    PidPitchPos.I = -0.00022f;//-0.00025
    PidPitchPos.D = 0;//-0.0001;
    PidPitchPos.IMax = 800.0f;

    PidPitchSpeed.SetPoint = 0.0f;
    PidPitchSpeed.P = 16000.0f;//10000
    PidPitchSpeed.I = 0.5f;//5
    PidPitchSpeed.D = 0;//-0.05;
    PidPitchSpeed.IMax = 800.0f;
	
	  Testpitchpid.SetPoint=GIM_PIT_INIT_SCALE;
	  Testpitchpid.Kp=-0.32f;
	  Testpitchpid.Ki=-0.0002f;
	  Testpitchpid.Kd=0;
	  Testpitchpid.IMax=800.0f;

}
/**********************************************************************************************************
*�� �� ��: Pid_YawGyroPosSpeed
*����˵��: Yaw�������ǣ��ٶȻ���λ�û�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_YawGyroPosSpeed(void)
{
    PidYawPos.SetPoint = 0.0f;
    PidYawPos.P = -0.30f;
    PidYawPos.I = -0.00005f;//-0.004;
    PidYawPos.D = 0.0f;//-0.0001;
    PidYawPos.IMax = 70.0f;

    PidYawSpeed.SetPoint = 0.0f;
    PidYawSpeed.P =12000.0f;//
    PidYawSpeed.I =0.5f;//5;
    PidYawSpeed.D =0;// 200;	
    PidYawSpeed.IMax = 500.0f;

}
/**********************************************************************************************************
*�� �� ��:Pid_PitchYaw_ArmorPos
*����˵��: ����ģʽYaw��Pitchλ�û����ٶȻ�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_PitchYaw_ArmorPos(void)
{
    PidYawArmorPos.SetPoint = 0.0f;
    PidYawArmorPos.P = -0.3f;
    PidYawArmorPos.I = -0.00015f;;//-0.002f;
    PidYawArmorPos.D = 0.0f;
    PidYawArmorPos.IMax = 70.0f;
    
	  PidYawArmorSpeed.SetPoint=0.0f;
    PidYawArmorSpeed.P=12000;
	  PidYawArmorSpeed.I=2.0f;
	  PidYawArmorSpeed.D=0;
	  PidYawArmorSpeed.IMax=500.0f;
	
	  PidPitchArmorPos.SetPoint = 0.0f;
    PidPitchArmorPos.P = -0.32;//-0.57		//0.2		//0.01
    PidPitchArmorPos.I = -0.0002;//-0.003;
    PidPitchArmorPos.D = -0;//-0.0002;
    PidPitchArmorPos.IMax = 100;
	
	  PidPitchArmorSpeed.SetPoint=0.0f;
	  PidPitchArmorSpeed.P=15000;
	  PidPitchArmorSpeed.I=1.5f;
		PidPitchArmorSpeed.D=0;
		PidPitchArmorSpeed.IMax=800.0f;
}


/**********************************************************************************************************
*�� �� ��:
*����˵��: ����ģʽYaw��Pitchλ�û����ٶȻ�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_LobInit(void)
{
	
    PidLobYawPos.SetPoint = 0.0f;
    PidLobYawPos.P = -0.35f;
    PidLobYawPos.I = -0.00025;//-0.002f;
    PidLobYawPos.D = 0.0f;
    PidLobYawPos.IMax = 70.0f;
    
	  PidLobYawSpeed.SetPoint=0.0f;
    PidLobYawSpeed.P=15500.0f;//12000
	  PidLobYawSpeed.I=5.0f;//8
	  PidLobYawSpeed.D=0;
	  PidLobYawSpeed.IMax=500.0f;
	

	  PidLobPitchPos.SetPoint = 0.0f;
    PidLobPitchPos.P = -0.35;//-0.35		//0.2		//0.01
    PidLobPitchPos.I = -0.0003f;//-0.0003f
    PidLobPitchPos.D = -0;//-0.0002;
    PidLobPitchPos.IMax = 800.0f;
	
	  PidLobPitchSpeed.SetPoint=0.0f;
	  PidLobPitchSpeed.P= 15000.0f;//14000.0f
	  PidLobPitchSpeed.I=2.5f;//2.0f
		PidLobPitchSpeed.D=0;
		PidLobPitchSpeed.IMax=800.0f;
}



/**********************************************************************************************************
*�� �� ��: GIMBAL_Rst
*����˵��: ��̨����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void GIMBAL_Rst()
{
    ISGimbalAble=GimbalDisable;//0��ʾʧ�ܣ�1��ʾʹ��
	   if((RC_Ctl.rc.ch0<400)&&(RC_Ctl.rc.ch3<400))
    PC_Sendflag=0xff;
    PidPitchSpeed.SetPoint = 0;
    PidYawSpeed.SetPoint = 0;
    PidPitchPos.SetPoint = Gyro.Pitch;
    PidYawPos.SetPoint = Gyro.Yaw;

    CAN2_GimbalSend(0,0);

}

/**********************************************************************************************************
*�� �� ��: AbsolutePitch
*����˵��: ����ԽǶ�
*��    ��: ��
*�� �� ֵ: ���ԽǶ�
*˵    ����˳ʱ��ת����Ϊ�Ƕȼ�С����ʱ�뷴֮����ʼʱ����Ϊ��ȡ����õ�ʱ���в���ֱ��ʹ��absolutepitch���������ֻ��Ϊ��watch�п��Կ��õ��������
**********************************************************************************************************/
float AbsolutePitch(void)//���µ�����߻���װ��һ��Ҫ�Ĳ������������м�
{
  AbsoluteAngle.Pitch=(GIM_PIT_INIT_SCALE-Gimbal_Motor_Pitch.Angle)*360.0f/8192;
//		AbsoluteAngle.Pitch=Gyro.Pitch+AbsoluteAngle.correctPit;//Ϊ�˿�����watch�ж������Լ��˱���  ��Ϊԭ��ʹ�������ǵ�ֵ
    return AbsoluteAngle.Pitch;
}

/**********************************************************************************************************
*�� �� ��: AbsoluteYaw
*����˵��: ����ԽǶ�
*��    ��: ��
*�� �� ֵ: ���ԽǶ�
*˵    ����˳ʱ��ת����Ϊ�Ƕȼ�С����ʱ�뷴֮����ʼʱ����Ϊ���
**********************************************************************************************************/
short circle;
float AbsoluteYaw(void)//���µ�����߻���װ��һ��Ҫ�Ĳ������������м�
{
	
	if(HeroStatus.ChassisMode==Chassis_Solo_Mode)
	{
		circle=(ZeroCheck_YawOutPut()-Hero.Solo_Yaw_init)/8192;  //�õ���ǰyaw������ת��������ٷֱ�
    AbsoluteAngle.Yaw = -(ZeroCheck_YawOutPut()-circle*8192-Hero.Solo_Yaw_init)/8192.0*360;//Ϊ�˿�����watch�ж������Լ��˱���
		AbsoluteAngle.TureYaw = -(ZeroCheck_YawOutPut()-circle*8192-GIM_YAW_INIT_SCALE)/8192.0*360;
		
		
		
			 if(AbsoluteAngle.TureYaw>180)
				 {
			AbsoluteAngle.TureYaw-=360;
		}
		else if(AbsoluteAngle.TureYaw<-180)
			{
			AbsoluteAngle.TureYaw+=360;
		}
	}
	else
		{
	circle=(ZeroCheck_YawOutPut()-Hero.Yaw_init)/8192;  //�õ���ǰyaw������ת��������ٷֱ�   ע����ֱ���1��1 ��һ�ܵ�ģ��ֵ�� 8192
  AbsoluteAngle.Yaw = -(ZeroCheck_YawOutPut()-circle*8192-Hero.Yaw_init)/8192.0*360;//Ϊ�˿�����watch�ж������Լ��˱���
	  }
	
	 if(AbsoluteAngle.Yaw>180){
			AbsoluteAngle.Yaw-=360;
		}
		else if(AbsoluteAngle.Yaw<-180){
			AbsoluteAngle.Yaw+=360;
		}
    return AbsoluteAngle.Yaw;
}
/**********************************************************************************************************
*�� �� ��: AbsoluteangleInit
*����˵��: ����ԽǶ�
*��    ��: ��
*�� �� ֵ: ���ԽǶ�
*˵    ����˳ʱ��ת����Ϊ�Ƕȼ�С����ʱ�뷴֮����ʼʱ����Ϊ���
**********************************************************************************************************/
void AbsoluteAngleInit(void)
{
	      delay_ms(100);
        AbsoluteAngle.PitchMotorPosInit=Gimbal_Motor_Pitch.Angle;
        AbsoluteAngle.correctPit=(GIM_PIT_INIT_SCALE-AbsoluteAngle.PitchMotorPosInit)*360.0f/8192;

}
/**********************************************************************************************************
*�� �� ��: GIMBAL_task
*����˵��: ��̨����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t GIMBAL_high_water;
void GIMBAL_task(void *pvParameters)
{

	  Pid_YawGyroPosSpeed();
    Pid_PitchGyroPosSpeed();
    Pid_PitchYaw_ArmorPos();
	  Pid_LobInit();
	  
    while (1)
			{
        ZeroCheck_cal();
				AbsoluteYaw();
        if(HeroStatus.GimbalMode == Gimbal_Act_Mode)
            Gimbal_Act_Cal();
        else if(HeroStatus.GimbalMode == Gimbal_CeaseMode)
            Gimbal_Cease_Cal();
				else if(HeroStatus.GimbalMode == Gimbal_Armor_Mode)
		        Gimbal_Armor_Cal();
				else if(HeroStatus.GimbalMode == Gimbal_Lob_Mode)
					  Gimbal_Lob_Cal();

        GimbalCur_Pid_Cal();
//	      const portTickType xFrequency = 1;
//				portTickType xLastWakeTime;
//				xLastWakeTime = xTaskGetTickCount();
//        vTaskDelayUntil(&xLastWakeTime,xFrequency); 
				
				
#if INCLUDE_uxTaskGetStackHighWaterMark
        GIMBAL_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
        vTaskDelay(1); /* ��ʱ 500 �� tick */
			}
			
}



////Gimbal_xΪyaw��,Gimbal_yΪpitch��,����Ƕȼ�Ϊ��Χ
///*���Է�Χ�Ƕ���+50��-50��*/
//int Gimbal_System_start_flag;
//float	Gimbal_direct;
//void Gimbal_System_identification(float Gimbal_x,float Gimbal_y)
//{
//	if(Gimbal_System_start_flag == 1)
//	{
//		if(Gimbal_x != 0&&Gimbal_y == 0)
//		{
//			GimbalYawPos =Gimbal_direct*Gimbal_x;
//		}
//		else if(Gimbal_x == 0&&Gimbal_y != 0)
//		{
//			GimbalPitPos = Gimbal_direct*Gimbal_y;
//		}
//	}
//}
//int T;//����
//int T_cnt = 0;//����
//int T_Time_cnt = 0;//���ڴ���������
//int F_cnt = 0,F_cnt_last;//ָ��F��ָ��
//float F = 1;
//int j;
//void get_F(void)
//{
//	if(F < 22)
//	{
//		j++;
//		F += 0.5f;
//	}
//	else if(F == 22)
//	{
//		F = 24;
//	}
//	else if(F >= 24&&F < 40)
//	{
//		F = F + 2;
//	}
//	else if(F == 40)
//	{
//		F = 50;
//	}
//	else if(F >= 50&&F < 120)
//	{
//		F = F + 10;
//	}
//	else if(F == 120)
//	{
//		F = 200;
//	}
//	else if(F == 250)
//	{
//		F = 333;
//	}
//	else if(F == 333)
//	{
//		F = 500;
//	}
//}
//int i;
//void T_change(void)
//{
//	T = round(300000/F);
//	if(Gimbal_System_start_flag == 1)
//	{
//    Gimbal_direct = sin(2*3.14*T_cnt/T);
//		if(T_cnt/T >= 1)
//		{
//			T_cnt = 0;
//			T_Time_cnt++;
//		}
//		T_cnt++;
//		F_cnt_last = F_cnt;

//		if(T_Time_cnt >= 10 && F <22)
//		{
//			T_Time_cnt = 0;
//			F_cnt++; 
//		}
//		else if(T_Time_cnt >= 10 && F >= 22 && F <50)
//		{
//			T_Time_cnt = 0;
//			F_cnt++; 
//		}
//		else if(T_Time_cnt >= 20 && F >= 50)
//		{
//			T_Time_cnt = 0;
//			F_cnt++; 
//		}
//		if(F_cnt != F_cnt_last)
//		{
//			i++;
//			get_F();
//		}
//	}
//	else if(Gimbal_System_start_flag == 0)
//	{
//		F = 1;
//	}
//}

