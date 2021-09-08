#ifndef _TIM1_H_
#define _TIM1_H_

#include <stdint.h>

#define	TIM1_25KHZ_PSC			168-1
#define TIM1_25KHZ_ARR			40-1
#define TIM1_MOTOR_ARR			160-1

void TIM1_Config(uint32_t psc,uint32_t arr);
void TIM1_MOTOR_Config(uint32_t psc,uint32_t arr);
void Open_TEC_Fan(uint32_t ccr,uint8_t InitCcr,uint8_t EndCcr);
void Change_Motor_Freq(uint32_t ccr,uint8_t ChangeCcr);
#endif

