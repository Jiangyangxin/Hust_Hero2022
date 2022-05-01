/**********************************************************************************************************
2020.11
**********************************************************************************************************/


/*--------------------------头文件----------------------------*/
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

/*--------------------------函数声明----------------------------*/
void Algorithm_Init(void);
void Peripheraldevice_Init(void);
void SteeringEngineInit(void);
/*--------------------------全局变量----------------------------*/
RobotInit_Struct Hero;
/*--------------------------内部变量----------------------------*/
short Juge_DisConnect = 0;
unsigned char Juge_Flag = 1;
unsigned char RC_Flag = 1;		//0为掉线，1为正常

/*--------------------------引用外部变量----------------------------*/
extern KalmanFilter_t pitch_Kalman, yaw_Kalman;
extern KalmanFilter_Init_t K;
extern short KalMan_doneflag;
extern RC_Ctl_t RC_Ctl;
extern short RC_DisConnect;//遥控器掉线检测
extern short F105_DisConect;//功率板掉线检测
extern short Gimbal_DisConnect;//云台掉线检测

//float i=1;
float angle=0;
/**********************************************************************************************************
*函 数 名: main
*功能说明: 主函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int main()
{
    
    

  	Peripheraldevice_Init();
    delay_ms(5000);
    Algorithm_Init();
    delay_ms(5000);
    LED_ON_BLUE();
	  LED_ON_RED();
	  IWDG_Config(IWDG_Prescaler_64,1250);//625
	  startTast();
//开启系统调度
    vTaskStartScheduler();
    
    while(1) {  }
}
/**********************************************************************************************************
*函 数 名: System_Configration
*功能说明: 外设初始化函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Peripheraldevice_Init(void)
{
 LED_Conf();

/************************************
	CAN配置
	CAN用于板间，电调的通信
************************************/    
	  CAN1_Configration();
    delay_ms(100);
    CAN2_Configration();
    delay_ms(100);
/************************************
	串口配置
	1：遥控器
	5：裁判系统
	6：PC通信
************************************/  

    USART3_Configuration();
    delay_ms(100);
    UART5_Configuration();
    delay_ms(100);

/************************************
	TIMER配置
	用途？？？？？？？？？
************************************/  
    TIM4_Init();
		TIM7_Configuration();
/************************************
	其他配置
  遥控器
	弹仓盖舵机
	激光器
************************************/  
		RemoteInit(&RC_Ctl);//遥控器
		delay_ms(100);
		//SteeringEngine_Configuration();//弹仓盖舵机
		SteeringEngineInit();
	  delay_ms(100);
		LASER_Configration();
}

/**********************************************************************************************************
*函 数 名: Algorithm_Init
*功能说明: 各种算法的初始化函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Algorithm_Init(void)
{
/************************************
	算法配置
  过零检测
	卡尔曼滤波
************************************/  
	  ZeroCheck_Init();
    KalmanFilter_Init(&pitch_Kalman, &K);
    KalmanFilter_Init(&yaw_Kalman, &K);
	  KalMan_doneflag=1; 
	  AbsoluteAngleInit();
}


/**********************************************************************************************************
*函 数 名: SteeringEngineInit
*功能说明: 弹仓盖开合的初始化函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SteeringEngineInit(void)
{
//	  Hero.MagClose = 2000;
//	  Hero.MagOpen = 800;
	  Hero.Pitch_init = GIM_PIT_INIT_SCALE;
	  Hero.Yaw_init  = GIM_YAW_INIT_SCALE;
	  Hero.Solo_Yaw_init=5604;
}
/**********************************************************************************************************
*函 数 名: Offline_Check_task
*功能说明: 掉线检测任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void Offline_Check_task(void *pvParameters)
{
	//在这个任务里不断的给掉线赋值，然后再接受里面清除，
	//如果达到目标次数就说明没有进入相应执行函数
   while (1) {
    
//	/*遥控器掉线检测*/
//	if(RC_DisConnect>100)
//		RC_Rst();
	
	/*底盘掉线检测*/
//	if(F105_DisConect>150)
//		F105_Rst();
	
//	/*云台掉线检测*/
//	if(Gimbal_DisConnect > 100)
//		GIMBAL_Rst();
	
	RC_DisConnect++;
	F105_DisConect++;
	Gimbal_DisConnect++;
	IWDG_Feed();//喂狗，即清空看门狗计数器
		 
  vTaskDelay(5); 
		 

    }
}

/**********************************************************************************************************
*函 数 名: delay_ms
*功能说明: 毫秒延时函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void delay_ms(unsigned int t)
{
    int i;
    for( i=0; i<t; i++)
    {
        int a=10300;
        while(a--);
    }
}