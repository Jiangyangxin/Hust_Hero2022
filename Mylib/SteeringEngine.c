/**********************************************************************************************************
 * @文件     SteeringEngine.c
 * @说明     弹舱盖舵机初始化
 * @版本  	 V1.0
 * @作者     黄志雄
 * @日期     2020.1
**********************************************************************************************************/
#include "main.h"
#include "SteeringEngine.h"
/**********************************************************************************************************
*函 数 名: SteeringEngine_Configuration
*功能说明: 弹舱盖舵机初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SteeringEngine_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);

    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

    TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period=20000-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler=72-1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

    TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
    TIM_OC3Init(TIM3,&TIM_OCInitStruct);

    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3,ENABLE);

    TIM_CtrlPWMOutputs(TIM3,ENABLE);

    TIM_Cmd(TIM3,ENABLE);
}

/**********************************************************************************************************
*函 数 名: SteeringEngine_Set
*功能说明: 开/关弹舱盖
*形    参: position
*返 回 值: 无
**********************************************************************************************************/
unsigned char magazineState=0x00; //初次记录为关着的状态
extern RobotInit_Struct Hero;
void SteeringEngine_Set(int position)
{
    if(position==Hero.MagOpen)
        magazineState=0x01;
    if(position==Hero.MagClose)
        magazineState=0x00;
    TIM_SetCompare3(TIM3,position);
}

