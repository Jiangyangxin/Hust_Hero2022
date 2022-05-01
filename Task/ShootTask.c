#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/*--------------------------ȫ�ֱ���----------------------------*/
Pid_Typedef	PidBigPullerSpeed,PidBigFrictSpeed[2],PidBigPullerPos;
int SendToTx2BullectCnt;
/*--------------------------�����ⲿ����----------------------------*/
//extern Jugement_t Juge;
extern Key_typedef key_status;
extern HeroStatus_typedef			HeroStatus;
extern M3508Receive_Typedef		BigFrict[2],BigPuller;
extern GM6020Receive_Typedef	Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
extern RC_Ctl_t RC_Ctl;
extern Chassis_typedef 	F105;
extern short Frict_status;
/*--------------------------�ڲ�����----------------------------*/

Shoot_typedef BigShoot;
static short ShootInitFlag;
short BigPullerDir = 3;
short BigFrictSpeed[2] = {3500,5100};
short BigPullerSpeed = 0;//600;
short BigPuller_PosAngle = 8191;
short BigPullerCom=26290;//26276   8191*���ٱ�/6
short TrigFlag = 0;
short DangershootFlag=0; //��������������Ʊ�־λ
unsigned char BigFrictFlag = 0;
/**
  * @brief  �����������ģʽ
  * @param
  * @retval
  */
void Shoot_Act_Cal(void)
{
    if(ShootInitFlag != 1)
    {
        BigShoot.Priod = 300;
        PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();//���̽Ƕȹ���������ֵ
        BigShoot.RemainNum = 0;
        ShootInitFlag = 1;
    }

    if((ABS(BigFrict[0].RealSpeed)>1000)&&(ABS(BigFrict[0].RealSpeed)>1000))	//Ħ����ת����־λ
        BigFrictFlag = 1;   
    else
        BigFrictFlag = 0;
		

    Shoot_Fire_Cal();
}
/**
  * @brief  �����������ģʽ
  * @param
  * @retval
  */
 int count = 0;//count��������ʱ�䣬���ﵽһ��ʱ���Ħ���ֻ�ת��
void Shoot_Fire_Cal(void)
{//������Ħ���֣�Ҫ�ֱ����
 
//	F105.bulletSpeedmax=16;//�����ã���ʽ����Ҫɾ��

	
    if(F105.bulletSpeedmax==16){
				PidBigFrictSpeed[0].SetPoint = BigFrictSpeed[1];
				PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed[1];
		}
		else if(F105.bulletSpeedmax==10){
				PidBigFrictSpeed[0].SetPoint = BigFrictSpeed[0];
				PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed[0];
		}
		else //Ĭ�ϲ���
		{
				PidBigFrictSpeed[0].SetPoint = BigFrictSpeed[1];
				PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed[1];
		}
	  
    if(HeroStatus.CtrMode == RemoteMode) //ң�ؼ�¼ģʽ��ÿ��һ��ʱ���
    {	
//			BigFrictFlag=1;
//	    F105.IsShootAble=1;
			
				count ++;
			
				if((count%4000) == 0&&BigFrictFlag==1&&F105.IsShootAble==1) 
					{
						PidBigPullerPos.SetPoint -= BigPullerCom;
						count = 0;
				  }
    }
    else if(HeroStatus.CtrMode == MouseKeyMode)
    {
			//�����ã���ʽ����Ҫɾ��
//			BigFrictFlag=1;
//	    F105.IsShootAble=1;
			
			//���⻹��������ı�־λ BigFrictAble �ǵ��޸�
			
		    key_status.mouse_l_rise_flag = RC_Ctl.mouse.press_l - key_status.pre_mouse_l;
				key_status.pre_mouse_l = RC_Ctl.mouse.press_l;
				
			if(key_status.mouse_l_rise_flag == 1&&BigFrictFlag==1&&F105.IsShootAble==1&&DangershootFlag==0)//��֤Ħ������ת��ʱ��ŻᲦ�� //BigFrictFlagĦ����ת����־λ
			    {   
					PidBigPullerPos.SetPoint -= BigPullerCom;
				  }
			else if(key_status.mouse_l_rise_flag == 1&&BigFrictFlag==1&&DangershootFlag==1)		//��ɱģʽ ��������������� DangershootFlag
			   {				
					PidBigPullerPos.SetPoint -= BigPullerCom;
	
         }
     }	 
		
		 else if(HeroStatus.CtrMode == CeaseMode) //ֹͣ����
		 {
		   HeroStatus.ShootMode=Shoot_CeaseMode;	 
		 }
	 
		 
		//    if(BigShoot.PullerCmd == 1&&BigShoot.Shoot_Finish == 0)
//    {
//        PidBigPullerPos.SetPoint -= BigPullerDir * BigPuller_PosAngle+1600;
//        BigShoot.Shoot_Finish = 1;
//    }
//��ת���
		if(ABS(PidBigPullerPos.PreError)>1000)
			BigShoot.StuckTimeCnt++;
		else
			BigShoot.StuckTimeCnt = 0;
		if(BigShoot.StuckTimeCnt>800)
		{
			BigShoot.StuckTimeCnt = 0;
			PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();
			PidBigPullerSpeed.SetPoint = 0;
		}
}
/**
  * @brief  �����������ģʽ
  * @param
  * @retval
  */
