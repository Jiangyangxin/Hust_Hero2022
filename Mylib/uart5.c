#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
/******************************* 全局变量声明 ************************************/
unsigned char usart5_tx_buff[70];
unsigned char usart5_rx_buff[30];
/*--------------------------引用外部变量----------------------------*/

extern xTaskHandle TX2Task_Handler;
extern Gyro_t Gyro;
/*--------------------------内部变量----------------------------*/

static short PC_Flag=0;
static unsigned char SendToPC_Buff[ PC_SENDBUF_SIZE];
static unsigned char PCbuffer[PC_RECVBUF_SIZE]= {0,0,0};

/**
  * @brief  串口5配置，TX2接收
  * @param  None
  * @retval None
  */
void UART5_Configuration(void)
{
    
	//触发
	  GPIO_InitTypeDef gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//AHB1
	  gpio.GPIO_Pin = GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

  	USART_InitTypeDef usart5;

    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
    RCC_AHB1PeriphClockCmd(USART5_RX_RCC_GPIOx,ENABLE);
#if USART5_CONFIG_TX
    RCC_AHB1PeriphClockCmd(USART5_TX_GPIO_PINSOURCEx,ENABLE);
#endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);

    GPIO_PinAFConfig(USART5_RX_GPIOx,USART5_RX_GPIO_PINSOURCEx,GPIO_AF_UART5);
#if USART5_CONFIG_TX
    GPIO_PinAFConfig(USART5_TX_GPIOx,USART5_TX_GPIO_PINSOURCEx,GPIO_AF_UART5);
#endif

    gpio.GPIO_Pin = USART5_RX_GPIO_PINx;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(USART5_RX_GPIOx,&gpio);
#if USART5_CONFIG_TX
    gpio.GPIO_Pin = USART5_TX_GPIO_PINx;
    GPIO_Init(USART5_TX_GPIOx,&gpio);
#endif
//
#if USART5_CONFIG_CTR
    RCC_AHB1PeriphClockCmd(USART5_CTR_RCC_GPIOx,ENABLE);
    gpio.GPIO_Pin = USART5_CTR_GPIO_PINx;           //使能IO口
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(USART5_CTR_GPIOx,&gpio);
    GPIO_SetBits(USART5_CTR_GPIOx,USART5_CTR_GPIO_PINx);
#endif
//

    usart5.USART_BaudRate = USART5_BAUDRATE;
    usart5.USART_WordLength = USART_WordLength_8b;
    usart5.USART_StopBits = USART_StopBits_1;
    usart5.USART_Parity = USART_Parity_No;
    usart5.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    usart5.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5,&usart5);

    USART_Cmd(UART5,ENABLE);
   USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);




    nvic.NVIC_IRQChannel =DMA1_Stream0_IRQn; //UART5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);
    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Stream0);//stream0重设为缺省值
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);/* UART5接收数据地址 */
        dma.DMA_Memory0BaseAddr = (uint32_t)PCbuffer;//接收缓存区首地址
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;//数据存储方向
        dma.DMA_BufferSize = PC_RECVBUF_SIZE;
        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dma.DMA_Mode = DMA_Mode_Circular;
        dma.DMA_Priority = DMA_Priority_VeryHigh;
        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        dma.DMA_MemoryBurst = DMA_Mode_Normal;
        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_Init(DMA1_Stream0, &dma);

        DMA_ITConfig(DMA1_Stream0,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA1_Stream0, ENABLE);
    }


    USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);
    nvic.NVIC_IRQChannel = DMA1_Stream7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
        DMA_DeInit(DMA1_Stream7);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)SendToPC_Buff;
        dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
        dma.DMA_BufferSize = PC_SENDBUF_SIZE;
        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dma.DMA_Mode = DMA_Mode_Circular;
        dma.DMA_Priority = DMA_Priority_VeryHigh;
        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        dma.DMA_MemoryBurst = DMA_Mode_Normal;
        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_ITConfig(DMA1_Stream7,DMA_IT_TC,ENABLE);
        DMA_Init(DMA1_Stream7,&dma);
        DMA_Cmd(DMA1_Stream7,DISABLE);
    }
}

/***DMA 传输完成中断****/
void DMA1_Stream7_IRQHandler()
{
    if(DMA_GetITStatus(DMA1_Stream7, DMA_IT_TCIF7))
    {
        DMA_Cmd(DMA1_Stream7,DISABLE);
    }
    DMA_ClearFlag(DMA1_Stream7, DMA_FLAG_TCIF7);
    DMA_ClearITPendingBit(DMA1_Stream7, DMA_IT_TCIF7);
}

