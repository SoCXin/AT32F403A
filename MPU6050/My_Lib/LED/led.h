#ifndef __led_H
#define __led_H 

#include "ALL_Includes.h"

#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOA 
#define LED_PORT   	   GPIOA
#define LED_PIN1        GPIO_Pin_5
#define LED_PIN2        GPIO_Pin_6


void LED_Init(void);
void LED_ON(u8 res);
void LED_OFF(u8 res);

#endif



