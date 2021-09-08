#ifndef _TIM9_IN_H_
#define _TIM9_IN_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "tim9.h"

#define TIM9_PWM_RCC					RCC_APB2Periph_TIM9
#define TIM9_PWM_GPIO_RCC			RCC_AHB1Periph_GPIOE
#define TIM9_PWM_GPIO					GPIOE
#define TIM9_PWM_PIN					GPIO_Pin_5
#define TIM9_PWM_PIN_SRC			GPIO_PinSource5

#define TIM9_RCC_INIT	\
	do{\
		RCC_APB2PeriphClockCmd(TIM9_PWM_RCC,ENABLE);\
		}while(0)

#define TIM9_GPIO_RCC_INIT	\
		do{\
			RCC_AHB1PeriphClockCmd(TIM9_PWM_GPIO_RCC,ENABLE);\
			}while(0)

#define TIM9_PWM_GPIO_INIT	\
			do{\
				GPIO_InitTypeDef	GPIO_Struct;	\
				GPIO_PinAFConfig(TIM9_PWM_GPIO,TIM9_PWM_PIN_SRC,GPIO_AF_TIM9);\
				\
				GPIO_Struct.GPIO_Pin	=	TIM9_PWM_PIN;\
				GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;\
				GPIO_Struct.GPIO_OType = GPIO_OType_PP;\
				GPIO_Struct.GPIO_PuPd = GPIO_PuPd_UP;\
				GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;\
				GPIO_Init(TIM9_PWM_GPIO,&GPIO_Struct);\
				\
				}while(0)
#endif
