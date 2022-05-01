#include "main.h"

/******************************* 全局变量声明 ************************************/
unsigned char usart2_rx_buff[USART2_RX_BUFF_SIZE];
unsigned char usart2_tx_buff[USART2_TX_BUFF_SIZE];
unsigned char GYRO0_buffer[GYRO_BUF_SIZE];

/**
  * @brief  串口2配置，与PC通信
  * @param  None
  * @retval None
  */
void USART2_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(USART2_RX_RCC_GPIOx,ENABLE);

#if USART2_CONFIG_TX
    RCC_AHB1PeriphClockCmd(USART2_TX_RCC_GPIOx,ENABLE);
#endif

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

#if USART2_CONFIG_TX
    GPIO_PinAFConfig(USART2_TX_GPIOx,USART2_TX_GPIO_PINSOURCEx,GPIO_AF_USART2);
#endif
    GPIO_PinAFConfig(USART2_RX_GPIOx,USART2_RX_GPIO_PINSOURCEx,GPIO_AF_USART2);

    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
#if USART2_CONFIG_TX
    gpio.GPIO_Pin = USART2_RX_GPIO_PINx | USART2_TX_GPIO_PINx;
    GPIO_Init(USART2_RX_GPIOx,&gpio);
    //GPIO_Init(USART2_TX_GPIOx,&gpio);
#else
    gpio.GPIO_Pin = USART2_RX_GPIO_PINx;
    GPIO_Init(USART2_RX_GPIOx,&gpio);
#endif

#if USART2_CONFIG_CTR
    RCC_AHB1PeriphClockCmd(USART2_CTR_RCC_GPIOx,ENABLE);
    gpio.GPIO_Pin = USART2_CTR_GPIO_PINx;           //使能IO口
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(USART2_CTR_GPIOx,&gpio);
    GPIO_SetBits(USART2_CTR_GPIOx,USART2_CTR_GPIO_PINx);
#endif

    usart.USART_BaudRate = USART2_BAUDRATE;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
#if	USART2_CONFIG_TX
    usart.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
#else
    usart.USART_Mode =USART_Mode_Rx;
#endif
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2,&usart);

//		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
//	  USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
        DMA_DeInit(DMA1_Stream5);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t) usart2_rx_buff;
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dma.DMA_BufferSize = 8;
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
        DMA_Init(DMA1_Stream5,&dma);

        DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);

        DMA_Cmd(DMA1_Stream5,ENABLE);
    }
    nvic.NVIC_IRQChannel =DMA1_Stream5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    USART_Cmd(USART2,ENABLE);

#if		USART2_CONFIG_TX

    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);

//    nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;//USART2_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 1;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&nvic);
    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
        DMA_DeInit(DMA1_Stream6);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)usart2_tx_buff;
        dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
        dma.DMA_BufferSize = BUFFER_SIZEMAX;
        dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
        dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
        dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        dma.DMA_Mode =  DMA_Mode_Normal;
        dma.DMA_Priority = DMA_Priority_VeryHigh;
        dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
        dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
        dma.DMA_MemoryBurst = DMA_Mode_Normal;
        dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);
        DMA_Init(DMA1_Stream6,&dma);
        DMA_Cmd(DMA1_Stream6,DISABLE);

        nvic.NVIC_IRQChannel = DMA1_Stream6_IRQn;
        nvic.NVIC_IRQChannelPreemptionPriority = 3;
        nvic.NVIC_IRQChannelSubPriority = 1;
        nvic.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&nvic);

    }
#endif
}

/**
  * @brief  串口2
  * @param  None
  * @retval None
  */

void DMA1_Stream6_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
    {
        DMA_Cmd(DMA1_Stream6,DISABLE);
    }
    DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);
    DMA_ClearITPendingBit(DMA1_Stream6, DMA_IT_TCIF6);
}
/**
  * @brief  串口2接收中断
  * @param  None
  * @retval None
  */
int data;
u8 u2_rx[8];
int u2_rx_cnt=0;
void USART2_IRQHandler(void)
{
//	 static int Rx2_DATA_LENGTH;
//	    short n,i;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) !=RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        u8 _data = USART_ReceiveData(USART2);
        u2_rx[LIMIT_MAX_MIN(u2_rx_cnt,8,0)] = _data;
        u2_rx_cnt ++;
    }
    if (USART_GetITStatus(USART2, USART_IT_IDLE) !=RESET)
    {
        (void)USART2->SR;
        (void)USART2->DR;
        u2_rx_cnt = 0;
        PCReceive(u2_rx);
    }
}
void DMA1_Stream5_IRQHandler(void)
{
    static unsigned char temptemp[2*8];
    unsigned char tempPC[8];
    short PackPoint,n;
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
    {
        memcpy(temptemp+8,usart2_rx_buff,8);
        for(PackPoint = 0; PackPoint < 8; PackPoint++)//防止错位，不一定数组元素的第一个就为
        {
            if(temptemp[PackPoint] == '!')
            {
                for(n=0; n<8; n++)
                {
                    tempPC[n] = temptemp[(n+PackPoint)];
                }
                if(Verify_CRC8_Check_Sum(tempPC+1,8-2))
                {
                    PCReceive(tempPC);
                }
                break;
            }
        }
        DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
        memcpy(temptemp,temptemp+8,8);
    }
}


