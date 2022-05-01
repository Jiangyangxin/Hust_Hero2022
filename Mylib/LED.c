#include "LED.h"

void LED_Conf()
{
	  GPIO_InitTypeDef  gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//AHB1
	  gpio.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_OUT;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC,&gpio);
	  gpio.GPIO_Pin = GPIO_Pin_13;
	  GPIO_SetBits(GPIOC,GPIO_Pin_14);
	  GPIO_SetBits(GPIOC,GPIO_Pin_13);

}

void LED_ON_RED()
{
   GPIO_ResetBits(GPIOC,GPIO_Pin_14);
}

void LED_ON_BLUE()
{
   GPIO_ResetBits(GPIOC,GPIO_Pin_13);
}

void LED_OFF_ALL()
{
   GPIO_SetBits(GPIOC,GPIO_Pin_14);
	 GPIO_SetBits(GPIOC,GPIO_Pin_13);
	 
}

void LED_OFF_RED()
{
   GPIO_SetBits(GPIOC,GPIO_Pin_14);	 
}

void LED_OFF_BLUE()
{
   GPIO_SetBits(GPIOC,GPIO_Pin_13);
}


