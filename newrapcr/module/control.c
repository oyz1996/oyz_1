#include "control_in.h"

static struct  usart_pr59rece pr59receive;
 


static void pr59hook(void * addr,uint16_t idleindex)
{
	struct USART_RcvDMADataAddr * temaddr = addr;
	if(temaddr->len)
	{
		memset(pr59receive.addr,0,sizeof(pr59receive.addr));
	}
	
	memcpy(pr59receive.addr, temaddr->addr, temaddr->len);
	
	if (temaddr->len1)
		memcpy(pr59receive.addr + temaddr->len, temaddr->addr1, temaddr->len1);
	
	pr59receive.len = temaddr->len + temaddr->len1;
	pr59receive.idleindex = idleindex;
	
}
void Init_Control(void)
{
	usart2hook.prtmprocess = pr59hook;
}

void Close_PR59(void)
{
		char Over_Comm[30]="$Q\r";
		memset(&pr59receive,0,sizeof(pr59receive));
		USART2_DMA_Send(Over_Comm,strlen(Over_Comm));
		while(!(pr59receive.idleindex))
	{
		pr59receive.wait_time ++;
		vTaskDelay(PR59_Wait_Time);
		if(pr59receive.wait_time >= PR59_Wait_Min_Time)
			break;
	};
}

double Read_Temper(void)
{
	char Temp_Data[30] = "$RN100?\r";
	double temperat = Errortemp;


	for(int i = 0; i < collnum ; i++)
	{
	memset(&pr59receive,0,sizeof(pr59receive));
	USART2_DMA_Send(Temp_Data,strlen(Temp_Data));

	while(!(pr59receive.idleindex))
	{
		pr59receive.wait_time ++;
		vTaskDelay(PR59_Wait_Time);
		if(pr59receive.wait_time >= PR59_Wait_Min_Time)
			break;
	};

	if(pr59receive.len == RN100_Len)
			temperat=Calc_Temp(pr59receive.addr,Errortemp);

		if(temperat != Errortemp)
			break;

	}
	if(temperat > 0 && temperat < 150)
		return temperat;
	else
		return 0;

}

int Control_Tec(double Temper,double kp,double ki,double kd,double Nr6,double value_limit)
{
	static double Left_kp = 0;
	static double Left_ki = 0;
	static double Left_kd = 0;
	static double Left_R6 = 0;
	char Send_Data[50];
	char Tem_Data[50];
	const  char R0_Data[30]="$R0=";
	const  char R1_Data[30]="$R1=";
	const  char R2_Data[30]="$R2=";
	const  char R3_Data[30]="$R3=";
	char R4_Data[30]="$R4=1\r";
	char R5_Data[30]="$R5=1\r";
	const  char R6_Data[30]="$R6=";
	

	char R8_Data[30]="$R8=";
	char R8_End[30];
	char Over_Comm[30]="$W\r";
	char Save_Comm[30]="$RW\r";

	memset(R8_End,0,sizeof(R8_End));
	sprintf(R8_End,"%.4f",value_limit);
		strcat(R8_Data,R8_End);


	memset(Tem_Data,0,sizeof (Tem_Data));
	memset(Send_Data,0,sizeof (Send_Data));

	if(Left_kp != kp || Left_ki != ki || Left_kd != kd)
	{
	strcat(Send_Data,R1_Data);
	sprintf(Tem_Data,"%.4f\r",(double)kp);
	strcat(Send_Data,Tem_Data);
		

	USART2_DMA_Send(Send_Data,strlen(Send_Data));

	memset(Tem_Data,0,sizeof (Tem_Data));
	memset(Send_Data,0,sizeof (Send_Data));
	strcat(Send_Data,R2_Data);
	sprintf(Tem_Data,"%.4f\r",(double)ki);
	strcat(Send_Data,Tem_Data);
		

	USART2_DMA_Send(Send_Data,strlen(Send_Data));

	memset(Tem_Data,0,sizeof (Tem_Data));
	memset(Send_Data,0,sizeof (Send_Data));
	strcat(Send_Data,R3_Data);
	sprintf(Tem_Data,"%.4f\r",(double)kd);
	strcat(Send_Data,Tem_Data);


	USART2_DMA_Send(Send_Data,strlen(Send_Data));


	memset(Tem_Data,0,sizeof (Tem_Data));
	memset(Send_Data,0,sizeof (Send_Data));

	USART2_DMA_Send(R4_Data,strlen(R4_Data));
	USART2_DMA_Send(R5_Data,strlen(R5_Data));
	USART2_DMA_Send(R8_Data,strlen(R8_Data));

	USART2_DMA_Send(Save_Comm,strlen(Save_Comm));
	Left_kp = kp;
	Left_ki = ki;
	Left_kd = kd;
	}
	
	if(Left_R6 != Nr6)
	{
		strcat(Send_Data,R6_Data);
		sprintf(Tem_Data,"%.4f\r",(double)Nr6);
		strcat(Send_Data,Tem_Data);
		
		USART2_DMA_Send(Send_Data,strlen(Send_Data));

		memset(Send_Data,0,sizeof (Send_Data));

		USART2_DMA_Send(Save_Comm,strlen(Save_Comm));
		Left_R6 = Nr6;
	}
	
	strcat(Send_Data,R0_Data);
	sprintf(Tem_Data,"%.4f\r",Temper);
	strcat(Send_Data,Tem_Data);

	USART2_DMA_Send(Send_Data,strlen(Send_Data));

	memset(Tem_Data,0,sizeof (Tem_Data));
	memset(Send_Data,0,sizeof (Send_Data));
	
	USART2_DMA_Send(Over_Comm,strlen(Over_Comm));
	
	return 0;
}
int Stable_Temp(int row,int arr,double temper)
{
    char UART3_Data[100];
    double Coll_Temp[10];
    double temp_midd=0;
    int i=0;
    int count=0;
    double temperat = Errortemp;
    memset(UART3_Data,0,sizeof(UART3_Data));
    memset(Coll_Temp,0,sizeof(Coll_Temp));

    if(temper)
    {
    	temp_midd = PrToNTC(temper);
    	Temp_Diff = 0.06;
    }
    else
   {
   	temp_midd= PrToNTC(CommAnaly.Brain_Array[row][arr]);
    	Temp_Diff = 0.5;
   }

    while(1)
    {
    for(i=0;i<5;i++)
    {
	
			xQueueReceive(worktoinspec,&temperat,3000);
 //   	temperat=Read_Temper();

	//		xQueueSend(worktoinspec,&temperat,900);
	//			temperat = NTCtoPr(temperat);
    	if(fabs(temperat-temp_midd) >= Temp_Diff)
    	{
    		i = -1;
    	}
    	count++;	
//			vTaskDelay(200);
    	if(count>60)
    	{
    		count=0;
    		return 1;
    	}
    }
    break;
    }
    return 0;
}

