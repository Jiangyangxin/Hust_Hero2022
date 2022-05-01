#include "main.h"

/********************************** 内核对象句柄 *********************************/
extern unsigned char usart2_tx_buff[USART2_TX_BUFF_SIZE];
extern Gyro_t Gyro;
/******************************* 全局变量声明 ************************************/
PC_Recv_t PC_Recv= {0,0,1,0,0};
PC_Send_t PC_Send;



/**
  * @brief  PC数据接收
  * @param
  * @retval
  */
float LastRecv[2]= {0};
float TempRecv[2]= {0};
float PitchRecvLog[110]= {0};
short PC_Cnt = 0;

void PCReceive(unsigned char PCReceivebuffer[])
{
    if((PCReceivebuffer[1]&0xF)!=0x1)
    {
        //通过串口发送扩大100倍
        PC_Recv.RCPitch = (float)((short)((PCReceivebuffer[2]<<8)|(PCReceivebuffer[3])))/100.0f;
        PC_Recv.RCYaw = -(float)((short)((PCReceivebuffer[4]<<8)|(PCReceivebuffer[5])))/100.0f;

        LastRecv[0]=PC_Recv.RCPitch;
        LastRecv[1]=PC_Recv.RCYaw;
        PC_Recv.PCDisconnect=0;
        PitchRecvLog[PC_Cnt] = PC_Recv.RCPitch;
        if(PC_Cnt > 99)
        {
            PC_Cnt = 0;
            memset(&PitchRecvLog,0,100);
        }
        PC_Cnt++;
        PC_Recv.Recvd++;
    }
}


/**
  * @brief  PC数据发送
	* @param   PC_Send.Type：0x10 大辅瞄；0x50：小辅瞄；0x30：PC关机
  * @retval
  */
short MiniPC_SendCnt=0;
float TempPit=0;
float TempYaw=0;
extern float now_yaw;
extern float now_pitch;
void MiniPC_SendThread(void)
{

    short floattoshort,PitchOutPut;
    PitchOutPut = (short)ZeroCheck_PitchOutPut();
    {
        MiniPC_SendCnt=0;

        usart2_tx_buff[0]='!';
        usart2_tx_buff[1]=PC_Send.Type;//0x10辅瞄


        TempPit=(((PitchOutPut-GIM_PIT_INIT_SCALE)%8192)/8192.0)*360;
        now_pitch =-TempPit;

        floattoshort=(short)(-TempPit*100.0f);
        usart2_tx_buff[2] = (unsigned char)((floattoshort>>8) & 0xff);
        usart2_tx_buff[3] = (unsigned char)(floattoshort & 0xff);


        TempYaw = Gyro.Yaw;
        while (TempYaw > 180)
        {
            TempYaw = TempYaw-360;
        }
        while (TempYaw < -180)
        {
            TempYaw = TempYaw + 360;
        }

        now_yaw = TempYaw;
        floattoshort=(short)(TempYaw*100.0f);
        usart2_tx_buff[4] =(unsigned char)((floattoshort>>8) & 0xff);
        usart2_tx_buff[5] = (unsigned char)(floattoshort & 0xff);
        Append_CRC8_Check_Sum(&usart2_tx_buff[1],6);
        usart2_tx_buff[7] = '#';
    }
    DMA_Cmd(DMA1_Stream6,ENABLE);
}


