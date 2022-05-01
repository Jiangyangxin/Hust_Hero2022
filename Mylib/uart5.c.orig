#include "main.h"

/******************************* 全局变量声明 ************************************/
unsigned char usart5_tx_buff[70];
unsigned char usart5_rx_buff[30];

/**
  * @brief  串口5配置，裁判系统数据接收
  * @param  None
  * @retval None
  */
void UART5_Configuration(void)
{
    USART_InitTypeDef usart5;
		GPIO_InitTypeDef  gpio;
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
 //   USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
		
		
		
    	
    nvic.NVIC_IRQChannel =DMA1_Stream0_IRQn; //UART5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);	
		
		USART_DMACmd(UART5,USART_DMAReq_Rx,ENABLE);
		{
			  DMA_InitTypeDef   dma;
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
        DMA_DeInit(DMA1_Stream0);
        dma.DMA_Channel= DMA_Channel_4;
        dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
        dma.DMA_Memory0BaseAddr = (uint32_t)usart5_rx_buff;
        dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
        dma.DMA_BufferSize = 30;
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
    nvic.NVIC_IRQChannelPreemptionPriority = 3;
    nvic.NVIC_IRQChannelSubPriority = 3;
    nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
		{
			DMA_InitTypeDef   dma;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
			DMA_DeInit(DMA1_Stream7);
			dma.DMA_Channel= DMA_Channel_4;
			dma.DMA_PeripheralBaseAddr = (uint32_t)&(UART5->DR);
			dma.DMA_Memory0BaseAddr = (uint32_t)usart5_tx_buff;
			dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
			dma.DMA_BufferSize = 70;
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

/**
  * @brief  裁判系统DMA中断接收
  * @param  None
  * @retval None低位在前
  */

void DMA1_Stream0_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream0, DMA_IT_TCIF0))
	{
		DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF0);
		DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
		JugementRecv(usart5_rx_buff);
	}
}

