#ifndef _USART6_H_
#define _USART6_H_

#include "uart_comm.h"
#include "freehead.h"
#include "gpio.h"
#include <stdint.h>
#include <string.h>

void USART6_Config(void);
void USART6_Init(void);

void USART6_DMA_Config(void);
uint16_t USART6_DMA_Send(void *addr,uint16_t len);

void USART6_Send_Array(void *addr,uint16_t len);
void USART6_VarInit(void);

extern struct USART_HOOKHanler 	USART6hook;

#endif
