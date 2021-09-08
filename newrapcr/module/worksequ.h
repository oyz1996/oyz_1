#ifndef __WORKSEQU_H_
#define __WORKSEQU_H_

#include "freehead.h"
#include <stdint.h>

void New_Task(void);
void Init_WorkSequ(void);
void Delete_Task(void);
 double PrToNTC(double prtemp);



typedef struct workhook
{
	void ( * workstart)(void *,int);
	void ( * workstop)(void *, int);
}work_hook;


void Rise_Fall_Temp_PID(double runn_temp,double list_temp,double * kp, double * ki, double *kd , double * va_limit);
extern xQueueHandle cameratoinspec;
extern xQueueHandle worktoinspec;
extern xQueueHandle Camera_Cric_Array;
extern work_hook  work_init;
extern xQueueHandle workwaittime;
extern QueueHandle_t waittotimeindex;
extern xTaskHandle  WorkSequ_Handler;
extern xQueueHandle RgTempinspec;
#endif
