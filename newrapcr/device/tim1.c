#include "tim1_in.h"

static void TIM1_PWM_RCC_Config(void)
{
	TIM1_RCC_INIT;
	TIM1_GPIO_RCC_INIT;
}

static void TIM1_MOTOR_RCC_Config(void)
{
		TIM1_MOTOR_RCC_INIT;
		TIM1_MOTOR_GPIO_RCC_INIT;
}

static void TIM1_PWM_GPIO_Config(void)
{
	TIM1_PWM_GPIO_INIT;
}

static void TIM1_MOTOR_GPIO_Config(void)
{
	TIM1_MOTOR_GPIO_INIT;
}

static void TIM1_PWM_Mode_Config(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIM_OC_Struct; 
	
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = arr;
	TIM_Struct.TIM_Prescaler = psc;
	TIM_Struct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1,&TIM_Struct);
	
	TIM_OC_Struct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OC_Struct.TIM_Pulse = (arr + 1) * 0.7;	
	TIM_OC_Struct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC1Init(TIM1,&TIM_OC_Struct);

	TIM_OC_Struct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OC_Struct.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	
	TIM_OC_Struct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OC_Struct.TIM_OCPolarity = TIM_OCPolarity_Low;
	
	TIM_OC_Struct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC_Struct.TIM_Pulse = (arr + 1) * 0.75  ;
	TIM_OC2Init(TIM1,&TIM_OC_Struct);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
//	TIM_ARRPreloadConfig(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

}

static void TIM1_MOTOR_PWM_Mode_Config(uint16_t psc,uint16_t arr)
{
	TIM_OCInitTypeDef TIM_OC_Struct; 
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_OC_Struct.TIM_OCMode = TIM_OCMode_Toggle;		
	TIM_OC_Struct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC2Init(TIM1,&TIM_OC_Struct);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
		/*
		TIM_ITConfig(TIM1, TIM_IT_CC2 , ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	  NVIC_Init(&NVIC_InitStructure); //配置中断
		*/
	
//	TIM_ARRPreloadConfig(TIM1,ENABLE);
//	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	
}
void TIM1_Config(uint32_t psc,uint32_t arr)
{

		TIM1_PWM_RCC_Config();
		TIM1_MOTOR_RCC_Config();
		TIM1_PWM_GPIO_Config();
		TIM1_MOTOR_GPIO_Config();
		TIM1_PWM_GPIO_Config();
		TIM1_PWM_Mode_Config(psc,arr);
	//	TIM1_MOTOR_PWM_Mode_Config(psc-1,arr);

		TIM_Cmd(TIM1,ENABLE);
			TIM_CCxCmd(TIM1,TIM_Channel_2, DISABLE);
		TIM_CCxNCmd(TIM1,TIM_Channel_2, DISABLE);
//		TIM_SetCompare1(TIM1,arr/2);
//		TIM_SetCompare2(TIM1,0XFFFF);
}	

void TIM1_MOTOR_Config(uint32_t psc,uint32_t arr)
{
		TIM1_MOTOR_RCC_Config();
		TIM1_MOTOR_GPIO_Config();
	//	TIM1_MOTOR_PWM_Mode_Config(psc,arr);
//		TIM_Cmd(TIM1,ENABLE);
}

void TIM1_Change_ARR(uint32_t arr,uint32_t ccr)
{
	double pulse = 0; 
	pulse = (double)ccr / 100;
	TIM_SetCompare1(TIM1,(arr + 1) * pulse);
}
static void TIM1_Change_Motor_ARR(uint32_t arr,uint32_t ccr)
{
	double pulse = 0; 
	pulse = (double)ccr / 100;
	TIM_SetCompare2(TIM1,(arr + 1) * pulse);
}
void Change_Motor_Freq(uint32_t ccr,uint8_t ChangeCcr)
{
	TIM1_Change_Motor_ARR(ccr,ChangeCcr);
}
void Open_TEC_Fan(uint32_t ccr,uint8_t InitCcr,uint8_t EndCcr)
{
//	for(int i = InitCcr ; i >= EndCcr ; i--)
//	{
		TIM1_Change_ARR(ccr,EndCcr);
//	}
}

#define tim3ch1p 10  //频率单位hz
#define tim3ch1z 50   //占空比单位%
void TIM1_CC_IRQHandler ()
{
   u16 ch2p=1000000/tim3ch1p;//1us*CCR=1/100,10^(-6)*CCR=10^(-2),CCR=10^(-2)/10^(-6)=10^6/10^2
   u16 ch2z=ch2p*tim3ch1z/100;
   u16 ccr=0;
   static u8 tim3flag=0;
	if(TIM_GetITStatus(TIM1, TIM_IT_CC2))
	{
		ccr=TIM_GetCapture2(TIM1);//读取当前CCR的值
		if(tim3flag)
			TIM_SetCompare2(TIM1, ccr+ch2p-ch2z);		 
		else
			 TIM_SetCompare2(TIM1,ccr+ch2z);
		
		tim3flag^=1;//不断取反
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
	}
	
}
