#ifndef __EXTI_H_
#define __EXTI_H_



#include "tim_exti.h"
#include "stm32f4xx_exti.h"

extern EXTI_InitTypeDef	EXTIInit;
extern NVIC_InitTypeDef	NVICInit;
extern GPIO_InitTypeDef 	GPIOInit_t;

void EXTI5_Init(void);
extern struct TIM_EXTI_HOOK exti5hook;
#endif

