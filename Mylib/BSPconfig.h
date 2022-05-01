#ifndef _CONFIG_H
#define _CONFIG_H

/*****************CAN2***********************/
//ID
#define CAN2_Mode         CAN_Mode_Normal
/*²¨ÌØÂÊÅäÖÃ  CAN BaudRate  42/(sjw+bs1+bs2)/pre (Mbps)*/
#define CAN2_SJW    CAN_SJW_1tq
#define CAN2_BS1    CAN_BS1_9tq
#define CAN2_BS2    CAN_BS2_4tq
#define CAN2_PRE    3
//
#define CAN2_TX_RCC_GPIOx        RCC_AHB1Periph_GPIOB
#define CAN2_TX_GPIOx            GPIOB
#define CAN2_TX_GPIO_PINx        GPIO_Pin_6
#define CAN2_TX_GPIO_PINSOURCEx  GPIO_PinSource6

#define CAN2_RX_RCC_GPIOx          RCC_AHB1Periph_GPIOB
#define CAN2_RX_GPIOx              GPIOB
#define CAN2_RX_GPIO_PINx          GPIO_Pin_5
#define CAN2_RX_GPIO_PINSOURCEx    GPIO_PinSource5
//
/*****************CAN1***********************/
//ID
#define CAN1_RX_Filter_ID 0x201
#define CAN1_TX_Filter_ID 0x201
#define CAN1_Mode         CAN_Mode_Normal
/*²¨ÌØÂÊÅäÖÃ
  CAN BaudRate  42/(sjw+bs1+bs2)/pre (Mbps)*/
#define CAN1_SJW    CAN_SJW_1tq
#define CAN1_BS1    CAN_BS1_9tq
#define CAN1_BS2    CAN_BS2_4tq
#define CAN1_PRE    3
//
#define CAN1_TX_RCC_GPIOx        RCC_AHB1Periph_GPIOB
#define CAN1_TX_GPIOx            GPIOB
#define CAN1_TX_GPIO_PINx        GPIO_Pin_9
#define CAN1_TX_GPIO_PINSOURCEx  GPIO_PinSource9

#define CAN1_RX_RCC_GPIOx          RCC_AHB1Periph_GPIOB
#define CAN1_RX_GPIOx              GPIOB
#define CAN1_RX_GPIO_PINx          GPIO_Pin_8
#define CAN1_RX_GPIO_PINSOURCEx    GPIO_PinSource8
//
/************************************************/
#define BUFFER_SIZEMAX  30
/*****************USART1*************************/
#define USART1_CONFIG_TX 0   //Æô¶¯ÅäÖÃ´®¿Ú1·¢ËÍ

#define USART1_RCC_GPIOx             RCC_AHB1Periph_GPIOA
#define USART1_RX_GPIOx                   GPIOA
#define USART1_RX_GPIO_PINx               GPIO_Pin_10
#define USART1_RX_GPIO_PINSOURCEx         GPIO_PinSource10

#if  USART1_CONFIG_TX

#define USART1_RCC_GPIOx             RCC_AHB1Periph_GPIOA
#define USART1_TX_GPIOx                   //GPIOA
#define USART1_TX_GPIO_PINx               //GPIO_Pin_10
#define USART1_TX_GPIO_PINSOURCEx         //GPIO_PinSource10

#endif

#define USART1_RX_BUFF_SIZE 18
#define USART1_TX_BUFF_SIZE 8
#define USART1_BAUDRATE 100000
/*****************USART4************************OK*/
/*
´®¿Ú4  ---Ò£¿ØÆ÷½ÓÊÕ
*/
#define USART4_CONFIG_TX 1   //Æô¶¯ÅäÖÃ´®¿Ú1·¢ËÍ

#define USART4_RX_RCC_GPIOx             RCC_AHB1Periph_GPIOC
#define USART4_RX_GPIOx                   GPIOC
#define USART4_RX_GPIO_PINx               GPIO_Pin_11
#define USART4_RX_GPIO_PINSOURCEx         GPIO_PinSource11
////
#if  USART4_CONFIG_TX
#define USART4_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
#define USART4_TX_GPIOx                   GPIOC
#define USART4_TX_GPIO_PINx               GPIO_Pin_10
#define USART4_TX_GPIO_PINSOURCEx         GPIO_PinSource10
#endif
///
#define USART4_RX_BUFF_SIZE 60
#define USART4_TX_BUFF_SIZE 12

#define USART4_BAUDRATE 115200
//#define USART4_CONFIG_TX 1   //Æô¶¯ÅäÖÃ´®¿Ú1·¢ËÍ

//#define USART4_RX_RCC_GPIOx               RCC_AHB1Periph_GPIOA
//#define USART4_RX_GPIOx                   GPIOA
//#define USART4_RX_GPIO_PINx               GPIO_Pin_1
//#define USART4_RX_GPIO_PINSOURCEx         GPIO_PinSource1
//////
//#if  USART4_CONFIG_TX
//#define USART4_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
//#define USART4_TX_GPIOx                   GPIOC
//#define USART4_TX_GPIO_PINx               GPIO_Pin_10
//#define USART4_TX_GPIO_PINSOURCEx         GPIO_PinSource10
//#endif
/////
//#define USART4_RX_BUFF_SIZE 60
//#define USART4_TX_BUFF_SIZE 12

//#define USART4_BAUDRATE 115200
/*****************************************************/

/*****************USART2************************OK*/
#define USART2_CONFIG_TX 1   //Æô¶¯ÅäÖÃ´®¿Ú·¢ËÍ
#define USART2_CONFIG_CTR 1

