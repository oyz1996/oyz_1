#ifndef __TIM4_H_
#define __TIM4_H_

#include <stdint.h>
#include "tim_exti.h"

void TIM4_Config(uint32_t arr);
extern struct TIM_EXTI_HOOK tim4hook;

#endif

