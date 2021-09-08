#ifndef __TIM4_IN_H_
#define __TIM4_IN_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "freehead.h"
#include "tim4.h"


// 使用 vTaskStartScheduler() 需要将 TIM4_IRQ_PRE_PRIOR 修改为 TIM4_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3
#define TIM4_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2
#define TIM4_IRQ_SUB_PRIOR    0

#endif

