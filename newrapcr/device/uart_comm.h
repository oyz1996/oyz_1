#ifndef _UART_COMM_H_
#define _UART_COMM_H_

#include <stdint.h>

struct USART_HOOKHanler
{
   void (* tmcprocess)(void *,uint16_t *,uint16_t);
   void (* receiveProcess)(void *,uint16_t);
   void (* prtmprocess)(void *,uint16_t);
};

struct USART_RcvDMADataAddr {
	uint8_t * addr;
	uint8_t * addr1;
	uint16_t len;
	uint16_t len1;
};

#endif
