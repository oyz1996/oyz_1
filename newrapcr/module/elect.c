#include "elect_in.h"

Elec_Index Elec_Struct = {0,0,0,0,0,0,0,0,0,0};
QueueHandle_t		tim_step;
QueueHandle_t		exti_limit;

FreSpeed  FreStruct;
static void TIM4_Hook_Handler(uint32_t tim_num)
{
	xQueueGiveFromISR(tim_step,NULL);
}

static void EXTI_Hook_Handler(uint32_t exti_num)
{
	xQueueGiveFromISR(exti_limit,NULL);
}

void Set_State(uint8_t mode,enum Bit_Sta comSta)
{
	if(mode == Low_Pin)
	{
		Boardack.Bit1_State&=(~comSta);
	}
	else
	{
		Boardack.Bit1_State|=comSta;
	}
}
void Set_Bit3_State(uint8_t mode,enum Bit3_Sta comSta)
{
	if(mode == Low_Pin)
	{
		Boardack.Bit3_State&=(~comSta);
	}
	else
	{
		Boardack.Bit3_State|=comSta;
	}
}
void Set_Light(enum Work_Sta myWork)
{
   
   static uint8_t nowstate = IN_Init;
   
   if(nowstate != myWork)
   {
      switch(myWork)
      {
        case IN_Idle :
				Set_GPIO(High_Pin,LED_G_EN);
				Set_GPIO(Low_Pin,LED_Y_EN);
				Set_GPIO(Low_Pin,LED_R_EN);
				
		
        break;
        case IN_Low_SIDE:
				I2C1_Send(Low_I2C_Regi);			
				break;
        case IN_High_SIDE:
				I2C1_Send(High_I2C_Regi);
				break;
        case IN_Work:
				Togg_GPIO(LED_G_EN);
				Set_GPIO(Low_Pin,LED_Y_EN);
				Set_GPIO(Low_Pin,LED_R_EN);
						
				break;
				case IN_Slef_Check:
				Set_GPIO(Low_Pin,LED_G_EN);
				Togg_GPIO(LED_Y_EN);
				Set_GPIO(Low_Pin,LED_R_EN);
				break;
        case IN_Warring:
				Set_GPIO(Low_Pin,LED_G_EN);
				Set_GPIO(High_Pin,LED_Y_EN);
				Set_GPIO(Low_Pin,LED_R_EN);
				break;
        case IN_Hard_Error:
				Set_GPIO(Low_Pin,LED_G_EN);
				Set_GPIO(Low_Pin,LED_Y_EN);
				Togg_GPIO(LED_R_EN);
				break;
        case IN_Memory_Error:
				Set_GPIO(Low_Pin,LED_G_EN);
				Set_GPIO(Low_Pin,LED_Y_EN);
				Set_GPIO(High_Pin,LED_R_EN);
				
				Set_GPIO(Low_Pin,VLAMP_EN);
				Set_GPIO(Low_Pin,TOP_HEATER_EN);
				Set_GPIO(Low_Pin,SIDE_HEATER_EN);
				break;       
      }
   }
}

void IncSpeed(uint32_t Slave_Pulse)
{
	FreStruct.SpeedStruct = IncSpeedState;
	FreStruct.IncNum = Slave_Pulse;
	FreStruct.SurNum = Slave_Pulse;
	FrePeriStruct.curnum = 0;
	Elect_Output(FreStruct.IncNum);
}
void DecSpeed(uint32_t Slave_Pulse)
{
	FreStruct.SpeedStruct = DecelSpeedState;
	FreStruct.IncNum = Slave_Pulse;
	Elect_Output(FreStruct.IncNum);
}
void AvrSpeed(uint32_t Slave_Pulse)
{
	FreStruct.SpeedStruct = AvrFlag;
	FreStruct.IncNum = Slave_Pulse;
	Elect_Output(FreStruct.IncNum);
}
void Elect_Output(uint32_t Slave_Pulse)
{
	if(Slave_Pulse == abs(Elect_MAX_Step) ||Slave_Pulse == Elect_EXIT_Step)
	{
		TIM4_Config(40000);
		TIM_Cmd(TIM3,ENABLE);
		TIM_Cmd(TIM4, ENABLE);
	}
	else
	{
	TIM4_Config(Slave_Pulse);
	TIM_Cmd(TIM4, ENABLE);
  TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);

	//TIM3_Config(TIM3_20KHZ_PSC,TIM3_20KHZ_ARR);
	TIM_Cmd(TIM3,ENABLE);
	}
}
int okiu = -1;

int Strat_Check(void)
{
	Elec_Struct.Check_Index = 1;
	Elec_Struct.Zero_num = 160 ;
	FrePeriStruct.Max_Fre = 1500;
	FrePeriStruct.Min_Fre = 1500;
	Calcu_Config(&FrePeriStruct);
	Open_EXTI_5;
	int Rece_Elect = 0 ;
	Set_GPIO(Low_Pin,STEP_DIR);
	Set_GPIO(Low_Pin,STEP_EN);
	Rece_Elect = setFilter(Elect_MAX_Step);
//	xQueueSemaphoreTake(exti_limit,Elect_Wait_Time);
//	Close_EXTI_5;
		okiu = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5);
	return Rece_Elect;
}
uint32_t setFilter(int devia)
{
	int ABS_Num = 0;
	if(devia > 0)
		Set_GPIO(Low_Pin,STEP_DIR);
	else
		Set_GPIO(High_Pin,STEP_DIR);
	
	ABS_Num = abs(devia);
	IncSpeed(abs(ABS_Num));

	if (abs(devia) < abs(Elect_Spare_Step))
		xQueueSemaphoreTake(tim_step,portMAX_DELAY);
	return ABS_Num;
}
int up_index = 0;
int down_index = 0;
void BoardConHeatDish_UP(void)
{
		TIM_CCxCmd(TIM1,TIM_Channel_2, ENABLE);
		TIM_CCxNCmd(TIM1,TIM_Channel_2, ENABLE);
		while(!(Read_GPIO(OP_MODULE_UP)));
		TIM_CCxCmd(TIM1,TIM_Channel_2, DISABLE);
		TIM_CCxNCmd(TIM1,TIM_Channel_2, DISABLE);
		up_index = 1;
		down_index = 0;
}
void BoardConHeatDish_DOWN(void)
{
		TIM_CCxNCmd(TIM1,TIM_Channel_2, ENABLE);
		TIM_CCxCmd(TIM1,TIM_Channel_2, ENABLE);
		while(!(Read_GPIO(OP_MODULE_DOWN)));
		TIM_CCxCmd(TIM1,TIM_Channel_2, DISABLE);
		TIM_CCxNCmd(TIM1,TIM_Channel_2, DISABLE);
			up_index = 0;
		down_index = 1;
}

void Elec_Init(void)
{
	tim4hook.timhook = TIM4_Hook_Handler;
	exti5hook.extihook = EXTI_Hook_Handler;
	
	tim_step = xQueueGenericCreate(1,semSEMAPHORE_QUEUE_ITEM_LENGTH,queueQUEUE_TYPE_BINARY_SEMAPHORE);
	exti_limit = xQueueGenericCreate(1,semSEMAPHORE_QUEUE_ITEM_LENGTH,queueQUEUE_TYPE_BINARY_SEMAPHORE);
	
}

void Init_Speed(void)
{
	FreStruct.SpeedStruct = InitState;
	FreStruct.IncNum = 0;
	
	FreStruct.CurState = InitState;
}
