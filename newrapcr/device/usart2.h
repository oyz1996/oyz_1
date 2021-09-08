#ifndef _USART2_H_
#define _USART2_H_

#include <stdint.h>
#include "uart_comm.h"



void USART2_VAR_Init(void);
void USART2_Init(void);
void USART2_DMA_Init(void);
void USART2_VarInit(void);
void USART2_DMA_Config(void);
uint8_t USART2_DMA_Send(void *addr,uint16_t len);

void USART2_Config(void);
extern struct USART_HOOKHanler usart2hook;
#endif
