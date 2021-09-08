#include "resolv_in.h"

struct  Stor_Struct  CommAnaly;
struct  Ack_Struct   Boardack;
struct Index_Struct  Indexstate;
Fan_Con  		 TEC_Fan_Str;
State_Index 	Pcr_State;
static double data2hex(int NumFloat32)
{
    double sbit,ebits;
    double mbits;
    double result;
    double weishu=0;
    sbit = NumFloat32 >> 31;
    ebits = (NumFloat32&0x7FFFFFFF)>>23;
    mbits=(NumFloat32&0x7FFFFF)/pow(2,23);
    weishu=(double)1+mbits;
    result=pow(-1,sbit)*weishu*pow(2,(ebits-127));
    return result;
}
static uint8_t is_isdigit(char ch)
{
	if(ch >= '0' && ch <='9')
		return 1;
	else
		return 0;
}

static  uint8_t is_isalpha(char ch)
{
	if((ch >= 'A' && ch <='Z') || (ch >= 'a' && ch <= 'z'))
		return 1;
	else
		return 0;
}

static uint8_t is_isupper(char ch)
{
	if((ch >= 'A' && ch <='Z') )
		return 1;
	else 
		return 0;
}
static int c2i(char ch)
{
    // 如果是数字，则用数字的ASCII码减去48, 如果ch = '2' ,则 '2' - 48 = 2

    if(is_isdigit(ch))
            return ch - 48;

    // 如果是字母，但不是A~F,a~f则返回
    if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )
            return -1;

    // 如果是大写字母，则用数字的ASCII码减去55, 如果ch = 'A' ,则 'A' - 55 = 10
    // 如果是小写字母，则用数字的ASCII码减去87, 如果ch = 'a' ,则 'a' - 87 = 10
    if(is_isalpha(ch))
            return is_isupper(ch) ? ch - 55 : ch - 87;

    return -1;
}

static int h2i(char *src)
{
	char *drt=src;
	unsigned char length=0;
	int i=0,j=0;
	unsigned int result=0;
	length=strlen(drt);
	for(i=length-1,j=0;i>=0;i--,j++)
	{
		result+=c2i(drt[i])*pow(16,j);
	}
	return result;
}

static int Stor_Pow(int m,int n)
{
    int result = 1;
    while(n--) result *= m;
    return result;
}


static double Stor_Str2num(char * buf,int * dx)
{
    char *p = buf;
    int ires = 0,fres = 0;
    int ilen = 0,flen = 0,i;
    int mask = 0;
    double res;
    while(1) //得到整数和小数的长度
    {
        if(*p == '-'){mask |= 0X02;p++;}//是负数
        if(*p == 'E'||*p == '+'||*p == '/'||*p == '&'||*p == '@'||*p == 'B')break;//遇到结束了
        if(*p == '.'){mask |= 0X01;p++;}//遇到小数点了
        else if(*p > '9'||(*p < '0'))	//有非法字符
        {
            ilen = 0;
            flen = 0;
            break;
        }
        if(mask & 0X01) flen++;
        else ilen++;
        p++;
    }
    p++;
    if(mask & 0X02) buf++;	//去掉负号
    for(i = 0;i < ilen;i++)	//得到整数部分数据
    {
        ires += Stor_Pow(10,ilen-1-i)*(buf[i]-'0');
    }
    if(flen > 5) flen = 5;	//最多取5位小数
    *dx = flen;	 		//小数点位数
    for(i = 0;i < flen;i++)	//得到小数部分数据
    {
        fres += Stor_Pow(10,flen-1-i) * (buf[ilen+1+i]-'0');
    }
    res = ires * Stor_Pow(10,flen)+fres;
    if(mask & 0X02) res =- res;
    return res;
}

static int  GetData(char *sordata, int num)
{
    char *T_Array=sordata;
    while(num)
    {
        if(*sordata=='E')
            num--;
        sordata++;
    }
    return (sordata-T_Array);
}

static int  GetGiven(char *sordata, int num,char Letter)
{
    char *T_Array=sordata;
    while(num)
    {
        if(*sordata=='B')
            return 0xFF;
        if(Letter=='@')
            if(*sordata=='&') return 0XFF;
        if(*sordata==Letter)
            num--;
        sordata++;
    }
    return (sordata-T_Array);
}

double Calc_Temp(char *serial,uint8_t errtemp)
{
	double resist=0;
	int dec=0;
	int length=0;
	char src[10];
	char *iu;
	memset(src,0,sizeof(src));
	length=strlen(serial);
	if(serial[length-1]==0x20&&serial[length-2]==0x3e)
	{
		iu=strchr(serial,'\n');
		strncpy(src,&iu[1],8);
		src[8]='\0';
	}
	else return errtemp;
	dec=h2i(src);
	resist=data2hex(dec);
	return resist;
}


