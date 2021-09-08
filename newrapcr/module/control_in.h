#ifndef _CONTROL_IN_H_
#define _CONTROL_IN_H_

#include "control.h"
#include "usart2.h"
#include "resolv.h"
#include "worksequ.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PR59_MAX_SIZE		128

#define PR59_Wait_Time				10
#define PR59_Wait_Min_Time		(unsigned short)(100 * 2)
#define PR59_Wait_Max_Time		(unsigned short)(100 * 2 *10)
struct  usart_pr59rece
{
	uint16_t len;
	
	uint16_t idleindex;
	
	uint16_t wait_time;
	
	uint8_t addr[PR59_MAX_SIZE];
};

#define		collnum				2 

#define    RN100_Len		0x15

double Temp_Diff = 0.5;

#endif
