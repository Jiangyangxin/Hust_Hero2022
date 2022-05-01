/**********************************************************************************************************
 * @文件     tim.c
 * @说明     c文件
 * @版本  	 V1.0
 * @作者     黄沛霖
 * @日期     2019.10
**********************************************************************************************************/
#include "main.h"
#include "tim.h"
extern int counti;
/**********************************************************************************************************
*函 数 名: TIM4_Init
*功能说明: tim4初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void TIM4_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_TimeBaseInitStructure.TIM_Period = 20000-1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 21000-1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
                                                                    //((1+TIM_Prescaler )/42M)*(1+TIM_Period ) =10 s 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**********************************************************************************************************
*函 数 名: TIM4_IRQHandler
*功能说明: tim4中断函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void  TIM4_IRQHandler (void)
{
    if ( TIM_GetITStatus( TIM4, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
    }
}
