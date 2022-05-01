#include "main.h"

/*--------------------------全局变量----------------------------*/

/*--------------------------引用外部变量----------------------------*/
extern short BigFrictAble;
extern Pid_Typedef	PidYawPos,PidPitchPos;
extern unsigned char ShootStatus;
F405_typedef F405;
/**
  * @brief  CAN1数据发送
  * @param
  * @retval
  */
	
	//摩擦轮数据发送电流
void CAN1_FrictSend(short a,short b,short c,short d)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    tx_message.StdId = 0x200;

    if(BigFrictAble==0){     
		a=LIMIT_MAX_MIN(a,16000,-16000);
    b=LIMIT_MAX_MIN(b,16000,-16000);
    c=LIMIT_MAX_MIN(c,16000,-16000);
    d=LIMIT_MAX_MIN(d,16000,-16000);
    tx_message.Data[0] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(a&0xff);
    tx_message.Data[2] = (unsigned char)((b>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(b&0xff);
    tx_message.Data[4] = (unsigned char)((c>>8)&0xff);
    tx_message.Data[5] = (unsigned char)(c&0xff);
    tx_message.Data[6] = (unsigned char)((d>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(d&0xff);
		CAN_Transmit(CAN1,&tx_message);
    }
		else
	{
		a=0;
    b=0;
    c=0;
    d=0;
    tx_message.Data[0] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(a&0xff);
    tx_message.Data[2] = (unsigned char)((b>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(b&0xff);
    tx_message.Data[4] = (unsigned char)((c>>8)&0xff);
    tx_message.Data[5] = (unsigned char)(c&0xff);
    tx_message.Data[6] = (unsigned char)((d>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(d&0xff);
	  CAN_Transmit(CAN1,&tx_message);
		}
}
   //大拨盘数据发送电流
void CAN2_BigPullerSend(short a)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    tx_message.StdId = 0x200;
    a=LIMIT_MAX_MIN(a,16000,-16000);
	short q =0,b=0,c=0;
	  tx_message.Data[0] = (unsigned char)((q>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(q&0xff);
    tx_message.Data[2] = (unsigned char)((b>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(b&0xff);
    tx_message.Data[4] = (unsigned char)((c>>8)&0xff);
    tx_message.Data[5] = (unsigned char)(c&0xff);
    tx_message.Data[6] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(a&0xff);
    CAN_Transmit(CAN2,&tx_message);
}

/**
  * @brief  底盘速度分量发送
  * @param
  * @retval
  */
extern HeroStatus_typedef	HeroStatus;
int tick;
void CAN1_ChassisSend(Chassis_typedef* Chassis)//(&F105,0)
{

    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    tx_message.StdId = 0x100;
//	  Chassis->carSpeedx*=500;
//	  Chassis->carSpeedy*=500;
//	  Chassis->carSpeedw*=20;
	  tx_message.Data[0]=(unsigned char)( ((short)Chassis->carSpeedy>>8)&0xff);
    tx_message.Data[1]=(unsigned char)( (short)Chassis->carSpeedy&0xff);
    tx_message.Data[2]=(unsigned char)( ((short)-Chassis->carSpeedx>>8)&0xff);
    tx_message.Data[3]=(unsigned char)( (short)-Chassis->carSpeedx&0xff);
    tx_message.Data[4]=(unsigned char)( ((short)Chassis->carSpeedw>>8)&0xff);
    tx_message.Data[5]=(unsigned char)( (short)Chassis->carSpeedw&0xff);
    tx_message.Data[6]=(unsigned char) ((short)HeroStatus.ChassisMode);                                 
    tx_message.Data[7]= (unsigned char)	(Chassis->SuperCapCmd<<1|Chassis->PowerdownCmd);
    CAN_Transmit(CAN1,&tx_message); 
	
//    tx_message.IDE = CAN_ID_STD;
//    tx_message.RTR = CAN_RTR_DATA;
//    tx_message.DLC = 0x08;
//    tx_message.StdId = 0x121;
//	  tx_message.Data[0]=ShootStatus;
//                           
//   
//    CAN_Transmit(CAN1,&tx_message); 
//		
		tick++;
		if(tick%100==0)			
{	
		tx_message.IDE = CAN_ID_STD;
		tx_message.RTR = CAN_RTR_DATA;
		tx_message.DLC = 0x08;
		tx_message.StdId = 0x121;				
//				tx_message.Data[0]=ShootStatus;
	F405.Send_Pack1  = ((F405.Frict_state&0x01)<<0)|
				((F405.Laser_Flag&0x01)<<1)|
				((F405.Graphic_Init_Flag&0x01)<<2)|
				((F405.Follow_state&0x01)<<3);
		memcpy(&tx_message.Data[0],&F405.SuperPowerLimit,1);
		memcpy(&tx_message.Data[1],&F405.Chassis_Flag,1);	
		memcpy(&tx_message.Data[2],&F405.Gimbal_100,2);
		memcpy(&tx_message.Data[4],&F405.Gimbal_Flag,1);
		memcpy(&tx_message.Data[5],&F405.Send_Pack1,1);
										 
		tick=0;
		CAN_Transmit(CAN1,&tx_message); 
}
}
/**
  * @brief   云台电流发送
  * @param
  * @retval
  */
void CAN2_GimbalSend(short pitch,short yaw)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;
    tx_message.RTR = CAN_RTR_DATA;
    tx_message.DLC = 0x08;
    tx_message.StdId = 0x1FF;
    pitch	=	LIMIT_MAX_MIN(pitch,30000,-30000);
    yaw		=	LIMIT_MAX_MIN(yaw,30000,-30000);
    //USART4SEND(PidYawPos.ActPoint,PidYawPos.SetPoint,PidPitchPos.ActPoint,PidPitchPos.SetPoint,(float)(yaw*10),(float)(pitch*10));
    tx_message.Data[0] = (unsigned char)((pitch>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(pitch&0xff);
    tx_message.Data[2] = (unsigned char)((yaw>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(yaw&0xff);
    CAN_Transmit(CAN2,&tx_message);
	
	
}



