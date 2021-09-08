#include "inspection_in.h"

#define  inspection_pri			7
#define  inspection_name		"insphandle"
#define  inspection_space		2400
xTaskHandle inspection_handler;
void inspection_fun(void *pvParamters);


portTickType xinspLastWakeTime;


void Init_Inspection(void)
{
	cameratoinspec = xQueueCreate(1,sizeof(uint8_t));
	xTaskCreate(
		inspection_fun,
		inspection_name,
		inspection_space,
		NULL,
		inspection_pri,
		&inspection_handler
	);	
}

	int uxHighWaterMark4;
int uxHighWaterMark5;
int uxHighWaterMark6;
//  rece_time 停止计数时间不对   ！ 需修改!
void inspection_fun(void *pvParamters)
{

	xinspLastWakeTime =  xTaskGetTickCount();
	double rece_time = 0;
	uint8_t rececamera = 0;
	uint8_t Over_Time = 0;
	uint8_t Camera_Cric[21];
	uint8_t Last_Camera_Cric[21];
	memset(Camera_Cric,0,sizeof(Camera_Cric));
	memset(Last_Camera_Cric,0,sizeof(Last_Camera_Cric));
	while(1)
	{
		 vTaskDelayUntil(&xinspLastWakeTime, 1000);	
		if(Pcr_State.slestate == SelfCheck)
		{
			Set_State(High_Pin,Runn_Bit2);
			Set_State(High_Pin,Idle_Bit1);
			Set_Light(IN_Slef_Check);
		}
		else if(Pcr_State.slestate == IDle)
		{
			Set_Light(IN_Idle);
			Set_State(Low_Pin,Runn_Bit2);
			Set_State(Low_Pin,Idle_Bit1); 

		}
		else if(Pcr_State.slestate == Work)
		{
				Set_Light(IN_Work);
				Set_State(High_Pin,Runn_Bit2);
				Set_State(High_Pin,Idle_Bit1);
		}
		xQueueReceive(cameratoinspec,&rececamera,0);
		xQueueReceive(workwaittime,&rece_time,0);
		xQueueReceive(Camera_Cric_Array,Camera_Cric,0);

	  if(Elec_Struct.OP_Index)
		{
			if(Elec_Struct.Back_Zero && TIM4->CNT - Elec_Struct.Back_Zero >= 260)
			{	
			TIM_Cmd(TIM3, DISABLE); // 关闭定时器 
		  TIM_Cmd(TIM4, DISABLE); // 关闭定时器 
		  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
			EXTIInit.EXTI_Trigger = EXTI_Trigger_Rising;
			GPIOInit_t.GPIO_PuPd = GPIO_PuPd_DOWN;
			GPIO_Init(GPIOC,&GPIOInit_t);
			EXTI_Init(&EXTIInit);
			Elec_Struct.Check_Index = 1;
		  EXTI->IMR &= ~(EXTI_Line5);
			Elec_Struct.Begin_Num += TIM4->CNT;
			Elec_Struct.Back_Zero = 0;
			Elec_Struct.Zero_Wait_Time = 1;		
			}
			if(xQueueSemaphoreTake(exti_limit,0) == pdTRUE ||  Elec_Struct.Zero_Wait_Time  == 1)
			{
				FrePeriStruct.Max_Fre = 300;
				FrePeriStruct.Min_Fre = 300;
				Calcu_Config(&FrePeriStruct);
				EXTI->IMR |= EXTI_Line5;
				Elec_Struct.Zero_num = 12;
				vTaskDelay(500);
				Set_GPIO(Low_Pin,STEP_DIR);
				IncSpeed(8400);
				Elec_Struct.OP_Index = 0;
				Elec_Struct.Zero_Wait_Time = 0;
			}
		}
		if( rece_time )
		{
			rece_time -= 1.075;
			if(rece_time < 0)
			{
				rece_time = 0;
				xQueueGenericSend(waittotimeindex,NULL, semGIVE_BLOCK_TIME, queueSEND_TO_BACK);
			}
		}
		
		if(strcmp((char *)Camera_Cric,(char *)Last_Camera_Cric) != 0)
		{
			strcpy((char *)Last_Camera_Cric,(char *)Camera_Cric);
			Over_Time = 0;
		}
		if(rececamera != '1')
		{
				Over_Time = 0;
				Inspection_Fun();
		}
		else
		{
	  	double Top_temper = Read_Pin_Volt(TEMP_TOP_HEAT);
			Compare_Small(Top_temper,CommAnaly.Rg_Temper);
			Over_Time++;
			if (Over_Time >= Over_Time_MAX)
			{
				USART1_DMA_Send(Camera_Cric,strlen((char *)Camera_Cric));
				Over_Time = 0;		
			}
		}
	}
}
