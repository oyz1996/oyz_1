#ifndef  __RESOLV_H_
#define  __RESOLV_H_

#include <stdint.h>

#define Max_Cycles 100

#define Temper0_10_Slope				1.0635
#define Temper0_10_Intercept		1.7293

#define Temper10_50_Slope				1.0701
#define Temper10_50_Intercept		1.731

#define Temper50_60_Slope				1.0736
#define Temper50_60_Intercept		1.737

#define Temper60_65_Slope				1.0741
#define Temper60_65_Intercept		1.762

#define Temper65_75_Slope				1.0746
#define Temper65_75_Intercept		1.787

#define Temper75_90_Slope				1.0661
#define Temper75_90_Intercept		1.210

#define Temper90_100_Slope			1.0427
#define Temper90_100_Intercept	0.9784


#define Heat_TEC_FAN_MAX_CCR					15
#define Cool_TEC_FAN_MIN_CCR					15

enum Temp_Dir
{
	Heat_Up,
	Cool_Down,
	Keep,
};


typedef struct Fan_Fan
{
	double 	Fin_Temper;
	double 	Pelti_Temper;
	double 	Set_Temper;
	double  T_S_Temper_Diff;
	double  F_S_Temper_Diff;
	uint32_t Tec_Fan_Arr;
	uint8_t Last_Cycle;
	enum Temp_Dir	F_S_Ch_Dir;
	enum Temp_Dir T_S_Ch_Dir;
}Fan_Con;


typedef struct  Stor_Struct
{
    double Expo_Time;									//�ع�ʱ��
    double Tet_Temper;									//�������趨�¶�
    double Rg_Temper;									//�ȸ��趨�¶�
    double Rg_NowTemper;								//�ȸ�ʵʱ�¶�
    double Sam_Volu;									//�������
    int    Set_Bit1;									//�˹����ֽ�1
    int    Set_Bit2;									//�˹����ֽ�2
    int    Set_Bit3;									//�˹����ֽ�3
		int    Set_Bit4;									//�˹����ֽ�4
    int	   Cust_Bit1;									//�Զ����ֽ�1
    int	   Cust_Bit2;									//�Զ����ֽ�2
    int    Cust_Bit3;									//�Զ����ֽ�3
		volatile double Brain_Array[Max_Cycles][Max_Cycles];			//�¶�ʱ��
    int Brain_Tnum[Max_Cycles];							//�����¶�ʱ�򳤶�
    int Brain_Num;										//���¶�ʱ�򳤶�
    int ALL_Cycles[Max_Cycles];							//�����¶�ʱ���ѭ������
    int Curr_Cycles;									//��ǰѭ��λ��
		volatile  int Now_Stage;										
}My_Stor;

typedef struct  Ack_Struct
{
//	unsigned char Check_State;			//0:Check Fail     1:Check Success
//	unsigned char Idle_State;			//1:IDLE
//	unsigned char Runn_State;			//1��Running
//	unsigned char Err_State;			//0��No Error		1:An error occurred
//	unsigned char Led_State;			//0:normal			1:Expire
	unsigned char Memory_State;		//1:Memory Error
	unsigned char Bit1_State;			//bit0:Check_State		bit1:Idle_State		bit2:Runn_State	bit3:Err_State	bit4:Led_State
	unsigned char Bit2_State;			//bit0-bit7		Error Reason
	unsigned char Bit3_State;			//bit0-bit7
	unsigned char NowCount;

}Ack_Self;

typedef struct Index_Struct
{
	double  NowTemp;
}Index_Self;

enum state 
{
	SelfCheck,
	IDle,
	Work,
	Error
};
typedef struct Work_State
{
	enum state slestate;
}State_Index;


void Anasysis_Fun(char *Test_Data);
void Inspection_Fun(void);
double Calc_Temp(char *serial,uint8_t errtemp);
void Trends_Fin(Fan_Con * ch_fin);
double NTCtoPr(double prtemp);
extern struct  Stor_Struct  	CommAnaly;
extern struct  Ack_Struct 	 	Boardack;
extern struct  Index_Struct  	Indexstate;
extern Fan_Con   TEC_Fan_Str;
extern State_Index 	Pcr_State;

#endif 