#if     USART2_CONFIG_CTR
#define USART2_CTR_RCC_GPIOx               RCC_AHB1Periph_GPIOC
#define USART2_CTR_GPIOx                   GPIOC
#define USART2_CTR_GPIO_PINx               GPIO_Pin_13// GPIO_Pin_14¾É°æ
//#define USART2_CTR_GPIO_PINSOURCEx         GPIO_PinSource14
#endif
////
#define USART2_RX_RCC_GPIOx               RCC_AHB1Periph_GPIOA
#define USART2_RX_GPIOx                   GPIOA
#define USART2_RX_GPIO_PINx               GPIO_Pin_3
#define USART2_RX_GPIO_PINSOURCEx         GPIO_PinSource3
////
#if  USART2_CONFIG_TX
#define USART2_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOA
#define USART2_TX_GPIOx                   GPIOA
#define USART2_TX_GPIO_PINx               GPIO_Pin_2
#define USART2_TX_GPIO_PINSOURCEx         GPIO_PinSource2
#endif
///
#define USART2_RX_BUFF_SIZE 8
#define USART2_TX_BUFF_SIZE 8

#define USART2_BAUDRATE 115200
/*****************************************************/
/*********************USART6********************************OK*/
#define USART6_CONFIG_TX 1   //Æô¶¯ÅäÖÃ´®¿Ú·¢ËÍ

#define USART6_RX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
#define USART6_RX_GPIOx                   GPIOC
#define USART6_RX_GPIO_PINx               GPIO_Pin_7
#define USART6_RX_GPIO_PINSOURCEx         GPIO_PinSource7
////
#if  USART6_CONFIG_TX
#define USART6_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
#define USART6_TX_GPIOx                   GPIOC
#define USART6_TX_GPIO_PINx               GPIO_Pin_6
#define USART6_TX_GPIO_PINSOURCEx         GPIO_PinSource6
#endif
///
#define USART6_RX_BUFF_SIZE 60
#define USART6_TX_BUFF_SIZE 12

#define USART6_BAUDRATE 115200


//#define USART6_CONFIG_TX 0   //Æô¶¯ÅäÖÃ´®¿Ú·¢ËÍ

//#define USART6_RX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
//#define USART6_RX_GPIOx                   GPIOC
//#define USART6_RX_GPIO_PINx               GPIO_Pin_7
//#define USART6_RX_GPIO_PINSOURCEx         GPIO_PinSource7
//////
//#if  USART6_CONFIG_TX
//#define USART6_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
//#define USART6_TX_GPIOx                   GPIOC
//#define USART6_TX_GPIO_PINx               GPIO_Pin_6
//#define USART6_TX_GPIO_PINSOURCEx         GPIO_PinSource6
//#endif
/////
//#define USART6_RX_BUFF_SIZE 40
//#define USART6_TX_BUFF_SIZE 12

//#define USART6_BAUDRATE 115200
/********************************************************************/
/*******************USART5******************************************OK*/
#define USART5_CONFIG_TX 1   //Æô¶¯ÅäÖÃ´®¿Ú·¢ËÍ
#define USART5_CONFIG_CTR 0

#define USART5_RX_RCC_GPIOx               RCC_AHB1Periph_GPIOD
#define USART5_RX_GPIOx                   GPIOD
#define USART5_RX_GPIO_PINx               GPIO_Pin_2
#define USART5_RX_GPIO_PINSOURCEx         GPIO_PinSource2
////
#if  USART5_CONFIG_TX
#define USART5_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOC
#define USART5_TX_GPIOx                   GPIOC
#define USART5_TX_GPIO_PINx               GPIO_Pin_12
#define USART5_TX_GPIO_PINSOURCEx         GPIO_PinSource12
#endif
///
#if     USART5_CONFIG_CTR
#define USART5_CTR_RCC_GPIOx               RCC_AHB1Periph_GPIOB
#define USART5_CTR_GPIOx                   GPIOB
#define USART5_CTR_GPIO_PINx               GPIO_Pin_5

#endif

///
#define USART5_RX_BUFF_SIZE 30
#define USART5_TX_BUFF_SIZE 30

#define USART5_BAUDRATE 115200
/********************************************************************/
/*******************USART3******************************************OK*/
#define USART3_CONFIG_TX 1   //Æô¶¯ÅäÖÃ´®¿Ú·¢ËÍ
#define USART3_CONFIG_CTR 0

#define USART3_RX_RCC_GPIOx               RCC_AHB1Periph_GPIOB
#define USART3_RX_GPIOx                   GPIOB
#define USART3_RX_GPIO_PINx               GPIO_Pin_11
#define USART3_RX_GPIO_PINSOURCEx         GPIO_PinSource11
////
#if  USART3_CONFIG_TX
#define USART3_TX_RCC_GPIOx               RCC_AHB1Periph_GPIOB
#define USART3_TX_GPIOx                   GPIOB
#define USART3_TX_GPIO_PINx               GPIO_Pin_10
#define USART3_TX_GPIO_PINSOURCEx         GPIO_PinSource10
#endif
///
#if     USART3_CONFIG_CTR
#define USART3_CTR_RCC_GPIOx               RCC_AHB1Periph_GPIOB
#define USART3_CTR_GPIOx                   GPIOB
#define USART3_CTR_GPIO_PINx               GPIO_Pin_5
#endif
///
#define USART3_RX_BUFF_SIZE 18
#define USART3_TX_BUFF_SIZE 8

#define USART3_BAUDRATE 115200
/******************************************************************************************/
#define ABS(x) ((x)>0? (x):(-(x)))
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))
/***************************************************************************************/
//void delay_ms(unsigned int t);
//void delay_us(unsigned int t);
void BSP_Configration(void);
void System_INIT(void);
/***************************************************************************************/
#endif
