#include "exti_in.h"

	EXTI_InitTypeDef	EXTIInit;
	NVIC_InitTypeDef	NVICInit;
GPIO_InitTypeDef 	 GPIOInit_t;
static void EXTI_RCC_Config(void)
{
	EXTI_RCC_INIT;
}

static void EXTI_GPIO_Config(void)
{
	EXTI_GPIO_INIT;
}

static void EXTI_NVIC_Config(void)
{
	EXTI_DeInit();

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
	
	EXTIInit.EXTI_Line = EXTI_Line5;
	EXTIInit.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTIInit.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTIInit.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTIInit);

	NVICInit.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVICInit.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRE_PRIOR;
	NVICInit.NVIC_IRQChannelSubPriority = EXTI_IRQ_SUB_PRIOR;
	NVICInit.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVICInit);
	EXTI->IMR &= ~(EXTI_Line5);
	EXTI_GenerateSWInterrupt(EXTI_Line5);
}
struct TIM_EXTI_HOOK exti5hook;
static void EXTI_VAR_Config(void)
{
	exti5hook.extihook = 0;
}
void EXTI5_Init(void)
{
	EXTI_RCC_Config();
	EXTI_GPIO_Config();
	EXTI_NVIC_Config();
	EXTI_VAR_Config();
	
}
int step_num = -1;
//修改响应不及时:		清除中断放在最前。
//测试： 						清除中断放在最后。
void EXTI9_5_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{

		if(Elec_Struct.Check_Index )
		{
	//		EXTIInit.EXTI_Trigger = EXTI_Trigger_Falling;
	//		EXTI_Init(&EXTIInit);
			GPIOInit_t.GPIO_PuPd = GPIO_PuPd_UP;
			GPIO_Init(EXTI_GPIO,&GPIOInit_t);
			Elec_Struct.Back_Zero = TIM4->CNT;
			Elec_Struct.OP_Index = 1;
			Elec_Struct.Check_Index = 0;

		}
		else
		{
			step_num = TIM4->CNT;
			for( int suijishu = 0 ; suijishu < 65535 * 15 ; suijishu ++)
			{
				if(TIM4->CNT - step_num >= Elec_Struct.Zero_num)
					break;
			}
	  	TIM_Cmd(TIM3, DISABLE); // 关闭定时器 
		  TIM_Cmd(TIM4, DISABLE); // 关闭定时器 
		  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
			GPIOInit_t.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIO_Init(EXTI_GPIO,&GPIOInit_t);
//			EXTIInit.EXTI_Trigger = EXTI_Trigger_Rising;
//			EXTI_Init(&EXTIInit);
			Elec_Struct.Check_Index = 1;
			Elec_Struct.Back_Zero = 0;
		  EXTI->IMR &= ~(EXTI_Line5);
			if(Elec_Struct.Zero_num == 12)
			{
				Elec_Struct.Begin_Num -= TIM4->CNT;
				Elec_Struct.Zero_Index	 =  Elec_Struct.Begin_Num;
				FrePeriStruct.Max_Fre = 3000;
				FrePeriStruct.Min_Fre = 1500;
				Calcu_Config(&FrePeriStruct);
			}
			else
			{
				Elec_Struct.Begin_Num += TIM4->CNT;
			xQueueGiveFromISR(exti_limit,NULL);
			}

		}
		EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE3上的中断标志位
	}
}
