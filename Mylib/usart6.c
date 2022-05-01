/**********************************************************************************************************
 * @文件     usart6.c
 * @说明     usart6初始化：PC通信
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
 

*/   
/*--------------------------全局变量----------------------------*/

/*--------------------------引用外部变量----------------------------*/

/*--------------------------内部变量----------------------------*/

#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
/*--------------------------全局变量----------------------------*/

/*--------------------------引用外部变量----------------------------*/
//extern Gyro_t Gyro;
extern xTaskHandle TX2Task_Handler;
///*--------------------------内部变量----------------------------*/

////static short PC_Flag=0;
////static unsigned char SendToPC_Buff[ PC_RECVBUF_SIZE];
////static unsigned char PCbuffer[PC_RECVBUF_SIZE]= {0,0,0};


///**********************************************************************************************************
//*函 数 名: USART6_Configuration
//*功能说明: usart6配置函数(PC通信)
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//void USART6_Configuration(void)
//{
//    USART_InitTypeDef usart6;
//    GPIO_InitTypeDef  gpio;
//    NVIC_InitTypeDef  nvic;

////触发脉冲脚PA1
//	  
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//AHB1
//	  gpio.GPIO_Pin = GPIO_Pin_1;
//    gpio.GPIO_Mode = GPIO_Mode_OUT;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_Init(GPIOA,&gpio);

//  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

//    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
//    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);

//    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOC,&gpio);

//    usart6.USART_BaudRate = 115200;
//    usart6.USART_WordLength = USART_WordLength_8b;
//    usart6.USART_StopBits = USART_StopBits_1;
//    usart6.USART_Parity = USART_Parity_No;
//    usart6.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
//    usart6.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(USART6,&usart6);

//    USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);
//    USART_Cmd(USART6,ENABLE);

//    nvic.NVIC_IRQChannel = USART6_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 2;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);

//    USART_Cmd(USART6,ENABLE);
//    USART_DMACmd(USART6,USART_DMAReq_Rx,ENABLE);
//    USART_DMACmd(USART6,USART_DMAReq_Tx,ENABLE);

//    nvic.NVIC_IRQChannel = DMA2_Stream2_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);

//    {
//        DMA_InitTypeDef   dma;
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//        DMA_DeInit(DMA2_Stream2);
//        dma.DMA_Channel= DMA_Channel_5;
//        dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
////        dma.DMA_Memory0BaseAddr = (uint32_t)PCbuffer;
//        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//        dma.DMA_BufferSize = PC_RECVBUF_SIZE;
//        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        dma.DMA_Mode = DMA_Mode_Circular;
//        dma.DMA_Priority = DMA_Priority_VeryHigh;
//        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//        dma.DMA_MemoryBurst = DMA_Mode_Normal;
//        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//        DMA_Init(DMA2_Stream2,&dma);
//        DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
//        DMA_Cmd(DMA2_Stream2,ENABLE);
//    }

//    {
//        DMA_InitTypeDef   dma;
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
//        DMA_DeInit(DMA2_Stream7);
//        dma.DMA_Channel= DMA_Channel_5;
//        dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART6->DR);
////        dma.DMA_Memory0BaseAddr = (uint32_t)SendToPC_Buff;
//        dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//        dma.DMA_BufferSize = PC_SENDBUF_SIZE;
//        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
//        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//        dma.DMA_Mode = DMA_Mode_Circular;
//        dma.DMA_Priority = DMA_Priority_VeryHigh;
//        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
//        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//        dma.DMA_MemoryBurst = DMA_Mode_Normal;
//        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//        DMA_Init(DMA2_Stream7,&dma);
//        DMA_Cmd(DMA2_Stream7,DISABLE);
//    }
//}


//void USART6_IRQHandler(void)//不加串口闲时中断清除标志位会导致程序卡死在HardFault_Handler里
//{
//    if (USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)
//    {
//        (void)USART6->SR;
//        (void)USART6->DR;
//    }
//}
///**********************************************************************************************************
//*函 数 名: DMA2_Stream2_IRQHandler
//*功能说明: usart6 DMA接收中断
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//unsigned char tempPC[PC_RECVBUF_SIZE];//这里要改为全局变量，不然crc不通过
//short Crcpass,crcNopass;
//extern PC_Receive_t PC_Receive;
//unsigned char ErrorBuff[PC_RECVBUF_SIZE*4];
//short buffindex;

