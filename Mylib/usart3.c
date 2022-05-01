#include "main.h"

/******************************* 全局变量声明 ************************************/
volatile unsigned char usart3_rx_buffer[18];


/**********************************************************************************************************
*函 数 名: USART3_Configuration
*功能说明: usart3初始化，与遥控器通信
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void USART3_Configuration(void)
{
    USART_InitTypeDef usart3;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); 
//    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
//    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

//    gpio.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_100MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_11;           //使能IO口
    gpio.GPIO_Mode = GPIO_Mode_AF;
	  gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd=GPIO_PuPd_NOPULL;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;

		GPIO_Init(GPIOB,&gpio);
//    GPIO_SetBits(GPIOB,GPIO_Pin_11);
    USART_DeInit(USART3);
    usart3.USART_BaudRate = 100000;
    usart3.USART_WordLength = USART_WordLength_8b;
    usart3.USART_StopBits = USART_StopBits_1;
    usart3.USART_Parity = USART_Parity_Even;
    usart3.USART_Mode = USART_Mode_Rx;
    usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3,&usart3);

    USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
    USART_Cmd(USART3,ENABLE);
    USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);

    nvic.NVIC_IRQChannel = USART3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;//1
    nvic.NVIC_IRQChannelSubPriority = 2;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
        
			 DMA_DeInit(DMA1_Stream1);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)usart3_rx_buffer;
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dma.DMA_BufferSize = BUFFER_SIZEMAX;//18u;
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
        DMA_Init(DMA1_Stream1,&dma);
        DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA1_Stream1,ENABLE);
    }
}

/**********************************************************************************************************
*函 数 名: USART3_IRQHandler
*功能说明: usart3接收中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void USART3_IRQHandler(void)
{
    static int DATA_LENGTH=0;
    if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        (void)USART3->SR;
        (void)USART3->DR;
        DMA_Cmd(DMA1_Stream1,DISABLE);
        DATA_LENGTH=BUFFER_SIZEMAX-DMA_GetCurrDataCounter(DMA1_Stream1);
        if(DATA_LENGTH==18)
        {
        					RemoteReceive(usart3_rx_buffer);
        }
        DMA_SetCurrDataCounter(DMA1_Stream1,BUFFER_SIZEMAX);
        DMA_Cmd(DMA1_Stream1,ENABLE);
    }
}


