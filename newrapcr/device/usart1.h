#ifndef _USART1_H_
#define _USART1_H_

#include "freehead.h"
#include "uart_comm.h"


void USART1_VarInit(void);
	
void USART1_Config(void);

void USART1_DMA_Config(void);

void USART1_Init(void);

uint8_t USART1_DMA_Send(void *addr , uint8_t len);

extern struct USART_HOOKHanler usart1Hook;

#endif

