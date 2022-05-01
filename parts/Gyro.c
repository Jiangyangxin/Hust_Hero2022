#include "main.h"

/*--------------------------引用外部变量----------------------------*/
extern short Gyro_DisConnect;
/**
  * @brief 新陀螺仪
  * @param   新陀螺仪不需要过零检测
  * @retval
  */
Gyro_t Gyro= {0,0,0,0,0,0,1};
//GY=0；GZ=0；Pitch=0；Yaw=0;recvd=0;DisconnectCnt=1;

//可能更为简洁，但是没有那种滤波的感觉
//void NewGyro_DataHandle(CanRxMsg rx_message1){
//	switch(rx_message1.StdId){
//		case 0x100:{
//			memcpy(&Gyro.Pitch,&rx_message1.Data,4);
//			memcpy(&Gyro.GY,&rx_message1.Data[4],4);
//			
//		}
//		break;
//		case 0x101:{
//			memcpy(&Gyro.Yaw,&rx_message1.Data,4);
//			memcpy(&Gyro.GZ,&rx_message1.Data[4],4);
//			while(Gyro.Yaw>180)
//				Gyro.Yaw -= 360 ;
//			while(Gyro.Yaw < -180)
//				Gyro.Yaw += 360;
//		}
//		break;
//	}
//}


float pitch[2],yaw[2],dPitch=0,dYaw=0,gy=0,gz=0;
void NewGyro_DataHandle(CanRxMsg rx_message1)
{
    //Gyro_DisConnect = 0;
    switch(rx_message1.StdId)
    {
    case 0x100://PITCH电机ID
    {
        if(Gyro.recvd[0]>5)//接受次数大于5
        {
            pitch[last]=pitch[now];
            memcpy(&pitch[now], &rx_message1.Data, 4);
            memcpy(&gy, &rx_message1.Data[4], 8);

            Gyro.GY=gy;
            dPitch=pitch[now]-pitch[last];
            Gyro.Pitch+=-dPitch;
        }
        else
        {
            Gyro.recvd[0]++;
            //memcpy(&pitch[now], rx_message1.Data, 4);//此处源代码有问题，江进行修改
					memcpy(&pitch[now], &rx_message1.Data, 4);
        }
    }
    break;
    case 0x101:
    {
        if(Gyro.recvd[1]>5)//接受次数大于5
        {
            yaw[last]=yaw[now];
            memcpy(&yaw[now], &rx_message1.Data, 4);
            memcpy(&gz, &rx_message1.Data[4], 8);

            Gyro.GZ=gz;
            dYaw=yaw[last]-yaw[now];
            Gyro.Yaw+=-dYaw;
        }
        else
        {
            Gyro.recvd[1]++;
            memcpy(&yaw[now], &rx_message1.Data, 4);
        }
    }
    }
}

