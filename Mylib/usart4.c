#include "main.h"
//unsigned char usart4_rx_buffer[30];
//unsigned char usart4_tx_buff[30];
///**********************************************************************************************************
//*函 数 名: USART4_Configuration
//*功能说明: usart4初始化，接收裁判系统
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//void USART4_Configuration(void)
//{
//    USART_InitTypeDef uart4;
//    GPIO_InitTypeDef  gpio;
//    NVIC_InitTypeDef  nvic;

//    RCC_AHB1PeriphClockCmd(USART4_RX_RCC_GPIOx,ENABLE);
//    RCC_AHB1PeriphClockCmd(USART4_TX_RCC_GPIOx,ENABLE);
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

//    GPIO_PinAFConfig(USART4_RX_GPIOx,USART4_RX_GPIO_PINSOURCEx,GPIO_AF_UART4);
//    GPIO_PinAFConfig(USART4_TX_GPIOx,USART4_TX_GPIO_PINSOURCEx,GPIO_AF_UART4);

//    gpio.GPIO_Pin = USART4_RX_GPIO_PINx | USART4_TX_GPIO_PINx;
//    gpio.GPIO_Mode = GPIO_Mode_AF;
//    gpio.GPIO_OType = GPIO_OType_PP;
//    gpio.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(USART4_RX_GPIOx,&gpio);

//    uart4.USART_BaudRate = USART4_BAUDRATE;
//    uart4.USART_WordLength = USART_WordLength_8b;
//    uart4.USART_StopBits = USART_StopBits_1;
//    uart4.USART_Parity = USART_Parity_No;
//    uart4.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
//    uart4.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_Init(UART4,&uart4);

//    nvic.NVIC_IRQChannel = DMA1_Stream2_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 3;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);
//    USART_Cmd(UART4,ENABLE);


//    USART_DMACmd(UART4,USART_DMAReq_Rx,ENABLE);
//    {
//        DMA_InitTypeDef   dma;
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
//        DMA_DeInit(DMA1_Stream2);
//        dma.DMA_Channel= DMA_Channel_4;
//        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
//        dma.DMA_Memory0BaseAddr = (uint32_t) usart4_rx_buffer;
//        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
//        dma.DMA_BufferSize = 30;
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
//        DMA_Init(DMA1_Stream2,&dma);
//        DMA_ITConfig(DMA1_Stream2,DMA_IT_TC,ENABLE);
//        DMA_Cmd(DMA1_Stream2,ENABLE);
//    }

//    USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
//#if		USART4_CONFIG_TX

//    USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
//    nvic.NVIC_IRQChannel = DMA1_Stream4_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 3;
//    nvic.NVIC_IRQChannelSubPriority = 3;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);
//    {
//        DMA_InitTypeDef   dma;
//        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
//        DMA_DeInit(DMA1_Stream4);
//        dma.DMA_Channel= DMA_Channel_4;
//        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART4->DR);
//        dma.DMA_Memory0BaseAddr = (uint32_t)usart4_tx_buff;
//        dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//        dma.DMA_BufferSize = USART4_TX_BUFF_SIZE;
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
//        DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);
//        DMA_Init(DMA1_Stream4,&dma);
//        DMA_Cmd(DMA1_Stream4,DISABLE);
//    }
//#endif
//}

///**********************************************************************************************************
//*函 数 名: DMA1_Stream4_IRQHandler
//*功能说明: 发送完成中断
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//void DMA1_Stream4_IRQHandler()
//{
//    if(DMA_GetITStatus(DMA1_Stream4, DMA_IT_TCIF4))
//    {
//        DMA_Cmd(DMA1_Stream4,DISABLE);
//    }
//    DMA_ClearFlag(DMA1_Stream4, DMA_FLAG_TCIF4);
//    DMA_ClearITPendingBit(DMA1_Stream4, DMA_IT_TCIF4);
//}

///**********************************************************************************************************
//*函 数 名: DMA1_Stream4_IRQHandler
//*功能说明: 接收完成中断
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//void DMA1_Stream2_IRQHandler(void)
//{
//    if(DMA_GetITStatus(DMA1_Stream2, DMA_IT_TCIF0))
//    {
//        DMA_ClearFlag(DMA1_Stream2, DMA_FLAG_TCIF0);
//        DMA_ClearITPendingBit(DMA1_Stream2, DMA_IT_TCIF0);
////        JugementRecv(usart4_rx_buffer);
//    }
//}

