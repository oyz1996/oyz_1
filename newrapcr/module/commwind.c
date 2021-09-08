#include "commwind_in.h"

#define CommWind_PRI			9
#define CommWind_Space		2048
#define CommWind_Name			"CommWindHand"
xTaskHandle  CommWind_Handler;
void CommWind(void *pvParamters);

struct commdata  commanalywtb;

PID_Set PID_Ch;

QueueHandle_t windconectboard;
QueueHandle_t analywindover;
QueueHandle_t Sign_Camera;

static void CommWinProceHook(void * addr,uint16_t len)
{

		struct USART_RcvDMADataAddr * temaddr = addr;
	if(temaddr->len)
	{
		memset(commanalywtb.addr,0,sizeof(commanalywtb.addr));
	}
	
	memcpy(commanalywtb.addr, temaddr->addr, temaddr->len);
	commanalywtb.len = temaddr->len + temaddr->len1;
	if (temaddr->len1)
		memcpy(commanalywtb.addr + temaddr->len, temaddr->addr1, temaddr->len1);

		if(((uint8_t *)commanalywtb.addr)[0] == 'A' && ((uint8_t *)commanalywtb.addr)[commanalywtb.len-1] == 'B')
	{	
		xQueueGiveFromISR(windconectboard,NULL);	
	}
	else
	{
		memset(commanalywtb.addr,0,temaddr->len + temaddr->len1);
		commanalywtb.len = 0;
	}
	
}
static void Init_Standtask(void)
{
	memset(&CommAnaly,0,sizeof(CommAnaly));
	memset(&Elec_Struct,0,sizeof(Elec_Struct));
	memset(&(Boardack.Bit2_State),0,sizeof(Boardack.Bit2_State));
	memset(&(Boardack.Bit3_State),0,sizeof(Boardack.Bit3_State));
	memset(&(Boardack.NowCount),0,sizeof(Boardack.NowCount));
	memset(&Indexstate,0,sizeof(Indexstate));
	
}
static void Fan_Co_Init(uint8_t cyc, uint32_t arr)
{
	TEC_Fan_Str.Fin_Temper = 0;
	TEC_Fan_Str.Pelti_Temper = 0;
	TEC_Fan_Str.Set_Temper = 0;
	TEC_Fan_Str.Last_Cycle = cyc;
	TEC_Fan_Str.Tec_Fan_Arr = arr;
	TEC_Fan_Str.T_S_Temper_Diff = 0;
	TEC_Fan_Str.F_S_Temper_Diff = 0;
	TEC_Fan_Str.T_S_Ch_Dir = Keep;
	TEC_Fan_Str.F_S_Ch_Dir = Keep;
}
static void Check_Step(int num_step)
{	

	do
	{
	Elec_Struct.Begin_Num = 0;
	Elec_Struct.Zero_Index = 0;
	EXTI->IMR &= ~(EXTI_Line5);
  setFilter(-2815);
	vTaskDelay(500);
	setFilter(-2840);
	vTaskDelay(500);
	setFilter(-2840);
	vTaskDelay(500);
	setFilter(-2840);
	vTaskDelay(500);
	setFilter(-2840);
  vTaskDelay(500);
	Strat_Check();
	while(Elec_Struct.Zero_Index == 0 );
	vTaskDelay(2000);
	}while( abs((int)(Elec_Struct.Begin_Num - 11420)) >= 5);
}
static void workstarthander(void * data, int para)
{
	xQueueSend (cameratoinspec,"1",1500);
	
	Pcr_State.slestate = SelfCheck;

	Open_TEC_Fan(TIM1_25KHZ_ARR,Defalut_TEC_FAN_Init_CCR,Defalut_TEC_FAN_End_CCR);
	Fan_Co_Init(Defalut_TEC_FAN_End_CCR,TIM1_25KHZ_ARR);
	Open_Back_Fan(TIM9_25KHZ_ARR,Defalut_Back_FAN_Init_CCR,Defalut_Back_FAN_End_CCR);
	
	BoardConHeatDish_UP();
	Check_Step(1);	
	BoardConHeatDish_DOWN();
	Close_PR59();
	Set_Ntc_Mode();
	Set_GPIO(Low_Pin,VLAMP_EN);
	Set_GPIO(Low_Pin,SIDE_HEATER_EN);
	Pcr_State.slestate = IDle;
	
  xQueueSend (cameratoinspec,"0",1500);
}

