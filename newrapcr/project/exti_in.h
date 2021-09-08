#ifndef __EXTI_IN_H_
#define __EXTI_IN_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "freehead.h"
#include "elect.h"
#include "tim4.h"
#include "exti.h"
#include "tim3.h"
#define EXTI_GPIO_RCC		RCC_AHB1Periph_GPIOC	
#define	EXTI_GPIO				GPIOC
#define EXTI_PIN				GPIO_Pin_5

#define EXTI_RCC_INIT	\
		do{	\
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);\
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);\
			RCC_AHB1PeriphClockCmd(EXTI_GPIO_RCC,ENABLE);\
		}while(0);	

#define EXTI_GPIO_INIT	\
		do{	\
			GPIO_InitTypeDef 	GPIOInit;\
			GPIOInit.GPIO_Pin = EXTI_PIN; \
			GPIOInit.GPIO_Mode = GPIO_Mode_IN;\
			GPIOInit.GPIO_PuPd = GPIO_PuPd_DOWN;\
			GPIOInit.GPIO_Speed 	= GPIO_Speed_25MHz;\
			GPIO_Init(EXTI_GPIO,&GPIOInit);\
		}while(0);
	
// 使用 vTaskStartScheduler() 需要将 TIM4_IRQ_PRE_PRIOR 修改为 TIM4_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1		
#define EXTI_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY +  1
#define EXTI_IRQ_SUB_PRIOR    0
#endif
