#include "main.h"

/******************************* 全局变量声明 ************************************/
unsigned char usart1_rx_buffer[USART1_RX_BUFF_SIZE];


/**********************************************************************************************************
*函 数 名: USART1_Configuration
*功能说明: usart1初始化，与遥控器通信
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void USART1_Configuration(void)
{
    USART_InitTypeDef usart1;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);

    gpio.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_8;           //使能IO口
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA,&gpio);

    GPIO_SetBits(GPIOA,GPIO_Pin_8);

    usart1.USART_BaudRate = 100000;
    usart1.USART_WordLength = USART_WordLength_8b;
    usart1.USART_StopBits = USART_StopBits_1;
    usart1.USART_Parity = USART_Parity_No;
    usart1.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
    usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&usart1);

    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
    USART_Cmd(USART1,ENABLE);
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);

    nvic.NVIC_IRQChannel = USART1_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;//1
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    {
        DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
        DMA_DeInit(DMA2_Stream5);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)usart1_rx_buffer;
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dma.DMA_BufferSize = BUFFER_SIZEMAX;
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
        DMA_Init(DMA2_Stream5,&dma);
        DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
        DMA_Cmd(DMA2_Stream5,ENABLE);
    }
}

/**********************************************************************************************************
*函 数 名: USART1_IRQHandler
*功能说明: usart1接收中断
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/

void USART1_IRQHandler(void)
{
    static int DATA_LENGTH=0;
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        (void)USART1->SR;
        (void)USART1->DR;
        DMA_Cmd(DMA2_Stream5,DISABLE);
        DATA_LENGTH=BUFFER_SIZEMAX-DMA_GetCurrDataCounter(DMA2_Stream5);
        if(DATA_LENGTH==USART1_RX_BUFF_SIZE)
        {
            RemoteReceive(usart1_rx_buffer);
        }
        DMA_SetCurrDataCounter(DMA2_Stream5,BUFFER_SIZEMAX);
        DMA_Cmd(DMA2_Stream5,ENABLE);
    }
}


