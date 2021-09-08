#ifndef __INSPECTION_IN_H_
#define __INSPECTION_IN_H_

#include "stm32f4xx_gpio.h"
#include "freehead.h"
#include "inspection.h"
#include "resolv.h"
#include "worksequ.h"
#include "control.h"
#include "commwind.h"
#include "usart1.h"
#include "usart3.h"
#include "gpio.h"
#include "adc.h"
#include "elect.h"
#include "tim3.h"
#include "exti.h"
#include	<string.h>

#define Over_Time_MAX	30
#define Compare_Small(Set_Temp,Target_Temp)   (Set_Temp) > (Target_Temp) ? (Set_GPIO(Low_Pin,TOP_HEATER_EN)) : Set_GPIO(High_Pin,TOP_HEATER_EN);
#endif
