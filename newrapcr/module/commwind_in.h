#ifndef __COMMWIND_IN_H_
#define __COMMWIND_IN_H_

#include "stm32f4xx_gpio.h"
#include "freehead.h"
#include "usart1.h"
#include "gpio.h"
#include "commwind.h"
#include "resolv.h"
#include "elect.h"
#include "control.h"
#include "worksequ.h"
#include "tim1.h"
#include "tim3.h"
#include "tim9.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define Head_Array_row 4
#define Head_Array_col 7 

#define APCommAck 		"APYB"
#define APCommAck_Len	4
#define COMM_WIND_HOOK usart1Hook

#define MAX_BUF_SIZE 512

#define SN_Data_ADDR		0x080C0000
#define SN_Data_Length	14

#define SN_Data_ADDR	0x080C0000
#define Defalut_TEC_FAN_Init_CCR			50
#define Defalut_TEC_FAN_End_CCR				20
			
#define Defalut_Back_FAN_Init_CCR			Defalut_TEC_FAN_Init_CCR
#define Defalut_Back_FAN_End_CCR			30
struct commdata  
{
	uint16_t len;
	
	uint8_t addr[MAX_BUF_SIZE];
};




#endif
