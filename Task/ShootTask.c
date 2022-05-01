#include "main.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/*--------------------------全局变量----------------------------*/
Pid_Typedef	PidBigPullerSpeed,PidBigFrictSpeed[2],PidBigPullerPos;
int SendToTx2BullectCnt;
/*--------------------------引用外部变量----------------------------*/
//extern Jugement_t Juge;
extern Key_typedef key_status;
extern HeroStatus_typedef			HeroStatus;
extern M3508Receive_Typedef		BigFrict[2],BigPuller;
extern GM6020Receive_Typedef	Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
extern RC_Ctl_t RC_Ctl;
extern Chassis_typedef 	F105;
extern short Frict_status;
/*--------------------------内部变量----------------------------*/

Shoot_typedef BigShoot;
static short ShootInitFlag;
short BigPullerDir = 3;
short BigFrictSpeed[2] = {3500,5100};
short BigPullerSpeed = 0;//600;
short BigPuller_PosAngle = 8191;
short BigPullerCom=26290;//26276   8191*减速比/6
short TrigFlag = 0;
short DangershootFlag=0; //紧急解除热量限制标志位
unsigned char BigFrictFlag = 0;
/**
  * @brief  发射机构动作模式
  * @param
  * @retval
  */
void Shoot_Act_Cal(void)
{
    if(ShootInitFlag != 1)
    {
        BigShoot.Priod = 300;
        PidBigPullerPos.SetPoint = ZeroCheck_BigPullerPosOutPut();//大拨盘角度过零检测后输出值
        BigShoot.RemainNum = 0;
        ShootInitFlag = 1;
    }

    if((ABS(BigFrict[0].RealSpeed)>1000)&&(ABS(BigFrict[0].RealSpeed)>1000))	//摩擦轮转动标志位
        BigFrictFlag = 1;   
    else
        BigFrictFlag = 0;
		

    Shoot_Fire_Cal();
}
/**
  * @brief  发射机构开火模式
  * @param
  * @retval
  */
 int count = 0;//count用来计算时间，当达到一定时间后摩擦轮会转动
void Shoot_Fire_Cal(void)
{//有两个摩擦轮，要分别计算
 
//	F105.bulletSpeedmax=16;//测试用，正式比赛要删掉

	
    if(F105.bulletSpeedmax==16){
				PidBigFrictSpeed[0].SetPoint = BigFrictSpeed[1];
				PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed[1];
		}
		else if(F105.bulletSpeedmax==10){
				PidBigFrictSpeed[0].SetPoint = BigFrictSpeed[0];
				PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed[0];
		}
		else //默认参数
		{
				PidBigFrictSpeed[0].SetPoint = BigFrictSpeed[1];
				PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed[1];
		}
	  
    if(HeroStatus.CtrMode == RemoteMode) //遥控检录模式，每过一段时间打蛋
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
			//测试用，正式比赛要删掉
//			BigFrictFlag=1;
//	    F105.IsShootAble=1;
			
			//另外还有心跳板的标志位 BigFrictAble 记得修改
			
		    key_status.mouse_l_rise_flag = RC_Ctl.mouse.press_l - key_status.pre_mouse_l;
				key_status.pre_mouse_l = RC_Ctl.mouse.press_l;
				
			if(key_status.mouse_l_rise_flag == 1&&BigFrictFlag==1&&F105.IsShootAble==1&&DangershootFlag==0)//保证摩擦轮在转的时候才会拨弹 //BigFrictFlag摩擦轮转动标志位
			    {   
					PidBigPullerPos.SetPoint -= BigPullerCom;
				  }
			else if(key_status.mouse_l_rise_flag == 1&&BigFrictFlag==1&&DangershootFlag==1)		//自杀模式 紧急解除热量限制 DangershootFlag
			   {				
					PidBigPullerPos.SetPoint -= BigPullerCom;
	
         }
     }	 
		
		 else if(HeroStatus.CtrMode == CeaseMode) //停止发射
		 {
		   HeroStatus.ShootMode=Shoot_CeaseMode;	 
		 }
	 
		 
		//    if(BigShoot.PullerCmd == 1&&BigShoot.Shoot_Finish == 0)
//    {
//        PidBigPullerPos.SetPoint -= BigPullerDir * BigPuller_PosAngle+1600;
//        BigShoot.Shoot_Finish = 1;
//    }
//堵转检测
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
  * @brief  发射机构掉电模式
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
		
		//键鼠模式下，一旦开启，无法停止
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
  * @brief  发射机构pid计算,Frict[0]给负速度，Frict[1]给正速度 左0右1
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


    PidBigFrictSpeed[0].ActPoint = BigFrict[0].RealSpeed;  //RealSpeed来自电机的反馈
    PidBigFrictSpeed[1].ActPoint = BigFrict[1].RealSpeed;
    BigFrictCur[0] = (short)LIMIT_MAX_MIN(PID_Calc(&PidBigFrictSpeed[0]),16000,-16000);
    BigFrictCur[1] = (short)LIMIT_MAX_MIN(PID_Calc(&PidBigFrictSpeed[1]),16000,-16000);

    PidBigPullerPos.ActPoint = ZeroCheck_BigPullerPosOutPut();//大拨盘角度过零检测后输出值，在ZeroCheck_cal()执行后得到
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
		CAN1_FrictSend(BigFrictCur[0],BigFrictCur[1],0,0);//向CAN线即电机发送电流
    CAN2_BigPullerSend(BigPullerCur);

}
/**
  * @brief  大拨盘速度环
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
  * @brief  大拨盘位置环
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
  * @brief  大摩擦轮
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
    Pid_BigFrictSpeed();//摩擦轮
    Pid_BigPullerSpeed();//大拨盘
    Pid_BigPullerPos();
    while (1) {
        ZeroCheck_cal();//获取大拨盘位置环的过零检测值

        if(HeroStatus.ShootMode == Shoot_Act_Mode) 
					{
            Shoot_Act_Cal();//开启射击
        }
        else if(HeroStatus.ShootMode == Shoot_CeaseMode)
        {
            Shoot_Cease_Cal();//关闭射击
        }
        ShootCur_Pid_Cal();

        vTaskDelay(1); /* 延时 500 个 tick */

#if INCLUDE_uxTaskGetStackHighWaterMark
        SHOOT_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
