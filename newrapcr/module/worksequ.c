#include "worksequ_in.h"

#define WorkSequ_PRI			8
#define WorkSequ_Space		1400
#define WorkSequ_Name			"WorkSequHand"
xTaskHandle  WorkSequ_Handler;
work_hook  work_init;
void WorkSequ(void *pvParamters);


portTickType xLastWakeTime;

QueueHandle_t waittotimeindex;
xQueueHandle workwaittime;
xQueueHandle cameratoinspec;
xQueueHandle worktoinspec;
xQueueHandle RgTempinspec;
xQueueHandle Camera_Cric_Array;

void Delete_Task(void)
{
	if(WorkSequ_Handler != NULL)
		vTaskDelete(WorkSequ_Handler);
}
void New_Task(void)
{
	BaseType_t RETU_INT = 0;
	if(WorkSequ_Handler != NULL)
			vTaskDelete(WorkSequ_Handler);
	vTaskDelayUntil( &xLastWakeTime, 100 );
	RETU_INT = xTaskCreate(
				  WorkSequ,
				  WorkSequ_Name,
				  WorkSequ_Space,
				  NULL,
				  WorkSequ_PRI,
				  &WorkSequ_Handler);
	if( RETU_INT !=  pdPASS)
	{
		Set_Light(IN_Memory_Error);
	}
}

void Init_WorkSequ(void)
{
	xLastWakeTime = xTaskGetTickCount();
	
	work_init.workstart = NULL;
	work_init.workstop = NULL;
	
	worktoinspec = xQueueCreate(1,sizeof(double));
	RgTempinspec = xQueueCreate(1,sizeof(double));
	cameratoinspec = xQueueCreate(1,sizeof(uint8_t));
		workwaittime = xQueueCreate(1,sizeof(double));
	Camera_Cric_Array = xQueueCreate(1,sizeof(uint8_t) *20);
		waittotimeindex = xQueueGenericCreate(1,semSEMAPHORE_QUEUE_ITEM_LENGTH,queueQUEUE_TYPE_BINARY_SEMAPHORE);
			xTaskCreate(
				  WorkSequ,
				  WorkSequ_Name,
				  WorkSequ_Space,
				  NULL,
				  WorkSequ_PRI,
				  &WorkSequ_Handler);

}
 double PrToNTC(double prtemp)
{
	#if 1
							double Actual_Temper = 0;
						 if(prtemp > 0 && prtemp <= 10)
						 {
							 Actual_Temper = ((prtemp + Temper0_10_Intercept) / Temper0_10_Slope);
						 }
						 else if(prtemp > 10 && prtemp <50)
						 {
							 Actual_Temper = ((prtemp + Temper10_50_Intercept) / Temper10_50_Slope);
						 }
						 else if(prtemp >= 50 && prtemp <= 60)
						 {
							 Actual_Temper = ((prtemp + Temper50_60_Intercept) / Temper50_60_Slope);
						 }
						 else if(prtemp > 60 && prtemp < 65)
						 {
							 Actual_Temper = ((prtemp + Temper60_65_Intercept) / Temper60_65_Slope);
						 }
						 else if(prtemp >= 65 && prtemp <= 75)
						 {
							 Actual_Temper = ((prtemp + Temper65_75_Intercept) / Temper65_75_Slope);
						 }
						 else if(prtemp > 75 && prtemp < 90)
						 {
							 Actual_Temper = ((prtemp + Temper75_90_Intercept) / Temper75_90_Slope);
						 }
						 else if(prtemp >= 90 && prtemp <= 100)
						 {
							 Actual_Temper = ((prtemp - Temper90_100_Intercept) / Temper90_100_Slope);
						 }
						 else
						 {
							 Actual_Temper = prtemp;
						 }
						 return Actual_Temper;
		#else
						return prtemp;
		#endif
}