///**********************************************普通接收****************************************************************/
//void DMA2_Stream2_IRQHandler(void)
//{
//    static unsigned char temptemp[2*PC_RECVBUF_SIZE];
//    short PackPoint,n;
//    if(DMA_GetITStatus(DMA2_Stream2, DMA_IT_TCIF2))
//    {
//        memcpy(temptemp+PC_RECVBUF_SIZE,PCbuffer,PC_RECVBUF_SIZE);
//        for(PackPoint = 0; PackPoint < PC_RECVBUF_SIZE; PackPoint++)//防止错位，不一定数组元素的第一个就为
//        {
//            if(temptemp[PackPoint] == '!')
//            {
//                for(n=0; n<PC_RECVBUF_SIZE; n++)
//                {
//                    tempPC[n] = temptemp[(n+PackPoint)];
//                }
//                crcNopass++;
//                if(Verify_CRC8_Check_Sum(tempPC+1,PC_RECVBUF_SIZE-2))
//                {

//                    PCReceive(tempPC);//辅瞄
//                }
//                else
//                {
//                    buffindex++;
//                    buffindex = buffindex%4;
//                }
//                break;
//            }
//        }
//        DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
//        DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
//        memcpy(temptemp,temptemp+PC_RECVBUF_SIZE,PC_RECVBUF_SIZE);
//    }
//}



///**********************************************************************************************************
//*函 数 名: USART6_SendtoPC
//*功能说明: 弹丸数和云台姿态交替发送,发送姿态频率是子弹数的4倍
//					 PC_Sendflag 0x00待机·   0x10辅瞄   0x20能量机关   0x30 Tx2关机
//					 这些协议需要重新定一下
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//int PC_TX_num;			//和Pc之间通信的计数器，用于降低通讯帧率
//short down_sampling_rate = 6;
//short my_pitch, my_yaw;
//void USART6_SendtoPC(void)
//{
//   if(PC_TX_num % down_sampling_rate == 0)	//每down_sampling_rate发一次数据，并做同步
//	{
//		PC_TX_num = 0;
//		SendToPC_Buff[0] = '!';
//		
//		SendToPC_Buff[1] = 0x05;
//		my_pitch = (short)(AbsolutePitch()*50);
//		my_yaw = (short)(Gyro.Yaw*50);

//		SendToPC_Buff[2] = (unsigned char)((my_pitch >> 8) & 0x00FF);
//		SendToPC_Buff[3] = (unsigned char)((my_pitch) & 0x00FF);

//		SendToPC_Buff[4] = (unsigned char)((my_yaw >> 8) & 0x00FF);
//		SendToPC_Buff[5] = (unsigned char)((my_yaw) & 0x00FF);
//		
//	//	Tx2_Off_CheckAndSet(SendToPC_Buff);
//		
//		SendToPC_Buff[7] = '#';
//		Append_CRC8_Check_Sum(SendToPC_Buff,8);

//		GPIO_SetBits(GPIOA,GPIO_Pin_1);
//		DMA_Cmd(DMA2_Stream7, ENABLE);
//	}
//	if((PC_TX_num % (down_sampling_rate / 2) == 0)&&(PC_TX_num % down_sampling_rate != 0))
//		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
//	PC_TX_num++;
//}


///**********************************************************************************************************
//*函 数 名: Tx2_Off_CheckAndSet
//*功能说明: 发送指令给tx2使其关机
//*形    参: 指向发送数据的指针
//*返 回 值: 无
//**********************************************************************************************************/
//void Tx2_Off_CheckAndSet(u8* Buff)
//{
////	int i;
////	if(PC_Sendflag == Tx2_Off)
////	for(i = 0;i < 7;i++)
////	{
////		*Buff = '!';
////		Buff++;
////	}
//}



