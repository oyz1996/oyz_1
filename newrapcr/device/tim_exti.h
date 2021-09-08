#ifndef __TIM_EXTI_H_
#define __TIM_EXTI_H_

#include <stdint.h>

struct TIM_EXTI_HOOK
{
	void (* timhook)(uint32_t tim_num);
	void (* extihook)(uint32_t exti_num);
};

#endif
