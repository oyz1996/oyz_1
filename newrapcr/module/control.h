#ifndef  __CONTROL_H_
#define  __CONTROL_H_

#include <stdint.h>

void Init_Control(void);
void Close_PR59(void);
void Set_Ntc_Mode(void);
void pr59_error_handler(void);
double Read_Temper(void);
int Stable_Temp(int row,int arr,double temper);
int Control_Tec(double Temper,double kp,double ki,double kd,double Nr6,double value_limit);


#define 	Errortemp		0
#define 	RH16_10K 	 	12
#define 	R92_Set			3

#endif 
