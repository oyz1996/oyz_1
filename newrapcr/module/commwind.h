#ifndef __COMMWIND_H_
#define __COMMWIND_H_

#include "stdint.h"
#include "freeHead.h"

void  Init_CommWind(void);

typedef struct pid_para
{
	double para_kp;
	double para_ki;
	double para_kd;
}PID_Set;

extern QueueHandle_t analywindover;
extern QueueHandle_t Sign_Camera;

extern PID_Set PID_Ch;
extern xTaskHandle  CommWind_Handler;
#endif
