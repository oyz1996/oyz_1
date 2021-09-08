#ifndef	_GPIO_H_
#define _GPIO_H_

#include <stdint.h>
#include "freehead.h"
typedef enum 
{
	STEP_OUT,
	STEP_DIR,
	VLAMP_EN,
	LED_Y_EN,
	LED_G_EN,
	LED_R_EN,
	CAMERA_TRIGGER_IN,
	TOP_HEATER_EN,
	SIDE_HEATER_EN,
	STEP_EN,
	MOTOR_IN1,
	MOTOR_IN2,
	STEP_485_DE,
	STEP_485_RE,
	DECE_RESET
}_OUT_Pin;

typedef enum
{
	CAMERA_IO_OUT,
	STEP_OP,
	LIMIT_DOOR,
	PLATE_IN,
	CLEAN_DET,
	OP_MODULE_UP,
	OP_MODULE_DOWN
}_IN_Pin;

typedef enum
{
	Low_Pin,
	High_Pin
}Pin_States;

void GPIO_Config(void);

void Set_GPIO(uint8_t mode,_OUT_Pin SET_IO);
void Togg_GPIO(_OUT_Pin SET_IO);
void Device_Reset(void);
uint8_t Read_GPIO(_IN_Pin Read_IO);


#endif