static void workstophander(void * data, int para)
{
		double rece_rim_zero = 0;
		Init_Standtask();
		Close_PR59();
		BoardConHeatDish_DOWN();
		Set_GPIO(Low_Pin,VLAMP_EN);
		Set_GPIO(Low_Pin,SIDE_HEATER_EN);
		Pcr_State.slestate = IDle;
		CommAnaly.Rg_Temper = 105;
		Open_TEC_Fan(TIM1_25KHZ_ARR,Defalut_TEC_FAN_Init_CCR,Defalut_TEC_FAN_End_CCR);
		xQueueSend(workwaittime,&rece_rim_zero,0);
}

static void WinToBoradStopHook(void * addr,uint16_t len)
{
		New_Task();
		if(work_init.workstop != NULL)
			work_init.workstop(NULL,0);
	
	  xQueueSend (cameratoinspec,"0",1500);

}
static void WinToBoradStartHook(void * addr, uint16_t len)
{
		BoardConHeatDish_UP();
		Pcr_State.slestate = Work;
		Set_GPIO(High_Pin,VLAMP_EN);
		Init_Standtask();
		Check_Step(2);
		Anasysis_Fun(addr);
	  USART1_DMA_Send(APCommAck,APCommAck_Len);
		xQueueSend(cameratoinspec,"0",1500);
		xQueueGenericSend(analywindover,NULL,semGIVE_BLOCK_TIME,queueSEND_TO_BACK);

}
int iii = -1;
#include <ctype.h>
static void WinToBoradEnginHook(void * addr, uint16_t len)
{
	#define  array_length  7
	char * Logic_addr = addr;
	static uint8_t Origin[array_length][5] = {"AGL","AGM","AGI","AGT","AGD","AGR","AGS"};
	uint8_t index_orign = 250;
	for(int i = 0; i < array_length;i++)
	{
			if(strncmp(addr,(char *)Origin[i],3) == 0)
			{
				 index_orign = i;
				break;
			}
	}
	if(index_orign >= array_length)
		return ;
	if(index_orign == 0 )
	{
		Set_GPIO(atoi(&Logic_addr[3]),VLAMP_EN);
	}
	else if(index_orign == 1)
	{
		((atoi(&Logic_addr[3]))) ? BoardConHeatDish_UP() : 	BoardConHeatDish_DOWN();
	}
	else if(index_orign == 2)
	{

		static int last_fil_num = 0; 
		int src_num = -2840;

		if(atoi(&Logic_addr[3]) == 0)
		{
			Elec_Struct.Begin_Num = 0;
			Elec_Struct.Zero_Index = 0;
			Strat_Check();
			vTaskDelay(5000);	
			last_fil_num = 0;
		}
		else
		{
		int filter_num = ( atoi(&Logic_addr[3]) - last_fil_num );
		int Abs_filter = abs(filter_num);
			if(atoi(&Logic_addr[3]) == 1 )
			{
				if(Elec_Struct.Begin_Num)
					src_num = - (2815 - ( Elec_Struct.Begin_Num - 11420 ));
				else
					src_num = -2815;
			}
				EXTI->IMR &= ~(EXTI_Line5);		
		for(int i = 0 ; i < Abs_filter ; i++)
		{
			if(filter_num < 0)
				setFilter(-src_num);
			else
				setFilter(+src_num);
			src_num = -2840;
			vTaskDelay(1000);
		}
		last_fil_num = atoi(&Logic_addr[3]);
		}
	}
	else if(index_orign == 3)
	{
		double wendu_list = 0;
		double cal_linmit = 0;
		char temp_ch[20] = "0";
		wendu_list = Read_Temper();
		strncpy(temp_ch,&Logic_addr[3],len - 4);
		double kp = 11.6;
		double ki = 1.4;
		double kd = 4.875;
		Rise_Fall_Temp_PID(atoi(temp_ch),wendu_list,&kp,&ki,&kd,&cal_linmit);
		double nowr6 = 100;
		if(atoi(temp_ch) == 0)
			Close_PR59();
			else
		Control_Tec(atoi(temp_ch),kp,ki,kd,nowr6,cal_linmit);
	}
		else if(index_orign == 4)
	{
		int indexstart = 0;
		int indexend = 0;
		char temp_ch[20] = "0";
		char doubl_ch[10] = "0";
		double rece_pid[3]={0,0,0};
		int rece_num = 0;
		strncpy(temp_ch,Logic_addr,len);
		for(int i = 0 ; i< len ;i++)
		{
			if(isdigit(temp_ch[i]) )
			{
				if(!indexstart)
				{
					indexstart = i;
				}
			}
				else if(temp_ch[i] == 'E' || temp_ch[i] == 'B')
				{
					 indexend= i;
						strncpy(doubl_ch,&temp_ch[indexstart],indexend - indexstart);
						rece_pid[rece_num++] = atof(doubl_ch);
						memset(doubl_ch,0,sizeof(doubl_ch));
					  indexstart = 0;
					  indexend = 0;	
				}
		}
		PID_Ch.para_kp = rece_pid[0];
		PID_Ch.para_ki = rece_pid[1];
		PID_Ch.para_kd = rece_pid[2];
	}
	else if(index_orign == 5)
	{
		Device_Reset();
	}
	else if(index_orign == 6)
	{
		
		uint8_t Board_Head = 3;
		uint8_t Board_End = 1;
		uint8_t SN_Data[18] = "AGS";
		uint32_t SN_Addr = SN_Data_ADDR;

		for(int j = Board_Head ; j < SN_Data_Length + Board_Head ; j++,SN_Addr++)
		{
			SN_Data[j] = *(volatile uint32_t *)SN_Addr;
		}
		SN_Data[SN_Data_Length + Board_Head] = 'B';
		USART1_DMA_Send(SN_Data,SN_Data_Length + Board_Head + Board_End);
	}
	

}
static void WinToBoradCameraHook(void * addr, uint16_t len)
{
	xQueueGenericSend(Sign_Camera,NULL,semGIVE_BLOCK_TIME,queueSEND_TO_BACK);
}

