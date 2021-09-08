#include "gpio_in.h"


static void GPIO_RCC_Config(void)
{
	GPIO_RCC_Init;
}
static void GPIO_Mode_Config(void)
{
	GPIO_InitTypeDef	GPIO_struct;
	
	/*	GPIOA	Input	*/
	GPIO_struct.GPIO_Pin = GPIO_Pin_1	| GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA,&GPIO_struct);
	
	/*	GPIOB	Output	*/
	GPIO_struct.GPIO_Pin =  GPIO_Pin_1;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_struct);
	
	/*	GPIOC	Output	*/
	
	GPIO_struct.GPIO_Pin = GPIO_Pin_8;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;

	
	GPIO_Init(GPIOC,&GPIO_struct);
	
	
	/*	GPIOD	Output	*/
	GPIO_struct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOD,&GPIO_struct);
	
	/*	GPIOD	Input	*/
	GPIO_struct.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_12;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD,&GPIO_struct);
	
	/*	GPIOE	Output	*/
	GPIO_struct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_10;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE,&GPIO_struct);
	
	
	GPIO_struct.GPIO_Pin =  GPIO_Pin_8 ;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE,&GPIO_struct);
	
	/*	GPIOE	Input	*/
	GPIO_struct.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOE,&GPIO_struct);
	
//	Set_GPIO(1,STEP_EN);
	
}

void GPIO_Config(void)
{
	GPIO_RCC_Config();
	GPIO_Mode_Config();
}
/*
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
*/
void Set_GPIO(uint8_t mode,_OUT_Pin SET_IO)
{
	BitAction IO_State = Bit_RESET;
	
	if( mode == Low_Pin)
		 IO_State = Bit_RESET;
	else 
			IO_State = Bit_SET;

	switch((int)SET_IO)
	{
	//	case STEP_OUT:
	//		GPIO_WriteBit(STEP_OUT_GPIO,STEP_OUT_PIN,IO_State);
//		break;
		case STEP_DIR:
			GPIO_WriteBit(STEP_DIR_GPIO,STEP_DIR_PIN,IO_State);
		break;
		case VLAMP_EN:
			GPIO_WriteBit(VLAMP_EN_GPIO,VLAMP_EN_PIN,IO_State);
		break;		
		case LED_Y_EN:
			GPIO_WriteBit(LED_Y_EN_GPIO,LED_Y_EN_PIN,IO_State);
		break;		
		case LED_G_EN:
			GPIO_WriteBit(LED_G_EN_GPIO,LED_G_EN_PIN,IO_State);
		break;
		case LED_R_EN:
			GPIO_WriteBit(LED_R_EN_GPIO,LED_R_EN_PIN,IO_State);
		break;
		case CAMERA_TRIGGER_IN:
			GPIO_WriteBit(CAMERA_TRIGGER_IN_GPIO,CAMERA_TRIGGER_IN_PIN,IO_State);
		break;
		case TOP_HEATER_EN:
			GPIO_WriteBit(TOP_HEATER_EN_GPIO,TOP_HEATER_EN_PIN,IO_State);
		break;
		case SIDE_HEATER_EN:
			GPIO_WriteBit(SIDE_HEATER_EN_GPIO,SIDE_HEATER_EN_PIN,IO_State);
		break;
		case STEP_EN:
			GPIO_WriteBit(STEP_EN_GPIO,STEP_EN_PIN,IO_State);
		break;
		case MOTOR_IN1:
			GPIO_WriteBit(MOTOR_IN1_GPIO,MOTOR_IN1_PIN,IO_State);
		break;
		case MOTOR_IN2:
			GPIO_WriteBit(MOTOR_IN2_GPIO,MOTOR_IN2_PIN,IO_State);
		break;
		case STEP_485_DE:
			GPIO_WriteBit(STEP_485_DE_GPIO,STEP_485_DE_PIN,IO_State);
		break;
		case STEP_485_RE:
			GPIO_WriteBit(STEP_485_RE_GPIO,STEP_485_RE_PIN,IO_State);
		break;
		case DECE_RESET:
			GPIO_WriteBit(DECE_RESET_GPIO,DECE_RESET_PIN,IO_State);
		break;
	}
}

uint8_t Read_GPIO(_IN_Pin Read_IO_IO)
{
	uint8_t result = 250;
	
	switch((int)Read_IO_IO)
	{
		case CAMERA_IO_OUT:
			result = GPIO_ReadInputDataBit(CAMERA_IO_OUT_GPIO,CAMERA_IO_OUT_PIN);
		break;
	//	case STEP_OP:
//			result = GPIO_ReadInputDataBit(STEP_OP_GPIO,STEP_OP_PIN);
//		break;
		case LIMIT_DOOR:
			result = GPIO_ReadInputDataBit(LIMIT_DOOR_GPIO,LIMIT_DOOR_PIN);
		break;	
		case PLATE_IN:
			result = GPIO_ReadInputDataBit(PLATE_IN_GPIO,PLATE_IN_PIN);
		break;		
		case CLEAN_DET:
			result = GPIO_ReadInputDataBit(CLEAN_DET_GPIO,CLEAN_DET_PIN);
		break;		
		case OP_MODULE_UP:
			result = GPIO_ReadInputDataBit(OP_MODULE_UP_GPIO,OP_MODULE_UP_PIN);
		break;	
		case OP_MODULE_DOWN:
			result = GPIO_ReadInputDataBit(OP_MODULE_DOWN_GPIO,OP_MODULE_DOWN_PIN);
		break;				
	}
	return result;
}
void Togg_GPIO(_OUT_Pin SET_IO)
{
	switch(SET_IO)
	{
		case LED_Y_EN:
			GPIO_ToggleBits(LED_Y_EN_GPIO,LED_Y_EN_PIN);
		break;		
		case LED_G_EN:
			GPIO_ToggleBits(LED_G_EN_GPIO,LED_G_EN_PIN);
		break;
		case LED_R_EN:
			GPIO_ToggleBits(LED_R_EN_GPIO,LED_R_EN_PIN);
			for(int i = 0; i<= 10 * 32; i++)
		for(int j = 0; j <= 65535; j++);
		break;
	//	case STEP_OUT:
	//		GPIO_ToggleBits(STEP_OUT_GPIO,STEP_OUT_PIN);
	//	break;
	}

}
void Device_Reset(void)
{
	Set_GPIO(High_Pin,DECE_RESET);
}
