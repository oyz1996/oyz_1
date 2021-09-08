#ifndef _GPIO_IN_H_
#define _GPIO_IN_H_

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "gpio.h"


#define GPIO_RCC_Init		\
		do{\
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	\
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	\
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	\
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);	\
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);	\
		}while(0)

/*
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
	STEP_485_RE
}_OUT_Pin;
*/
#define STEP_OUT_GPIO							GPIOB
#define STEP_OUT_PIN							GPIO_Pin_0
#define STEP_DIR_GPIO							GPIOB
#define STEP_DIR_PIN							GPIO_Pin_1
#define DECE_RESET_GPIO						GPIOC
#define DECE_RESET_PIN						GPIO_Pin_8
#define VLAMP_EN_GPIO							GPIOD
#define VLAMP_EN_PIN							GPIO_Pin_8
#define	LED_Y_EN_GPIO							GPIOD
#define	LED_Y_EN_PIN							GPIO_Pin_9
#define	LED_G_EN_GPIO							GPIOD
#define	LED_G_EN_PIN							GPIO_Pin_10
#define	LED_R_EN_GPIO							GPIOD
#define	LED_R_EN_PIN							GPIO_Pin_11
#define CAMERA_TRIGGER_IN_GPIO		GPIOD
#define CAMERA_TRIGGER_IN_PIN			GPIO_Pin_1
#define TOP_HEATER_EN_GPIO				GPIOE
#define TOP_HEATER_EN_PIN					GPIO_Pin_4
#define SIDE_HEATER_EN_GPIO				GPIOE
#define SIDE_HEATER_EN_PIN				GPIO_Pin_6
#define STEP_EN_GPIO							GPIOE
#define STEP_EN_PIN								GPIO_Pin_8
#define MOTOR_IN1_GPIO						GPIOE
#define MOTOR_IN1_PIN							GPIO_Pin_10
#define MOTOR_IN2_GPIO						GPIOE
#define MOTOR_IN2_PIN							GPIO_Pin_11
#define STEP_485_DE_GPIO					GPIOE
#define STEP_485_DE_PIN						GPIO_Pin_0
#define STEP_485_RE_GPIO					GPIOE
#define STEP_485_RE_PIN						GPIO_Pin_1
/*
typedef enum
{
	CAMERA_IO_OUT,
	STEP_OP,
	LIMIT_DOOR,
	PLATE_IN,
	CLEAN_DET,
	OP_MODULE_UP,
	OP_MODULE_DOWN,
}_IN_Pin;
*/
#define CAMERA_IO_OUT_GPIO					GPIOD
#define CAMERA_IO_OUT_PIN						GPIO_Pin_0
#define STEP_OP_GPIO								GPIOC
#define STEP_OP_PIN									GPIO_Pin_5
#define LIMIT_DOOR_GPIO							GPIOD
#define LIMIT_DOOR_PIN							GPIO_Pin_12
#define PLATE_IN_GPIO								GPIOE
#define PLATE_IN_PIN								GPIO_Pin_13
#define CLEAN_DET_GPIO							GPIOE
#define CLEAN_DET_PIN								GPIO_Pin_12
#define OP_MODULE_UP_GPIO						GPIOE
#define OP_MODULE_UP_PIN						GPIO_Pin_15
#define OP_MODULE_DOWN_GPIO					GPIOE
#define OP_MODULE_DOWN_PIN					GPIO_Pin_14

#endif 

