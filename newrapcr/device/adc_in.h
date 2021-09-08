#ifndef _ADC_IN_H_
#define _ADC_IN_H_

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"
#include "adc.h"
#include "freehead.h"
#include <math.h>
#include <stdio.h>

#define TMEP_TOP_HEAT_RCC						RCC_AHB1Periph_GPIOA
#define TEMP_TOP_HEAT_GPIO					GPIOA
#define TEMP_TOP_HEAT_PIN						GPIO_Pin_4
#define TEMP_TOP_HEAT_CHIN					ADC_Channel_4

#define TEMP_FIN_RCC								RCC_AHB1Periph_GPIOA
#define TEMP_FIN_GPIO								GPIOA
#define TEMP_FIN_PIN								GPIO_Pin_2
#define TEMP_FIN_CHIN								ADC_Channel_2

#define Cur_Heatside_RCC						RCC_AHB1Periph_GPIOA
#define Cur_Heatside_GPIO						GPIOA
#define Cur_Heatside_PIN						GPIO_Pin_3
#define Cur_Heatside_CHIN						ADC_Channel_3

#define Cur_Heattop_RCC							RCC_AHB1Periph_GPIOA
#define Cur_Heattop_GPIO						GPIOA
#define Cur_Heattop_PIN							GPIO_Pin_1
#define Cur_Heattop_CHIN						ADC_Channel_1

#define Cur_LAMP_RCC								RCC_AHB1Periph_GPIOA
#define Cur_LAMP_GPIO								GPIOA
#define Cur_LAMP_PIN								GPIO_Pin_5
#define Cur_LAMP_CHIN								ADC_Channel_5

#define ADC_RCC_INIT \
		do{\
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);	\
		}while(0)
#define ADC_GPIO_RCC_INIT \
			do{\
				RCC_AHB1PeriphClockCmd(TMEP_TOP_HEAT_RCC | TEMP_FIN_RCC | Cur_Heatside_RCC | Cur_Heattop_RCC | Cur_LAMP_RCC,ENABLE);\
				}while(0)
			
#define ADC_GPIO_INIT	\
		do{\
			GPIO_InitTypeDef GPIO_Struct;\
			\
			GPIO_Struct.GPIO_Pin = TEMP_TOP_HEAT_PIN;	\
			GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;	\
			GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL ;\
			GPIO_Init(TEMP_TOP_HEAT_GPIO,&GPIO_Struct); \
			\
			GPIO_Struct.GPIO_Pin = TEMP_FIN_PIN;	\
			GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;	\
			GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL ;\
			GPIO_Init(TEMP_FIN_GPIO,&GPIO_Struct); \
			\
			GPIO_Struct.GPIO_Pin = Cur_Heatside_PIN;	\
			GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;	\
			GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL ;\
			GPIO_Init(Cur_Heatside_GPIO,&GPIO_Struct); \
			\
			GPIO_Struct.GPIO_Pin = Cur_Heattop_PIN;	\
			GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;	\
			GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL ;\
			GPIO_Init(Cur_Heattop_GPIO,&GPIO_Struct); \
			\
			GPIO_Struct.GPIO_Pin = Cur_LAMP_PIN;	\
			GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;	\
			GPIO_Struct.GPIO_PuPd = GPIO_PuPd_NOPULL ;\
			GPIO_Init(Cur_LAMP_GPIO,&GPIO_Struct); \
		}while(0)
#endif
		