/**********************************************************************************************************
*函 数 名: USART5_SendtoPC
*功能说明: 弹丸数和云台姿态交替发送,发送姿态频率是子弹数的4倍
					 PC_Sendflag 0x00待机·   0x10辅瞄   0x20能量机关   0x30 Tx2关机
					 这些协议需要重新定一下
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int PC_TX_num;			//和Pc之间通信的计数器，用于降低通讯帧率
int down_sampling_rate = 6;
short my_pitch;
int my_yaw;
unsigned char ShootStatus=1;//1小0大
void USART5_SendtoPC(void)
{
   if(PC_TX_num % down_sampling_rate == 0)	//每down_sampling_rate发一次数据，并做同步
	{
		PC_TX_num = 0;
		SendToPC_Buff[0] = '!';
		
		SendToPC_Buff[1] = 0x05;
		my_pitch = (short)(AbsolutePitch()*100);
		my_yaw =  (int)(Gyro.Yaw*100);

		SendToPC_Buff[2] = (unsigned char)((my_pitch >> 8) & 0x00FF);
		SendToPC_Buff[3] = (unsigned char)((my_pitch) & 0x00FF);

		SendToPC_Buff[4] = (unsigned char)((my_yaw >> 24) & 0x00FF);
		SendToPC_Buff[5] = (unsigned char)((my_yaw >> 16) & 0x00FF);
		SendToPC_Buff[6] = (unsigned char)((my_yaw >> 8) & 0x00FF);
		SendToPC_Buff[7] = (unsigned char)((my_yaw) & 0x00FF);
		SendToPC_Buff[8] =  ShootStatus;
	//	Tx2_Off_CheckAndSet(SendToPC_Buff);
		
		SendToPC_Buff[9] = '#';
		Append_CRC8_Check_Sum(SendToPC_Buff,PC_SENDBUF_SIZE);

		GPIO_SetBits(GPIOA,GPIO_Pin_1);
		DMA_Cmd(DMA1_Stream7, ENABLE);
	}
	if((PC_TX_num % (down_sampling_rate / 2) == 0)&&(PC_TX_num % down_sampling_rate != 0))
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	PC_TX_num++;
}

/**
  * @brief  TX2DMA中断接收
  * @param  None
  * @retval None低位在前
  */
unsigned char tempPC[PC_RECVBUF_SIZE];//这里要改为全局变量，不然crc不通过
short Crcpass,crcNopass;
extern PC_Receive_t PC_Receive;
unsigned char ErrorBuff[PC_RECVBUF_SIZE*4];
short buffindex;
extern int PC_RX_num;
void DMA1_Stream0_IRQHandler(void)
{
     //PC_RX_num=0;
		 static unsigned char temptemp[2*PC_RECVBUF_SIZE];
    short PackPoint,n;
    if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0))
    {
        memcpy(temptemp+PC_RECVBUF_SIZE,PCbuffer,PC_RECVBUF_SIZE);
        for(PackPoint = 0; PackPoint < PC_RECVBUF_SIZE; PackPoint++)//防止错位，不一定数组元素的第一个就为
        {
            if(temptemp[PackPoint] == '!')
            {
                for(n=0; n<PC_RECVBUF_SIZE; n++)
                {
                    tempPC[n] = temptemp[(n+PackPoint)];
                }
                crcNopass++;
                if(Verify_CRC8_Check_Sum(tempPC,PC_RECVBUF_SIZE))
                {

                    PCReceive(tempPC);//辅瞄

                }
                else
                {
                    buffindex++;
                    buffindex = buffindex%4;
                }
                break;
            }
        }
        DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
        DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
        memcpy(temptemp,temptemp+PC_RECVBUF_SIZE,PC_RECVBUF_SIZE);
    }
}

/**********************************************************************************************************
*函 数 名: TX2_task
*功能说明: 通信任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint32_t TX2_high_water;
void TX2_task(void *pvParameters)
{

    while (1) {
        USART5_SendtoPC();

        vTaskDelay(1); 

    }
#if INCLUDE_uxTaskGetStackHighWaterMark
        TX2_high_water = uxTaskGetStackHighWaterMark(TX2Task_Handler);
#endif
}
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
