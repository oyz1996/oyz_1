#ifndef __WORKSEQU_IN_H_
#define __WORKSEQU_IN_H_

#include "commwind.h"
#include "worksequ.h"
#include "iic.h"
#include "elect.h"
#include "gpio.h"
#include "usart1.h"
#include "resolv.h"
#include "control.h"
#include "tim1.h"
#include "tim3.h"

#include "stm32f4xx_exti.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define Edge_High_Heat 0x30
#define Edge_Low_Heat  0x04


#define Min_Temp				4
#define Midd_Temp       40
#define MAX_Temp				95

#define Roul_Num				4
//	11.6
//	1.4
//	4.875
#define Normal_Kp				25
#define Normal_Ki				5.3
#define Normal_Kd				38
#define Normal_R6				100

#define Dislve_Kp				0.5
#define Dislve_Ki				0.195
#define Dislve_Kd				4
#define Dislve_R6				20

#define Dislve_Grea60_Kp	0.8
#define Large_Temper			60

#define Dislve_Less30_Kp	0.6
#define Small_Temper			30

#define Dislve_Diff				0.2

#define Diff_Temp_20_Base_Kp	8.3
#define Diff_Temp_20_Base_Ki	0.4
#define Diff_Temp_20_Base_Kd	11

#define Diff_Temp_20_Mult_Kp	1.072
#define Diff_Temp_20_Mult_Ki	0.658
#define Diff_Temp_20_Mult_Kd	1.131

#endif

