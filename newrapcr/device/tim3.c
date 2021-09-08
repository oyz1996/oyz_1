#include "tim3_in.h"

FrePeri FrePeriStruct = {NULL,NULL,3000,1500,4,0};
FrePeri FrefubenStruct = {NULL,NULL,3000,1500,4,0};
 static void TIM3_PWM_RCC_Config(void)
{
	TIM3_RCC_INIT;
	TIM3_GPIO_RCC_INIT;
}

static void TIM3_PWM_GPIO_Config(void)
{
	TIM3_PWM_GPIO_INIT;
}

static void TIM3_NVIC_Config(void)
{
	NVIC_InitTypeDef NVICInit;
	
	NVICInit.NVIC_IRQChannel = TIM3_IRQn;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVICInit.NVIC_IRQChannelPreemptionPriority = TIM3_IRQ_PRE_PRIOR;
	NVICInit.NVIC_IRQChannelSubPriority = TIM3_IRQ_SUB_PRIOR;
	
	NVIC_Init(&NVICInit);	
	TIM_ClearFlag(TIM3, TIM_IT_CC1); 
	TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	
}

static void TIM3_PWM_Mode_Config(uint16_t psc,uint16_t arr)
{
	TIM_TimeBaseInitTypeDef TIM_Struct;
	TIM_OCInitTypeDef TIM_OC_Struct; 
	
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Struct.TIM_Period = arr;
	TIM_Struct.TIM_Prescaler = psc;
	TIM_Struct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3,&TIM_Struct);
	//arr/TIM3_Pulse_1UST
	TIM_OC_Struct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OC_Struct.TIM_Pulse = arr/TIM3_Pulse_1US;	
	TIM_OC_Struct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC_Struct.TIM_OutputState = TIM_OutputState_Enable;
	
	TIM_OC3Init(TIM3,&TIM_OC_Struct);
	
	TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
	
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
}

void TIM3_Config(uint32_t psc,uint32_t arr)
{
		TIM3_PWM_RCC_Config();
		TIM3_PWM_GPIO_Config();
		TIM3_NVIC_Config();
		TIM3_PWM_Mode_Config(psc,arr);
		TIM_Cmd(TIM3,DISABLE);
}	


void Calcu_Config(FrePeri * freperiod)
{
	float Loca_fre = 0;
	unsigned short Local_peri[MAX_FRE_ARRAY]={0};
	CalculateSModelLine(&Loca_fre,Local_peri,MAX_ARRAY_INDEX,freperiod->Max_Fre,freperiod->Min_Fre,freperiod->Flex);
	freperiod->fre = &Loca_fre;
	freperiod->peri = Local_peri;
}

void CalculateSModelLine(float  * fre,  unsigned short period[],   float  len,  float fre_max,  float fre_min, float flexible)
{
    int i=0;
		float local_fre = 0; 
    float deno = 0 ;
    float melo = 0;
    float delt = fre_max-fre_min;
    for(; i<len; i++)
    {
        melo = flexible * (i-len/2) / (len/2);
        deno = 1.0f / (1 + expf(-melo));   //expf is a library function of exponential(e) 
        local_fre = delt * deno + fre_min;
       period[i] = (unsigned short)(2000000 / local_fre);    // 10000000 is the timer driver frequency
    }
		*fre = local_fre;
    return ;
}

void TIM3_IRQHandler(void)
{

	if ( TIM_GetITStatus(TIM3 , TIM_IT_CC1) != RESET )
	{
		static int list_temp = 0; 
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		FreStruct.SurNum--;
		if(FreStruct.SpeedStruct == InitState)
		{
			FreStruct.CurState = InitState;
			TIM_SetAutoreload(TIM3, FrePeriStruct.peri[FrePeriStruct.curnum++]);
		if(FrePeriStruct.curnum >= MAX_ARRAY_INDEX)
		{
			FrePeriStruct.curnum = 0;
			Set_GPIO(1,STEP_EN);
			TIM_ITConfig(TIM3, TIM_IT_CC1, DISABLE);
		}
		}
		else if (FreStruct.SpeedStruct == IncSpeedState)
		{
		//加速
			FreStruct.CurState = IncSpeedState;
			
			if(FrePeriStruct.curnum <= ACC_ACTU_FRE_ARRAY )
			{
			//	FrePeriStruct.curnum++;
					TIM_SetAutoreload(TIM3, FrePeriStruct.peri[FrePeriStruct.curnum]);
					FrePeriStruct.curnum++;
			}
			else 
			{
				list_temp = FrePeriStruct.curnum;
				FreStruct.SpeedStruct = AvrFlag;
			}
		}
		else if (FreStruct.SpeedStruct == AvrFlag)
		{
			//匀速
			FreStruct.SpeedStruct = AvrFlag;
			FrePeriStruct.curnum++;
			if(FrePeriStruct.curnum > UV_ACTU_FRE_ARRAY)
			{
				FreStruct.SpeedStruct = DecelSpeedState;
			}
			
		}
			else if (FreStruct.SpeedStruct == DecelSpeedState)
		{	
			//减速
			FreStruct.SpeedStruct = DecelSpeedState;
			if(list_temp <= MIN_ARRAY_INDEX )
			{
				list_temp = MIN_ARRAY_INDEX;
			}
			TIM_SetAutoreload(TIM3, FrePeriStruct.peri[list_temp--]);		
		}

	}
}