static int Separate2(char * Buf,int tnum)
{
    double ok;
    int num;
    int dx=0;
    ok=(double)Stor_Str2num(Buf,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Brain_Array[CommAnaly.Brain_Num][tnum++]=ok;
    num=GetGiven(Buf,1,'/');
    ok=(double)Stor_Str2num(Buf+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Brain_Array[CommAnaly.Brain_Num][tnum++]=ok;

    return tnum;
}

static void Separate(char * Buf)
{
    int i=0,j=1;
    int length=0;
    int dx=0;
    double ok;
    int num;
    int Loop=0;
    for(i=0;Buf[i]!='&'&&Buf[i]!='B';i++)
    {
        if(Buf[i]=='@')
        {
              Loop=1;
              break;
        }
    }
    if(Loop==0)
    {
        ok=(double)Stor_Str2num(Buf,&dx);
				ok = ok / Stor_Pow(10,dx);
        CommAnaly.Brain_Array[CommAnaly.Brain_Num][length++]=ok;
        num=GetGiven(Buf,1,'+');
        ok=(double)Stor_Str2num(Buf+num,&dx);
				ok = ok / Stor_Pow(10,dx);
        CommAnaly.Brain_Array[CommAnaly.Brain_Num][length++]=ok;
        num=GetGiven(Buf,1,'/');
        ok=(double)Stor_Str2num(Buf+num,&dx);
				ok = ok / Stor_Pow(10,dx);
        CommAnaly.Brain_Array[CommAnaly.Brain_Num][length++]=ok;
    }
    else
    {
        ok=(double)Stor_Str2num(Buf,&dx);
				ok = ok / Stor_Pow(10,dx);
        CommAnaly.Brain_Array[CommAnaly.Brain_Num][length++]=ok;
        num=GetGiven(Buf,1,'+');
        ok=(double)Stor_Str2num(Buf+num,&dx);
				ok = ok / Stor_Pow(10,dx);
        CommAnaly.Brain_Array[CommAnaly.Brain_Num][length++]=ok;
        num=GetGiven(Buf,1,'/');
        ok=(double)Stor_Str2num(Buf+num,&dx);
				ok = ok / Stor_Pow(10,dx);
        CommAnaly.Brain_Array[CommAnaly.Brain_Num][length++]=ok;
        do
        {
           num=GetGiven(Buf,j++,'@');
           if(num!=0XFF)
                length=Separate2(Buf+num,length);
        }while(num!=0xFF);
    }
    CommAnaly.Brain_Tnum[CommAnaly.Brain_Num]=length;
    CommAnaly.Brain_Num++;
}

static void An2_Funsion(char * test2)
{
    int num=0,i=1;
    while(num!=0XFF)
    {
        Separate(test2+num);
        num=GetGiven(test2,i++,'&');
    }
}

 void Anasysis_Fun(char *Test_Data)
{
    int num=0,dx=0;
    double ok=0;
    char Te[200];
    memset(Te,0,sizeof (Te));
    num=GetGiven(Test_Data,1,'P');
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Rg_Temper=ok;

    num=GetData(Test_Data,1);
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    strcpy(Te,Test_Data+num);
    An2_Funsion(Te);

    num=GetData(Test_Data,2);
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Expo_Time=ok;
    num=GetData(Test_Data,3);
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Sam_Volu=ok;
    num=GetData(Test_Data,4);
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Set_Bit1=(int)ok;
    num=GetData(Test_Data,5);
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Set_Bit2=(int)ok;
    num=GetData(Test_Data,6);
    ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Set_Bit3=(int)ok;
		num=GetData(Test_Data,7);
		ok=(double)Stor_Str2num(Test_Data+num,&dx);
		ok = ok / Stor_Pow(10,dx);
    CommAnaly.Set_Bit4=(int)ok;
}

 double NTCtoPr(double prtemp) 
{
	#if 1
	 double Actual_Temper = 0;
			if(prtemp > PrToNTC(0) &&( prtemp < PrToNTC(10) || fabs(prtemp - PrToNTC(10)) <= 0.001f))
			{
						 Actual_Temper = prtemp * Temper0_10_Slope - Temper0_10_Intercept;
			}
			else if(prtemp > PrToNTC(10) && prtemp < PrToNTC(50))
			{
					Actual_Temper = prtemp * Temper10_50_Slope - Temper10_50_Intercept;
			}
			else if((prtemp > PrToNTC(50) || fabs(prtemp - PrToNTC(50)) <= 0.001f) && ( prtemp < PrToNTC(60) || fabs(prtemp - PrToNTC(60)) <= 0.001f))
			{
					Actual_Temper = prtemp * Temper50_60_Slope - Temper50_60_Intercept;
			}
			else if(prtemp > PrToNTC(60) && prtemp < PrToNTC(65))
			{
					Actual_Temper = prtemp * Temper60_65_Slope - Temper60_65_Intercept;
			}
			else if( (prtemp > PrToNTC(65) || fabs(prtemp - PrToNTC(65)) <= 0.001f)&& (prtemp < PrToNTC(75) || fabs(prtemp - PrToNTC(75)) <= 0.001f))
			{
					Actual_Temper = prtemp * Temper65_75_Slope - Temper65_75_Intercept;
			}
			else if(prtemp > PrToNTC(75) && prtemp < PrToNTC(90))
			{
					Actual_Temper = prtemp * Temper75_90_Slope - Temper75_90_Intercept;
			}
			else if((prtemp > PrToNTC(90) || fabs(prtemp - PrToNTC(90)) <= 0.001f) && (prtemp < PrToNTC(100) || fabs(prtemp - PrToNTC(100)) <= 0.001f))
			{
					Actual_Temper = prtemp * Temper90_100_Slope + Temper90_100_Intercept;
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

void Inspection_Fun(void)
{
		int i,j;
    unsigned int length;

    char Head[3]="AZ";
    char End[5]="B";
    char Send_Data[300];
    char Temp_Array[200];
    char UART3_Data[100];
    double now_temper = Errortemp;
		double Top_temper = 0;
		double Fin_temper = 0; 
    unsigned camreaNum = 0;
		static double last_temper = 0;
		static uint8_t error_count = 0; 
    memset(UART3_Data,0,sizeof(UART3_Data));
    memset(Temp_Array,0,sizeof (Temp_Array));
    memset(Send_Data,0,sizeof (Send_Data));
		
	
		now_temper = Read_Temper();

	
	if(now_temper != 0)
	{
		last_temper = now_temper;
		error_count = 0 ;
	}
	else
	{
		now_temper = last_temper;
				error_count ++;
	}
	
	xQueueSend(worktoinspec,&now_temper,0);
	if(error_count >= Max_Error_Num)
	{
		Set_State(High_Pin,PR_Bit5);
	}
	else
	{
		if((Boardack.Bit1_State&PR_Bit5) == PR_Bit5)
					Set_State(0,PR_Bit5);	
	}
    strcat(Send_Data,Head);
    sprintf(Temp_Array,"%dE",Elec_Struct.Zero_Index);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));
	
		Top_temper = Read_Pin_Volt(TEMP_TOP_HEAT);
		xQueueSend(RgTempinspec,&Top_temper,0);
    sprintf(Temp_Array,"%.4fE",Top_temper);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));
	
		if(Top_temper < CommAnaly.Rg_Temper)
		{
			Set_GPIO(High_Pin,TOP_HEATER_EN);
		}
		else if (Top_temper > CommAnaly.Rg_Temper)
		{	
			Set_GPIO(Low_Pin,TOP_HEATER_EN);
		}
		
		Fin_temper = Read_Pin_Volt(TEMP_FIN);
		TEC_Fan_Str.Fin_Temper = Fin_temper;
		
    sprintf(Temp_Array,"%.4fE",Fin_temper);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));
		
		now_temper = NTCtoPr(now_temper);
		TEC_Fan_Str.Pelti_Temper = now_temper;

    sprintf(Temp_Array,"%.4fE",now_temper);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));

		int Stage_Num = 1;
		int All_Stage = 1;
