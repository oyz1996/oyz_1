#include "tim4_in.h"

int oioi = 1;
static void TIM4_NVIC_Config(void)
{
	NVIC_InitTypeDef NVICInit;
	
	NVICInit.NVIC_IRQChannel = TIM4_IRQn;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVICInit.NVIC_IRQChannelPreemptionPriority = TIM4_IRQ_PRE_PRIOR;
	NVICInit.NVIC_IRQChannelSubPriority = TIM4_IRQ_SUB_PRIOR;
	
	NVIC_Init(&NVICInit);	

}

static void TIM4_Mode_Config(uint32_t arr)
{

		TIM_TimeBaseInitTypeDef TIMInit;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	  TIMInit.TIM_Period = arr;                                                   
    TIMInit.TIM_Prescaler =0;                                                                        
    TIMInit.TIM_ClockDivision = 0;                
    TIMInit.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM4, &TIMInit);
	
	 TIM_SelectInputTrigger(TIM4, TIM_TS_ITR2);//TIM3主连接TIM4,TIM_TS_ITR2
   TIM_SelectSlaveMode(TIM4,TIM_SlaveMode_External1);// 等同 TIM2->SMCR|=0x07 //设置从模式寄存器
	
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE);
	

}
struct TIM_EXTI_HOOK tim4hook;

static void TIM_VAR_Config(void)
{
	tim4hook.timhook = 0;
}
void TIM4_Config(uint32_t arr)
{
	TIM4_Mode_Config(arr);
	TIM4_NVIC_Config();
}

int ssss;
void TIM4_IRQHandler(void) 
{ 
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)     // TIM_IT_CC1
    { 
			 TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除中断标志位 
			TIM_CtrlPWMOutputs(TIM3, DISABLE);
				ssss = TIM4->CNT;

        TIM_Cmd(TIM3, DISABLE); // 关闭定时器 
        TIM_Cmd(TIM4, DISABLE); // 关闭定时器 

				if(tim4hook.timhook != 0 )
				{
					tim4hook.timhook(TIM4->ARR);
				}			
				        TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE); 
    } 
}
