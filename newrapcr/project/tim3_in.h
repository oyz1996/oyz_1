#ifndef __TIM3_IN_H_
#define __TIM3_IN_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "gpio.h"
#include "tim3.h"
#include "elect.h"
#include "freehead.h"
#include <tgmath.h>

#define TIM3_PWM_RCC					RCC_APB1Periph_TIM3
#define TIM3_PWM_GPIO_RCC			RCC_AHB1Periph_GPIOB
#define TIM3_PWM_GPIO					GPIOB
#define TIM3_PWM_PIN					GPIO_Pin_0
#define TIM3_PWM_PIN_SRC			GPIO_PinSource0

#define TIM3_RCC_INIT	\
	do{\
		RCC_APB1PeriphClockCmd(TIM3_PWM_RCC,ENABLE);\
		}while(0)

#define TIM3_GPIO_RCC_INIT	\
		do{\
			RCC_AHB1PeriphClockCmd(TIM3_PWM_GPIO_RCC,ENABLE);\
			}while(0)

#define TIM3_PWM_GPIO_INIT	\
			do{\
				GPIO_InitTypeDef	GPIO_Struct;	\
				GPIO_PinAFConfig(TIM3_PWM_GPIO,TIM3_PWM_PIN_SRC,GPIO_AF_TIM3);\
				\
				GPIO_Struct.GPIO_Pin	=	TIM3_PWM_PIN;\
				GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;\
				GPIO_Struct.GPIO_OType = GPIO_OType_PP;\
				GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;\
				GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;\
				GPIO_Init(TIM3_PWM_GPIO,&GPIO_Struct);\
				\
				}while(0)
			
#define TIM3_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2
#define TIM3_IRQ_SUB_PRIOR    0
				
#define MIN_ARRAY_INDEX				1
#define MAX_ARRAY_INDEX				MAX_FRE_ARRAY
				

#endif