#if 1
		
    for(i=0;i<CommAnaly.Brain_Num;i++)
    {
      if(CommAnaly.Brain_Tnum[i] <= 3)
        {
            if( (All_Stage == CommAnaly.Set_Bit2 && CommAnaly.Set_Bit3) /*|| CommAnaly.Brain_Num ==1*/) 
						{
							char Diss_Char = 'M';
							sprintf(Temp_Array,"%c+",Diss_Char);
						}
						else
						{
						sprintf(Temp_Array,"%d+",(int)CommAnaly.Brain_Array[i][0]);
						}
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));

            if(CommAnaly.Brain_Array[i][1]==Indexstate.NowTemp&& Stage_Num == CommAnaly.Now_Stage)
						{
            	sprintf(Temp_Array,"%.4f$/",CommAnaly.Brain_Array[i][1]);
							All_Stage++;
						}
            else
						{
            	sprintf(Temp_Array,"%.4f/",CommAnaly.Brain_Array[i][1]);
							All_Stage++;
							Stage_Num ++;
						}
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));

            sprintf(Temp_Array,"%d&",(int)CommAnaly.Brain_Array[i][2]);
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));
        }
        else
        {
        	camreaNum++;
     				if( (All_Stage == CommAnaly.Set_Bit2  || /*CommAnaly.Brain_Num ==1 ||*/ (All_Stage < CommAnaly.Set_Bit2 && (All_Stage + ( CommAnaly.Brain_Tnum[i]-1)/2) > CommAnaly.Set_Bit2)) && CommAnaly.Set_Bit3 ) 
						{
							char Diss_Char = 'M';
							sprintf(Temp_Array,"%c+",Diss_Char);
						}
						else
						{
						sprintf(Temp_Array,"%d+",(int)CommAnaly.Brain_Array[i][0]);
						}
						
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));

            if(CommAnaly.Brain_Array[i][1]==Indexstate.NowTemp&&Stage_Num == CommAnaly.Now_Stage)
						{
            	sprintf(Temp_Array,"%.4f$/",CommAnaly.Brain_Array[i][1]);
							All_Stage++;
						}
            else
						{
            	sprintf(Temp_Array,"%.4f/",CommAnaly.Brain_Array[i][1]);
							Stage_Num ++;
							All_Stage++;
						}
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));

            for(j=2;j<CommAnaly.Brain_Tnum[i]-1;j++)
            {
                if(j%2==0)
                    sprintf(Temp_Array,"%d@",(int)CommAnaly.Brain_Array[i][j]);
                else
                {
                	if(CommAnaly.Brain_Array[i][j]==Indexstate.NowTemp&&Stage_Num == CommAnaly.Now_Stage)
									{
                		sprintf(Temp_Array,"%.4f$/",CommAnaly.Brain_Array[i][j]);
										All_Stage++;
									}
                	else
									{
                        sprintf(Temp_Array,"%.4f/",CommAnaly.Brain_Array[i][j]);
												Stage_Num ++;
										All_Stage++;
									}
                }
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));
            }
            sprintf(Temp_Array,"%d&",(int)CommAnaly.Brain_Array[i][j]);
            strcat(Send_Data,Temp_Array);
            memset(Temp_Array,0,sizeof (Temp_Array));
        }
    }
    length=strlen(Send_Data);
    Send_Data[length-1]='E';
