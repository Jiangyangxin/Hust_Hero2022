/**********************************************************************************************************
 * @�ļ�     DataReceive.c
 * @˵��     ���պ���
 * @�汾  	 V1.0
 * @����     ��־��
 * @����     2020.1
**********************************************************************************************************/
#include "main.h"
/**********************************************************************************************************
*�� �� ��: PCReceive
*����˵��: USART6��PCͨ�Ž������ݴ���
*��    ��: rx_buffer[]
*�� �� ֵ: ��
**********************************************************************************************************/
int PCReceive_Tick;
extern Trajector hero_traj; //��������
short pc_pitch;
int pc_yaw;
short distance;
float aim_yaw, aim_pitch;
extern short KalMan_doneflag;
PC_Receive_t PC_Receive;
float PitchMotorReceive,YawMotorReceive;//Pitch,Yaw����Ƕ�
int PC_RX_num=0;	
KalmanFilter_t pitch_Kalman, yaw_Kalman;
void PCReceive(unsigned char PCReceivebuffer[])
{
	  PC_RX_num=!PC_RX_num;
    pc_pitch = (short)(PCReceivebuffer[2]<<8|PCReceivebuffer[3]);//���ﲻ��תΪfloat����Ȼ�������������Ϊ����
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







