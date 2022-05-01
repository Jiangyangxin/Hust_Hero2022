#include "main.h"

/*--------------------------�����ⲿ����----------------------------*/
extern short Gyro_DisConnect;
/**
  * @brief ��������
  * @param   �������ǲ���Ҫ������
  * @retval
  */
Gyro_t Gyro= {0,0,0,0,0,0,1};
//GY=0��GZ=0��Pitch=0��Yaw=0;recvd=0;DisconnectCnt=1;

//���ܸ�Ϊ��࣬����û�������˲��ĸо�
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
    case 0x100://PITCH���ID
    {
        if(Gyro.recvd[0]>5)//���ܴ�������5
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
            //memcpy(&pitch[now], rx_message1.Data, 4);//�˴�Դ���������⣬�������޸�
					memcpy(&pitch[now], &rx_message1.Data, 4);
        }
    }
    break;
    case 0x101:
    {
        if(Gyro.recvd[1]>5)//���ܴ�������5
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