void pr59_error_handler(void)
{
	char reset_regula[10] = "$BC\n";
	USART2_DMA_Send(reset_regula,strlen(reset_regula));
}

void Set_Ntc_Mode(void)
{
	double R92_Result = Errortemp;


	char Search_R92[10]="$R92?\r";
	char RN0_Data[20] = "$RN0=41C80000\r";
	char RN4_Data[20] = "$RN4=3F800000\r";
	char RN5_Data[20] = "$RN5=3F800000\r";
	char RN7_Data[20] = "$RN7=3F800000\r";
	char RN8_Data[20] = "$RN8=428C0000\r";
	char RN55_Data[20] = "$R55=12\r";
	char RN35_Data[20] = "$R35=1\r";
	char RN36_Data[20] = "$R36=0\r";
	char RN59_Data[20] = "$RN59=3A58D013\r";
	char RN60_Data[20] = "$RN60=39893BAE\r";
	char RN61_Data[20] = "$RN61=341F9539\r";
	//char RN79_Data[20] = "$R79=1.462E3\r";
	//char RN80_Data[20] = "$R80=4.163E3\r";
	//char RN81_Data[20] = "$R81=1.0E4\r";
	char RN71_Data[20] = "$RN71=42F00000\r";
	char RN47_Data[20] = "$RN47=41F00000\r";
	char R92_Data[20] = "$R92=3\r";
	char PID_Mode[10] = "$R13=6\r";
	char Save_Comm[10] = "$RW\r";
	
	memset(&pr59receive,0,sizeof(pr59receive));
	USART2_DMA_Send(Search_R92,strlen(Search_R92));
	while(!(pr59receive.idleindex))
	{
		pr59receive.wait_time ++;
		vTaskDelay(PR59_Wait_Time);
		if(pr59receive.wait_time >= PR59_Wait_Min_Time)
			break;
	};
	char * Seek_car = strchr((char *)pr59receive.addr,'\n');
	for(int i = 0 ; Seek_car[i] != '\0' ; i++)
	{
		if(Seek_car[i] == '\r')
				break;	
		if((Seek_car[i] >= '0' && Seek_car[i] <= '9') || R92_Result != Errortemp)
		{
				R92_Result = R92_Result * 10;
				R92_Result +=   Seek_car[i] - 0x30;
		}
	
	}

	if(R92_Result != R92_Set)
	{
	USART2_DMA_Send(RN0_Data,strlen(RN0_Data));
	USART2_DMA_Send(RN4_Data,strlen(RN4_Data));
	USART2_DMA_Send(RN5_Data,strlen(RN5_Data));
	USART2_DMA_Send(RN7_Data,strlen(RN7_Data));
	USART2_DMA_Send(RN8_Data,strlen(RN8_Data));
	USART2_DMA_Send(RN55_Data,strlen(RN55_Data));
	USART2_DMA_Send(RN35_Data,strlen(RN35_Data));
	USART2_DMA_Send(RN36_Data,strlen(RN36_Data));
	USART2_DMA_Send(RN59_Data,strlen(RN59_Data));
	USART2_DMA_Send(RN60_Data,strlen(RN60_Data));
	USART2_DMA_Send(RN61_Data,strlen(RN61_Data));
//	USART2_DMA_Send(RN79_Data,strlen(RN79_Data));
//	USART2_DMA_Send(RN80_Data,strlen(RN80_Data));
//	USART2_DMA_Send(RN81_Data,strlen(RN81_Data));
	USART2_DMA_Send(RN47_Data,strlen(RN47_Data));
	USART2_DMA_Send(RN71_Data,strlen(RN71_Data));
	USART2_DMA_Send(R92_Data,strlen(R92_Data));
	USART2_DMA_Send(PID_Mode,strlen(PID_Mode));	 
	USART2_DMA_Send(Save_Comm,strlen(Save_Comm));	
	}
	
	
}
