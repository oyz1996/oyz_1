#ifndef _ELECT_H_
#define _ELECT_H_

#include <stdint.h>
#include "freehead.h"

enum Bit_Sta
{
	Check_Bit0=0x01,
	Idle_Bit1=0x02,
	Runn_Bit2=0x04,
	Err_Bit3=0x08,
	Led_Bit4=0x10,
	PR_Bit5=0x20,
	Tmcl_Bit6=0x40,
	Temp_Bit7=0x80
};

enum Bit3_Sta
{
	Dissolve_Bit0 = 0x01,
	Idle1_Bit1 = 0x02,
	Idle2_Bit1 = 0x03,
	Idle3_Bit1 = 0x04,
	Idle4_Bit1 = 0x05,
	Idle5_Bit1 = 0x06,
	Idle6_Bit1 = 0x07,
	Idle7_Bit1 = 0x08,	
};

enum Work_Sta
{
   IN_Idle = 0x00,
   IN_Low_SIDE,
   IN_High_SIDE,
   IN_Work,
   IN_Slef_Check,
   IN_Warring,
   IN_Hard_Error,
   IN_Memory_Error,
   IN_Init = 255
};

typedef struct 
{
	uint8_t OP_Index;								// 上升沿检测标志
	uint32_t Begin_Num;							// 返回原点的脉冲数
	uint32_t Check_Index;						// 需要上升沿标志
	uint32_t Back_Index;						// 第一个需要走的脉冲数
	uint32_t Zero_Index;						// 返回原点的脉冲数
	uint32_t Zero_Wait_Time;				// 下降沿检测超时表示
	uint32_t Zero_num;							// 上升沿 下降沿 超出脉冲数
	uint32_t Back_Zero;						  //	一次检测上升沿走的脉冲数			
	uint32_t One_Step;							// 第一次检测开始标志
	uint32_t  One_Step_Num;					// 第一次检测返回原点的脉冲数
}Elec_Index;

typedef enum 
{
	InitState,					//初始化状态
	AvrFlag,						//匀速状态
	IncSpeedState,			//加速状态
	DecelSpeedState			//减速状态	
}SpeedState;
typedef struct 
{
	uint32_t	IncNum;									//总脉冲计数
	uint32_t	SurNum;									//剩余脉冲计数
	SpeedState	SpeedStruct;					//速度状态
	uint8_t	CurState;
	
}FreSpeed;

extern Elec_Index Elec_Struct;
extern FreSpeed  FreStruct;
extern QueueHandle_t		exti_limit;
void Init_Speed(void);
void Elec_Init(void);
void Set_State(uint8_t mode,enum Bit_Sta comSta);
void Set_Light(enum Work_Sta myWork);
void Set_Bit3_State(uint8_t mode,enum Bit3_Sta comSta);
int Strat_Check(void);

void BoardConHeatDish_UP(void);
void BoardConHeatDish_DOWN(void);
void Elect_Output(uint32_t Slave_Pulse);

void IncSpeed(uint32_t Slave_Pulse);
void DecSpeed(uint32_t Slave_Pulse);
void AvrSpeed(uint32_t Slave_Pulse);
	
uint32_t setFilter(int devia);
void CalculateSModelLine(float fre[],  unsigned short period[],   float  len,  float fre_max,  float fre_min, float flexible);
#endif

