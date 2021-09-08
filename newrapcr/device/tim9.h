#ifndef _TIM9_H_
#define _TIM9_H_

#include <stdint.h>

#define	TIM9_25KHZ_PSC			168-1
#define TIM9_25KHZ_ARR			40-1

void TIM9_Config(uint32_t psc,uint32_t arr);
void Open_Back_Fan(uint32_t ccr,uint8_t InitCcr,uint8_t EndCcr);
#endif

