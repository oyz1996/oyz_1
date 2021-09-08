#ifndef _USART3_H_
#define _USART3_H_

#include <stdint.h>
#include "freehead.h"
#include "uart_comm.h"

void USART3_DMA_Config(void);
void USART3_Config(void);
void USART3_VAR_Init(void);
void USART3_Init(void);
void USART3_DMA_Init(void);
void USART3_VarInit(void);
uint8_t USART3_DMA_Send(void *addr,uint16_t len);

extern struct USART_HOOKHanler usart3hook;

#endif

