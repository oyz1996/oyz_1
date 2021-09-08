#ifndef _ELECT_IN_H_
#define _ELECT_IN_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include "commwind.h"
#include "elect.h"
#include "resolv.h"
#include "gpio.h"
#include "iic.h"
#include "tim4.h"
#include "tim3.h"
#include "tim1.h"
#include "exti.h"
#include "freehead.h"
#include <stdlib.h>
#include <tgmath.h>


#define Elect_Wait_Time			20000
#define Elect_MAX_Step			-30000
#define Elect_Spare_Step		-3200
#define Elect_EXIT_Step			8400
#define Elect_One_Step			2850
#define High_I2C_Regi				0x004
#define Low_I2C_Regi				0x030

#define Close_EXTI_5				EXTI->IMR &= ~(EXTI_Line5);
#define Open_EXTI_5					EXTI->IMR |= EXTI_Line5; 	
#endif