void (* const processwtb[])(void *,uint16_t) =
{
	WinToBoradStopHook,
	WinToBoradStartHook,
	WinToBoradCameraHook,
	WinToBoradEnginHook,
};


static uint8_t Analy_windtoboard(void * data)
{
	uint8_t * temppoint = (uint8_t * )data;
	const uint8_t Head_Array[Head_Array_row][Head_Array_col]={"ASB","AP","AENDB","AG"};
	for(int i = 0 ; i< Head_Array_row; i++)
	{
		if(strncmp((char *)Head_Array[i],(char *)temppoint,2) == 0)
		{
			return i;
		}
	}
	return Head_Array_row;
}

static void All_Analy_WTB(void *data,uint16_t len)
{
	uint8_t  analyretu = Head_Array_row;
	analyretu = Analy_windtoboard(data);
	if(analyretu < Head_Array_row)
	{
		processwtb[analyretu](data,len);
	}
}

void  Init_CommWind(void)
{
	BaseType_t RETU_INT = 0;
	COMM_WIND_HOOK.receiveProcess = CommWinProceHook;
	work_init.workstart = workstarthander;
	work_init.workstop = workstophander;
	RETU_INT =	xTaskCreate(
				  CommWind,
				  CommWind_Name,
				  CommWind_Space,
				  NULL,
				  CommWind_PRI,
				  &CommWind_Handler);
	if(RETU_INT != pdPASS )
		Set_Light(IN_Warring);
	windconectboard = xQueueGenericCreate(1,semSEMAPHORE_QUEUE_ITEM_LENGTH,queueQUEUE_TYPE_BINARY_SEMAPHORE);
	analywindover = xQueueGenericCreate(1,semSEMAPHORE_QUEUE_ITEM_LENGTH,queueQUEUE_TYPE_BINARY_SEMAPHORE);
	Sign_Camera = xQueueGenericCreate(1,semSEMAPHORE_QUEUE_ITEM_LENGTH,queueQUEUE_TYPE_BINARY_SEMAPHORE);
}

void CommWind(void *pvParamters)
{


	if(work_init.workstart != NULL)
	{
		CommAnaly.Rg_Temper = 105;
		work_init.workstart(NULL,0);
	}

	while(1)
	{
		xQueueSemaphoreTake(windconectboard,portMAX_DELAY);
		All_Analy_WTB(commanalywtb.addr,commanalywtb.len);
	}
	
}

