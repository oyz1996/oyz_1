#include "tim9_in.h"

static void TIM9_PWM_RCC_Config(void)
{
	TIM9_RCC_INIT;
	TIM9_GPIO_RCC_INIT;
}

static void TIM9_PWM_GPIO_Config(void)
{
	TIM9_PWM_GPIO_INIT;
}

static void TIM9_PWM_Mode_Config(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIM_OC_Struct; 
	
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = arr;
	TIM_Struct.TIM_Prescaler = psc;
	TIM_Struct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM9,&TIM_Struct);
	
	TIM_OC_Struct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OC_Struct.TIM_Pulse = (arr+1)*30/100;	
	TIM_OC_Struct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC1Init(TIM9,&TIM_OC_Struct);
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM9,ENABLE);

	
}
//psc  16800-1    arr	20-1
void TIM9_Config(uint32_t psc,uint32_t arr)
{
		TIM9_PWM_RCC_Config();
		TIM9_PWM_GPIO_Config();
		TIM9_PWM_Mode_Config(psc,arr);
		TIM_Cmd(TIM9,ENABLE);
}	
static void TIM9_Change_ARR(uint32_t arr,uint32_t ccr)
{
	TIM_SetCompare1(TIM9,(arr+1)*ccr/100);
}

void Open_Back_Fan(uint32_t ccr,uint8_t InitCcr,uint8_t EndCcr)
{
		for(int i = InitCcr ; i >= EndCcr ; i--)
	{
		TIM9_Change_ARR(ccr,i);
	}
}
