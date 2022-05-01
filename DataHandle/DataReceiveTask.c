/**********************************************************************************************************
 * @文件     DataReceive.c
 * @说明     接收函数
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*函 数 名: PCReceive
*功能说明: USART6与PC通信接收数据处理
*形    参: rx_buffer[]
*返 回 值: 无
**********************************************************************************************************/
int PCReceive_Tick;
extern Trajector hero_traj; //弹道方程
short pc_pitch;
int pc_yaw;
short distance;
float aim_yaw, aim_pitch;
extern short KalMan_doneflag;
PC_Receive_t PC_Receive;
float PitchMotorReceive,YawMotorReceive;//Pitch,Yaw电机角度
int PC_RX_num=0;	
KalmanFilter_t pitch_Kalman, yaw_Kalman;
void PCReceive(unsigned char PCReceivebuffer[])
{
	  PC_RX_num=!PC_RX_num;
    pc_pitch = (short)(PCReceivebuffer[2]<<8|PCReceivebuffer[3]);//这里不能转为float，不然负数传过来会变为正数
    pc_yaw = (int)(PCReceivebuffer[4]<<24|PCReceivebuffer[5]<<16|PCReceivebuffer[6]<<8|PCReceivebuffer[7]);
    distance = (unsigned char)PCReceivebuffer[8]/10;
    aim_yaw = (float)pc_yaw/100.0f;
    aim_pitch = (float)pc_pitch/100.0f;
//    if(KalMan_doneflag==1)
//    {
//      PC_Receive.PCPitch = KalmanFilter_Calc(&pitch_Kalman, aim_pitch);
//      PC_Receive.PCYaw = KalmanFilter_Calc(&yaw_Kalman, aim_yaw);
//    }
     
		PC_Receive.PCPitch =aim_pitch;
		PC_Receive.PCYaw = aim_yaw;
		
	
//    PC_Receive.PCdistance = (float)distance/10.0f;

    PC_Receive.DisConnect=0;
}