void Shoot_Cease_Cal(void)
{

    if(ShootInitFlag != 2){
					ShootInitFlag = 2;
				}
//Laser_OFF;
    if(HeroStatus.CtrMode==RemoteMode){
		PidBigFrictSpeed[0].SetPoint = 0;
    PidBigFrictSpeed[1].SetPoint = 0;
//    PidBigPullerSpeed.SetPoint = 0;
			PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();
		}
		
		//����ģʽ�£�һ���������޷�ֹͣ
		else
		{
   	PidBigFrictSpeed[0].SetPoint = 0;
    PidBigFrictSpeed[1].SetPoint = 0;
		PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();
		}
		
		
		
		
		//PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();
//		if(HeroStatus.CtrMode==MouseKeyMode&&PidBigPullerPos.SetPoint==PidBigPullerPos.ActPoint){
//		PidBigFrictSpeed[0].SetPoint = 0;
//    PidBigFrictSpeed[1].SetPoint = 0;
////    PidBigPullerSpeed.SetPoint = 0;
//			PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();
//		}

}

/**
  * @brief  �������pid����,Frict[0]�����ٶȣ�Frict[1]�����ٶ� ��0��1
  * @param
  * @retval
  */
float BigFrictSetLE;
float BigFrictSetRI;
float BigFrictActLE;
float BigFrictActRI;
float ErrorLE;
float ErrorRI;
float ErrorBo;
short BigFrictCur[2], BigPullerCur;
void ShootCur_Pid_Cal(void)
{


    PidBigFrictSpeed[0].ActPoint = BigFrict[0].RealSpeed;  //RealSpeed���Ե���ķ���
    PidBigFrictSpeed[1].ActPoint = BigFrict[1].RealSpeed;
    BigFrictCur[0] = (short)LIMIT_MAX_MIN(PID_Calc(&PidBigFrictSpeed[0]),16000,-16000);
    BigFrictCur[1] = (short)LIMIT_MAX_MIN(PID_Calc(&PidBigFrictSpeed[1]),16000,-16000);

    PidBigPullerPos.ActPoint = ZeroCheck_BigPullerPosOutPut();//���̽Ƕȹ���������ֵ����ZeroCheck_cal()ִ�к�õ�
    PidBigPullerSpeed.ActPoint = BigPuller.RealSpeed;
    PidBigPullerSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidBigPullerPos),5000,-5000);
    BigPullerCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidBigPullerSpeed),16000,-16000);
   
		BigFrictActLE = PidBigFrictSpeed[0].ActPoint;
		BigFrictActRI = -PidBigFrictSpeed[1].ActPoint;
		ErrorLE=BigFrictSetLE-BigFrictActLE;
		ErrorRI=BigFrictSetRI-BigFrictActRI;
		ErrorBo=BigFrictActLE-BigFrictActRI;
	
	if((ABS(BigFrict[0].RealSpeed)>1000)&&(ABS(BigFrict[0].RealSpeed)>1000))
		Frict_status=1;
	else
		Frict_status=0;
		
//		
		CAN1_FrictSend(BigFrictCur[0],BigFrictCur[1],0,0);//��CAN�߼�������͵���
    CAN2_BigPullerSend(BigPullerCur);

}
/**
  * @brief  �����ٶȻ�
  * @param
  * @retval
  */
void Pid_BigPullerSpeed(void)
{
    PidBigPullerSpeed.SetPoint = 0.0f;
    PidBigPullerSpeed.P = 10.0f;
    PidBigPullerSpeed.I = 0.002f;
    PidBigPullerSpeed.D	= 5.0f;
    PidBigPullerSpeed.IMax = 1000;
}

/**
  * @brief  ����λ�û�
  * @param
  * @retval
  */
void Pid_BigPullerPos(void)
{
    PidBigPullerPos.SetPoint = 0.0f;
    PidBigPullerPos.P = 0.3f;
    PidBigPullerPos.I = 0.002f;
    PidBigPullerPos.D	= 0.0f;
    PidBigPullerPos.IMax = 1000;
}

/**
  * @brief  ��Ħ����
  * @param
  * @retval
  */
void Pid_BigFrictSpeed(void)
{                                    
    PidBigFrictSpeed[0].SetPoint = 0.0f;
    PidBigFrictSpeed[0].P = 30; 
    PidBigFrictSpeed[0].I = 0;       
    PidBigFrictSpeed[0].D	= 0;         
    PidBigFrictSpeed[0].IMax = 0;
                                     
    PidBigFrictSpeed[1].SetPoint = 0.0f;
    PidBigFrictSpeed[1].P = 30;     
    PidBigFrictSpeed[1].I = 0;      
    PidBigFrictSpeed[1].D	= 0;      
    PidBigFrictSpeed[1].IMax = 0;
}

uint32_t SHOOT_high_water;

void SHOOT_task(void *pvParameters)
{
    Pid_BigFrictSpeed();//Ħ����
    Pid_BigPullerSpeed();//����
    Pid_BigPullerPos();
    while (1) {
        ZeroCheck_cal();//��ȡ����λ�û��Ĺ�����ֵ

        if(HeroStatus.ShootMode == Shoot_Act_Mode) 
					{
            Shoot_Act_Cal();//�������
        }
        else if(HeroStatus.ShootMode == Shoot_CeaseMode)
        {
            Shoot_Cease_Cal();//�ر����
        }
        ShootCur_Pid_Cal();

        vTaskDelay(1); /* ��ʱ 500 �� tick */

#if INCLUDE_uxTaskGetStackHighWaterMark
        SHOOT_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
