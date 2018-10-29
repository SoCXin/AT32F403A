#include "led.h"
#include "ALL_Includes.h"

/****************************************************
函数功能：LED初始化
输入参数：无
输出参数：无
备    注：调用此函数前，需要在LED.h修改宏定义LED引脚
****************************************************/
void LED_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_AHBPeriphClockCmd(LED_GPIO_CLK, ENABLE);
   
    GPIO_InitStructure.GPIO_Pin = LED_PIN1|LED_PIN2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

/****************************************************
函数功能：LED开
输入参数：1 LED1, 2 LED2
输出参数：无
备    注：调用此函数前，需要在LED.h修改宏定义LED引脚
****************************************************/
void LED_ON(u8 res)
{
	if(res==1)
	{
		GPIO_SetBits(LED_PORT, LED_PIN1);
	}
	else if(res==2)
	{
		GPIO_SetBits(LED_PORT, LED_PIN2);		
	}
}

/****************************************************
函数功能：LED关
输入参数：1  LED1 ,2 LED2
输出参数：无
备    注：调用此函数前，需要在LED.h修改宏定义LED引脚
****************************************************/
void LED_OFF(u8 res)
{
	if(res==1)
	{
		GPIO_ResetBits(LED_PORT, LED_PIN1);
	}
	else if(res==2)
	{
		GPIO_ResetBits(LED_PORT, LED_PIN2);		
	}
}

