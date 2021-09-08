#ifndef _TIM1_IN_H_
#define _TIM1_IN_H_



#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "tim1.h"

#define TIM1_PWM_RCC					RCC_APB2Periph_TIM1
#define TIM1_PWM_GPIO_RCC			RCC_AHB1Periph_GPIOE
#define TIM1_PWM_GPIO					GPIOE
#define TIM1_PWM_PIN					GPIO_Pin_9
#define TIM1_PWM_PIN_SRC			GPIO_PinSource9

#define TIM1_MOTOR_IN1_RCC				RCC_APB2Periph_TIM1
#define TIM1_MOTOR_IN1_GPIO_RCC		RCC_AHB1Periph_GPIOE
#define TIM1_MOTOR_IN1_GPIO				GPIOE
#define TIM1_MOTOR_IN1_PIN				GPIO_Pin_10
#define TIM1_MOTOR_IN1_SRC				GPIO_PinSource10

#define TIM1_MOTOR_IN2_PIN				GPIO_Pin_11
#define TIM1_MOTOR_IN2_SRC				GPIO_PinSource11
#define TIM1_RCC_INIT	\
	do{\
		RCC_APB2PeriphClockCmd(TIM1_PWM_RCC,ENABLE);\
		}while(0)

#define TIM1_MOTOR_RCC_INIT \
	do{\
		RCC_APB2PeriphClockCmd(TIM1_MOTOR_IN1_RCC,ENABLE);\
		}while(0)
			
		
		
#define TIM1_GPIO_RCC_INIT	\
		do{\
			RCC_AHB1PeriphClockCmd(TIM1_PWM_GPIO_RCC,ENABLE);\
			}while(0)

#define TIM1_MOTOR_GPIO_RCC_INIT	\
		do{\
			RCC_AHB1PeriphClockCmd(TIM1_MOTOR_IN1_GPIO_RCC,ENABLE);\
			}while(0)			
		
			
#define TIM1_PWM_GPIO_INIT	\
			do{\
				GPIO_InitTypeDef	GPIO_Struct;	\
				GPIO_PinAFConfig(TIM1_PWM_GPIO,TIM1_PWM_PIN_SRC,GPIO_AF_TIM1);\
				\
				GPIO_Struct.GPIO_Pin	=	TIM1_PWM_PIN ;\
				GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;\
				GPIO_Struct.GPIO_OType = GPIO_OType_PP;\
				GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;\
				GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;\
				GPIO_Init(TIM1_PWM_GPIO,&GPIO_Struct);\
				\
				}while(0)
			
#define TIM1_MOTOR_GPIO_INIT	\
			do{\
				GPIO_InitTypeDef	GPIO_Struct;	\
				GPIO_PinAFConfig(TIM1_MOTOR_IN1_GPIO,TIM1_MOTOR_IN1_SRC,GPIO_AF_TIM1);\
				GPIO_PinAFConfig(TIM1_MOTOR_IN1_GPIO,TIM1_MOTOR_IN2_SRC,GPIO_AF_TIM1);\
				\
				GPIO_Struct.GPIO_Pin	=	TIM1_MOTOR_IN2_PIN ;\
				GPIO_Struct.GPIO_Mode = GPIO_Mode_AF;\
				GPIO_Struct.GPIO_OType = GPIO_OType_PP;\
				GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL;\
				GPIO_Struct.GPIO_Speed = GPIO_Speed_100MHz;\
				GPIO_Init(TIM1_MOTOR_IN1_GPIO,&GPIO_Struct);\
				\
				}while(0)
			
#endif