#endif
    sprintf(Temp_Array,"%.4fE",CommAnaly.Expo_Time);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));

    sprintf(Temp_Array,"%dE",Boardack.Bit1_State);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));

    Boardack.Bit2_State=Boardack.NowCount;
    sprintf(Temp_Array,"%dE",Boardack.Bit2_State);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));

    sprintf(Temp_Array,"%d",Elec_Struct.Back_Index);
    strcat(Send_Data,Temp_Array);
    memset(Temp_Array,0,sizeof (Temp_Array));

    strcat(Send_Data,End);
	
		USART1_DMA_Send(Send_Data,strlen(Send_Data));
		
}

 void Trends_Fin(Fan_Con * ch_fin)
{
	Fan_Con * lo_fan = ch_fin;
	double Temper_Diff = 0;
	
	Temper_Diff = lo_fan->Pelti_Temper - lo_fan->Set_Temper;
		
	if( Temper_Diff > 0 )
			lo_fan->T_S_Ch_Dir = Cool_Down;
	else if( Temper_Diff < 0)
			lo_fan->T_S_Ch_Dir = Heat_Up;
	else
			lo_fan->T_S_Ch_Dir = Keep;
	
	lo_fan->T_S_Temper_Diff = fabs(Temper_Diff);
	
	Temper_Diff = lo_fan->Fin_Temper - lo_fan->Set_Temper;
	
	if( Temper_Diff > 0 )
			lo_fan->F_S_Ch_Dir = Cool_Down;
	else if( Temper_Diff < 0)
			lo_fan->F_S_Ch_Dir = Heat_Up;
	else
			lo_fan->F_S_Ch_Dir = Keep;
	
	lo_fan->F_S_Temper_Diff = fabs(Temper_Diff);
	
	if( lo_fan->F_S_Ch_Dir == Heat_Up )
	{
		  int Decile  = -1;
		  int fan_ccr = 0;
		  Decile = (((int)lo_fan->F_S_Temper_Diff) / 10) * 1; 
		  fan_ccr = Heat_TEC_FAN_MAX_CCR - Decile;
			Open_TEC_Fan(lo_fan->Tec_Fan_Arr,lo_fan->Last_Cycle,fan_ccr);  
			lo_fan->Last_Cycle = fan_ccr ;		
	}
	else if( lo_fan->F_S_Ch_Dir == Cool_Down )
	{
			int Decile  = -1;
			int fan_ccr = 0;
			Decile = (((int)lo_fan->F_S_Temper_Diff) / 10) * 2; 
			fan_ccr = Cool_TEC_FAN_MIN_CCR + Decile ; 
			
			Open_TEC_Fan(lo_fan->Tec_Fan_Arr,lo_fan->Last_Cycle,fan_ccr); 
			lo_fan->Last_Cycle = fan_ccr ;
		
	}
	else
	{

	}
		
}
