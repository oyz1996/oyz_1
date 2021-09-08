#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

typedef enum
{
	TEMP_TOP_HEAT,
	TEMP_FIN,
	Cur_Heatside,
	Cur_Heattop,
	Cur_LAMP
}_Volt_Pin;

void ADC_Config(void);
double Read_Pin_Volt(_Volt_Pin Volt_Pin);


#endif