void Rise_Fall_Temp_PID(double runn_temp,double list_temp,double * kp, double * ki, double *kd , double * va_limit)
{
	if(runn_temp >= Min_Temp && runn_temp < Midd_Temp)
	{
		if(runn_temp >= list_temp || runn_temp < 30)
		{
			*kp = 0.0000123581781317346 * pow(runn_temp,4) -0.000797782613837295* pow(runn_temp,3) + 0.0159715115009015 *pow(runn_temp,2)  - 0.326070187316962  * pow(runn_temp,1) + 16.3377515555994;
			*ki = 0.0000129850340665379 * pow(runn_temp,4) -0.000944512248391496 * pow(runn_temp,3) +0.017108149737145 *pow(runn_temp,2)  -0.0168614292383726 * pow(runn_temp,1) +1.27246122639534  ;
			*kd = -0.0000121413210086713 * pow(runn_temp,4)  +0.0012179938532391 * pow(runn_temp,3) -0.032726328672667 *pow(runn_temp,2)  +0.111313978042699 * pow(runn_temp,1) +31.0629124436874;
			*va_limit = 100;
		}
		else
		{
			*kp = 0.0000269607843137238*pow(runn_temp,4)-0.00219607843137239*pow(runn_temp,3)+0.0543627450980345*pow(runn_temp,2)-0.69803921568622*pow(runn_temp,1)+17.4705882352939;
			*ki = 0.000113270308123249*pow(runn_temp,4)-0.00982703081232494*pow(runn_temp,3)+0.271446078431373*pow(runn_temp,2)-2.56351540616247*pow(runn_temp,1)+9.18487394957984;
			*kd = -0.000165616246498595*pow(runn_temp,4)+0.0153949579831929*pow(runn_temp,3)-0.459656862745087*pow(runn_temp,2)+4.49747899159651*pow(runn_temp,1)+17.2521008403366;
			*va_limit = 0.0000311624649859955*pow(runn_temp,4)+0.00455042016806716*pow(runn_temp,3)-0.420931372549019*pow(runn_temp,2)+5.77521008403362*pow(runn_temp,1)+79.4789915966386;			 
		}
	}
	else if(runn_temp >= Midd_Temp && runn_temp <= MAX_Temp)
	{
		if(runn_temp >= list_temp)
		{
				*kp = 1.20611E-08*pow(runn_temp,6)-4.72882E-06*pow(runn_temp,5)+0.0007529846079854520*pow(runn_temp,4)	\
						 -0.062222456710031500*pow(runn_temp,3)+2.811474867728530*pow(runn_temp,2)-65.77970851380210*pow(runn_temp,1)+627.2545454555230;
				*ki = 3.59764309764639E-08*pow(runn_temp,6)-0.000014239141414155*pow(runn_temp,5)+0.0022964436026959*pow(runn_temp,4)	\
						 -0.192853409091113*pow(runn_temp,3)+8.88105648149149*pow(runn_temp,2)-212.3398434346*pow(runn_temp,1)+2058.85454545722;
				*kd = -5.9475709475744E-08*pow(runn_temp,6)+0.0000231121933622076*pow(runn_temp,5)-0.00368473184223427*pow(runn_temp,4)	\
						 +0.308864610389826*pow(runn_temp,3)-14.3688108465714*pow(runn_temp,2)+351.767265512538*pow(runn_temp,1) -3518.09090909376;
				*va_limit = -5.32227032227526E-08*pow(runn_temp,6)+0.0000215068542568747*pow(runn_temp,5)-0.00356808561808909*pow(runn_temp,4)	\
											+0.310842532467841*pow(runn_temp,3)-14.9797804232956*pow(runn_temp,2)+378.465829726219*pow(runn_temp,1)-3909.90909091316;	
		}
		else
		{
			*kp = 0.0000000120610870611*pow(runn_temp,6)-4.72882395382895E-06*pow(runn_temp,5)+0.000752984607985452*pow(runn_temp,4)	\
					 -0.0622224567100315*pow(runn_temp,3)+2.81147486772853*pow(runn_temp,2)-65.7797085138021*pow(runn_temp,1)+627.254545455523;
			*ki =-4.7402597402601E-09*pow(runn_temp,6)+2.64036796536808E-06*pow(runn_temp,5)-0.00056209956709958*pow(runn_temp,4)	\
					+0.0599949675324682*pow(runn_temp,3)-3.4267880952381*pow(runn_temp,2)+99.9272813852809*pow(runn_temp,1)-1163.8909090909;
			*kd = -1.18614718614816E-07*pow(runn_temp,6)+0.0000475930735931138*pow(runn_temp,5)-0.00782175324676005*pow(runn_temp,4)	\
					+0.673673160173763*pow(runn_temp,3)-32.0575476190771*pow(runn_temp,2)+798.820389611145*pow(runn_temp,1)-8117.81818182608;				
			*va_limit = -6.13756613757233E-08*pow(runn_temp,6)+0.0000236031746032003*pow(runn_temp,5)-0.00369431216931652*pow(runn_temp,4)	\
									+0.300642857143245*pow(runn_temp,3)-13.3816216931407*pow(runn_temp,2)+308.183015873504*pow(runn_temp,1)-2858.00000000512;			
		}
	}
	else
	{
		*kp = Normal_Kp;
		*ki = Normal_Ki;
		*kd = Normal_Kd;
		*va_limit = 100;	
	}
	
	if(*va_limit > 100)
	{
		*va_limit = 100;
	}
	else if(*va_limit <= 0 )
	{
		*va_limit = 1;
	}
	
}
void WorkSequ(void *pvParamters)
{
	while(1)
	{
	xQueueSemaphoreTake(analywindover,portMAX_DELAY);
	unsigned char  i,j,k;
	unsigned char  bitnum=1;

	double kp = Normal_Kp;
	double ki = Normal_Ki;
	double kd = Normal_Kd;
	double nowr6 = Normal_R6;
	double solur6 = Dislve_R6;
	double Divi=0;
  double rece_rgtemp;
	static double list_temper = 0;
	double value_limit = 0;
	int devianum=0;
	int copybit=0;
	int num=1;
	int reIndex=0;
	int CycCount=0;
	int allCycNum=0;
	unsigned char Bithe=1;
	char   CameraTemp[10];
	char   CameraIndex[50];

	memset(CameraTemp,0,sizeof(CameraTemp));
	memset(CameraIndex,0,sizeof(CameraIndex));
	xQueueReceive(worktoinspec,&list_temper,5000);	

	do
	{
		xQueueReceive(RgTempinspec,&rece_rgtemp,5000);
	}while(0);
  vTaskDelay(10000);
	
	
  Elec_Struct.One_Step = 1;
	Elec_Struct.One_Step_Num = 0;
	Elec_Struct.Zero_Index	 =  Elec_Struct.Begin_Num;
//	Elec_Struct.Begin_Num = 11355;
	
	for(i=0;i<CommAnaly.Brain_Num;i++)
	  {
		CommAnaly.ALL_Cycles[i]=0;
		CommAnaly.Curr_Cycles=i;
		num=1;
	    for(j=0;j<CommAnaly.Brain_Array[i][0];j++)
	      {
	    	CommAnaly.ALL_Cycles[i]=num++;

	    	if(CommAnaly.ALL_Cycles[i]>1)
	    		allCycNum-=CycCount;
	    	CycCount=0;

	       for(k=1;k<CommAnaly.Brain_Tnum[i];k++)
	         {

	           if(k%2 != 0)
	           {
						Indexstate.NowTemp=CommAnaly.Brain_Array[i][k];
		        allCycNum++;
						CommAnaly.Now_Stage = allCycNum;
	        	CycCount++;

	  
	        	if((CommAnaly.Brain_Array[i][k]<(double)65) && Bithe != Edge_High_Heat)	
						{
							Set_GPIO(High_Pin,SIDE_HEATER_EN);
							Set_Light(IN_Low_SIDE);
							Bithe = Edge_High_Heat;
						}							
	        	 else if((CommAnaly.Brain_Array[i][k]>=(double)65) && Bithe != Edge_Low_Heat)
						 {
							 Set_GPIO(High_Pin,SIDE_HEATER_EN);
							 Set_Light(IN_High_SIDE);
							 Bithe = Edge_Low_Heat;
						 }
						 Rise_Fall_Temp_PID(CommAnaly.Brain_Array[i][k],list_temper,&kp,&ki,&kd,&value_limit);
						 list_temper = CommAnaly.Brain_Array[i][k];
						 if(PID_Ch.para_kp != 0 || PID_Ch.para_ki != 0 || PID_Ch.para_kd != 0)
						 {
							kp = PID_Ch.para_kp;
						  ki = PID_Ch.para_ki;
						  kd = PID_Ch.para_kd;
							nowr6 = Normal_R6;
						 }
							xQueueSend (cameratoinspec,"1",3000);
							double Actual_Temp = 0;
							 Actual_Temp = PrToNTC(CommAnaly.Brain_Array[i][k]);
						 	TEC_Fan_Str.Set_Temper = Actual_Temp;
							Trends_Fin(&TEC_Fan_Str);
							Control_Tec(Actual_Temp,kp,ki,kd,nowr6,value_limit);
							xQueueSend (cameratoinspec,"0",3000);

	           }
	           else
	           {

	        	   if( allCycNum == CommAnaly.Set_Bit2  ||  allCycNum == CommAnaly.Set_Bit4  )
							 {
	        		   Boardack.NowCount=CommAnaly.ALL_Cycles[i];
							 }
	        	   else
	        		   Boardack.NowCount=0;
							reIndex=Stable_Temp(i,k-1,0);
	        	   Set_State(reIndex,Temp_Bit7);
							 
							 	double waittemptime = CommAnaly.Brain_Array[i][k] + 1;
								xQueueSend(workwaittime,&waittemptime,2000);

	        	   if( allCycNum == CommAnaly.Set_Bit2 || allCycNum == CommAnaly.Set_Bit4 )
	        	   {
								 vTaskDelay((CommAnaly.Brain_Array[i][k] - 15) * 1000 *0.94);
								static int Dissolu_Count = 0;
								Dissolu_Count	=	CommAnaly.ALL_Cycles[i];
								do
								{

	    	       	   copybit=CommAnaly.Set_Bit1;
										
	    	       	   unsigned char Freq_Count=0;
									 if(Elec_Struct.One_Step && Elec_Struct.Begin_Num)
										 {
											 Elec_Struct.One_Step_Num = 11420;
											 Elec_Struct.One_Step = 0;
										 }
								   xQueueSend (cameratoinspec,"1",3000);
	    	       	   while(Freq_Count++ <= Roul_Num)
	    	       	   {
	    	       		  bitnum=copybit&0x01;
	    	       		  copybit=copybit>>1;
										 
										 if(Freq_Count == 1)
										 {
											 devianum = 2815 - ( Elec_Struct.Begin_Num - Elec_Struct.One_Step_Num ) ;
											  Elec_Struct.Back_Index = devianum;
										 }
										 else
										 {
											 devianum = 2840 ;
										 }	
											EXTI->IMR &= ~(EXTI_Line5);			
											Elec_Struct.Begin_Num = 0;
										 setFilter(-devianum);
								//	 vTaskDelay(4000);
									if(bitnum)
										{
										 strcat(CameraIndex,"AF");
										 sprintf(CameraTemp,"%dE",Dissolu_Count);
										 strcat(CameraIndex,CameraTemp);
										 memset(CameraTemp,0,sizeof(CameraTemp));
										 sprintf(CameraTemp,"%dB",Freq_Count);
										 strcat(CameraIndex,CameraTemp);
										 xQueueSendToBack(Camera_Cric_Array,CameraIndex,0);
										 vTaskDelay(1000);
										 USART1_DMA_Send(CameraIndex,strlen(CameraIndex));
										
										 xQueueSemaphoreTake(Sign_Camera,portMAX_DELAY);
											
										 memset(CameraIndex,0,sizeof(CameraTemp));
										 memset(CameraTemp,0,sizeof(CameraTemp));
										}
										}
										Strat_Check();

									 	xQueueSend (cameratoinspec,"0",3000);
										
	        	   if( CommAnaly.Set_Bit3 || allCycNum == CommAnaly.Set_Bit4 )
	        	   {
								 Set_Bit3_State(1,Dissolve_Bit0);
	        		   double randavr=0;
								 static double Cum_Sum = -Dislve_Diff;
								 Cum_Sum += Dislve_Diff;	 
								 								 
								 if(CommAnaly.Brain_Array[i][k-1] <= Small_Temper)
									 kp = Dislve_Less30_Kp  ;
								 else if(CommAnaly.Brain_Array[i][k-1] >= Large_Temper)
									 kp = Dislve_Grea60_Kp ;
								 else
									 kp = Dislve_Kp  ;
							
	        		   ki = Dislve_Ki;
	        		   kd = Dislve_Kd;
								 solur6 = Dislve_R6 + Cum_Sum; 
								 if(solur6 >= 100)
									 solur6 = 100;
                 if(k+1>=CommAnaly.Brain_Tnum[i])
                   {
                      if(CommAnaly.Brain_Array[i][0] != 1&&CommAnaly.ALL_Cycles[i] <= CommAnaly.Brain_Array[i][0])
                          randavr=CommAnaly.Brain_Array[i][1];
                      else
													randavr=CommAnaly.Brain_Array[i+1][1];
                   }
                   else
                   {
													randavr=CommAnaly.Brain_Array[i][k+1];
                   }


                  if(CommAnaly.Brain_Array[i][k-1]<randavr)
                    {
													Divi+=((double)(CommAnaly.Set_Bit3&0x7F)/10);
                       if((CommAnaly.Brain_Array[i][k-1] + Divi)>=randavr) 
											 {
												 Cum_Sum = -Dislve_Diff;
												 break;
											 }
										}
                    else
                    {
                        Divi-=((double)(CommAnaly.Set_Bit3&0x7F)/10);
                      if((CommAnaly.Brain_Array[i][k-1]+Divi)<=randavr)
											{
												Cum_Sum = -Dislve_Diff;
												break;
											}
										}
										double cvalue_limi = 0;
									Rise_Fall_Temp_PID(CommAnaly.Brain_Array[i][k-1] + Divi,CommAnaly.Brain_Array[i][k-1],&kp,&ki,&kd,&cvalue_limi);
									xQueueSend (cameratoinspec,"1",3000);
									double Actual_Temp = 0;
									Actual_Temp = PrToNTC(CommAnaly.Brain_Array[i][k-1] + Divi);
									Control_Tec(Actual_Temp,kp,ki,kd,solur6,cvalue_limi);
									TEC_Fan_Str.Set_Temper = Actual_Temp;
									Trends_Fin(&TEC_Fan_Str);
									xQueueSend (cameratoinspec,"0",3000);	
									vTaskDelay(15 * 1000);										
	        		   reIndex=Stable_Temp(0,0,CommAnaly.Brain_Array[i][k-1]+Divi);
	        		   Set_State(reIndex,Temp_Bit7);														
	        	   }
							 Dissolu_Count ++;
	        	   }while(CommAnaly.Set_Bit3  || allCycNum == CommAnaly.Set_Bit4 );
								Set_Bit3_State(0,Dissolve_Bit0);
	        	   }  
								xQueueSemaphoreTake(waittotimeindex,portMAX_DELAY);						
						 }
	         }
	      }
	  }	
			if(work_init.workstop != NULL)
					work_init.workstop(NULL,0);
}
}
