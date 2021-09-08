#ifndef _TIM3_H_
#define _TIM3_H_

#include "stdint.h"
//	176-1
#define	TIM3_20KHZ_PSC			21-1
#define TIM3_20KHZ_ARR			200000-1
#define TIM3_Pulse_1US			226

#define MAX_FRE_ARRAY					2000
#define MAX_ACTU_FRE_ARRAY		MAX_FRE_ARRAY/3
#define ACC_ACTU_FRE_ARRAY		MAX_FRE_ARRAY/4
#define UV_ACTU_FRE_ARRAY			( ACC_ACTU_FRE_ARRAY + MAX_FRE_ARRAY/2)
#define DECE_ACTU_FRE_ARRAY		(UV_ACTU_FRE_ARRAY + MAX_FRE_ARRAY/4)
#define MAX_PERI_ARRAY				MAX_FRE_ARRAY

typedef struct
{
	float * fre;
	unsigned short * peri;
	float Max_Fre;
	float Min_Fre;
	float Flex;
	uint32_t curnum;
	
}FrePeri;

extern FrePeri FrePeriStruct; 
extern FrePeri FrefubenStruct;
void TIM3_Config(uint32_t psc,uint32_t arr);
void Calcu_Config(FrePeri * freperiod);
#endif

