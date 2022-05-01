#include "main.h"

/******************************* ȫ�ֱ������� ************************************/
M3508Receive_Typedef		BigFrict[2],BigPuller;
GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
short BigFrictAble=0;//������
/******************************* �����ⲿ���� ************************************/
short Gimbal_DisConnect;
extern Chassis_typedef F105;
/**
  * @brief  CAN1����
  * @param
  * @retval
  */
void CAN1_Receive0Task(CanRxMsg rx_message)
{
    switch(rx_message.StdId)
    {
    case 0x120:
        F105.bulletSpeedmax = (rx_message.Data[0])|(rx_message.Data[1]<<8);
			 	F105.IsShootAble= (rx_message.Data[2]);
		    F105.bulletSpeed= ((short)(rx_message.Data[3])|(rx_message.Data[4]<<8));
		    F105.bulletSpeed=F105.bulletSpeed/100.0f;
        break;
		
    case 0x202:      // Fric Right;
        BigFrict[1].Angle 			= (rx_message.Data[0]<<8) | rx_message.Data[1];
        BigFrict[1].RealSpeed 	= (rx_message.Data[2]<<8) | rx_message.Data[3];
        break;
    case 0x201:     //Frict  Left
        BigFrict[0].Angle 			= (rx_message.Data[0]<<8) | rx_message.Data[1];
        BigFrict[0].RealSpeed 	= (rx_message.Data[2]<<8) | rx_message.Data[3];
        break; 
      
    }
}
/**
  * @brief  CAN2����
  * @param
  * @retval
  */
void CAN2_Receive0Task(CanRxMsg rx_message)
{
    switch(rx_message.StdId)
    {

			case 0x004:
				BigFrictAble =  rx_message.Data[0];//0��ʾBooster��Ħ�����ϵ磬
				break;
		  case 0x206: 	       //GimYAW			 3
        Gimbal_Motor_Yaw.Angle				= 	(rx_message.Data[0]<<8) | rx_message.Data[1];
        Gimbal_Motor_Yaw.RealSpeed		=		(rx_message.Data[2]<<8) | rx_message.Data[3];
			  Gimbal_Motor_Yaw.Current      =   (rx_message.Data[4]<<8) | rx_message.Data[5];
        Gimbal_DisConnect = 0;  //���߱�־��0��ʾ������
        break;
      case 0x205:         //GimPitch				2
        Gimbal_Motor_Pitch.Angle				= 	(rx_message.Data[0]<<8) | rx_message.Data[1];
        Gimbal_Motor_Pitch.RealSpeed		=		(rx_message.Data[2]<<8) | rx_message.Data[3];
			  Gimbal_Motor_Pitch.Current      =   (rx_message.Data[4]<<8) | rx_message.Data[5];
        Gimbal_DisConnect = 0;  //���߱�־��0��ʾ������
        break;
	    case 0x204:					//����
        BigPuller.Angle 			= (rx_message.Data[0]<<8) | rx_message.Data[1];
        BigPuller.RealSpeed 	= (rx_message.Data[2]<<8) | rx_message.Data[3];

    }
}
