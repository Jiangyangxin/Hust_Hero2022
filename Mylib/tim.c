/**********************************************************************************************************
 * @�ļ�     tim.c
 * @˵��     c�ļ�
 * @�汾  	 V1.0
 * @����     ������
 * @����     2019.10
**********************************************************************************************************/
#include "main.h"
#include "tim.h"
extern int counti;
/**********************************************************************************************************
*�� �� ��: TIM4_Init
*����˵��: tim4��ʼ��
*��    ��: ��
*�� �� ֵ: ��
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
*�� �� ��: TIM4_IRQHandler
*����˵��: tim4�жϺ���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void  TIM4_IRQHandler (void)
{
    if ( TIM_GetITStatus( TIM4, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
    }
}
