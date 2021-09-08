#ifndef _USART3_IN_H_
#define _USART3_IN_H_

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dma.h>

#include "usart3.h"


#define USART3_TX_RCC            RCC_AHB1Periph_GPIOB  
#define USART3_TX_GPIO           GPIOB
#define USART3_TX_PIN            GPIO_Pin_10
#define USART3_TX_PIN_SRC        GPIO_PinSource10

#define USART3_RX_RCC            RCC_AHB1Periph_GPIOB
#define USART3_RX_GPIO           GPIOB
#define USART3_RX_PIN            GPIO_Pin_11
#define USART3_RX_PIN_SRC        GPIO_PinSource11

#define USART3_PIN_RCC_INIT \
   do{\
      RCC_AHB1PeriphClockCmd(USART3_TX_RCC | USART3_RX_RCC,ENABLE);\
     }while(0)

#define USART3_GPIO_INIT   \
    do{\
       GPIO_InitTypeDef GPIOinitype;\
       GPIO_PinAFConfig(USART3_TX_GPIO,USART3_TX_PIN_SRC,GPIO_AF_USART3);\
       GPIO_PinAFConfig(USART3_RX_GPIO,USART3_RX_PIN_SRC,GPIO_AF_USART3);\
       \
       GPIOinitype.GPIO_Mode = GPIO_Mode_AF; \
       GPIOinitype.GPIO_Speed = GPIO_Speed_50MHz;\
       GPIOinitype.GPIO_OType = GPIO_OType_PP;\
       GPIOinitype.GPIO_PuPd = GPIO_PuPd_UP;\
       \
       GPIOinitype.GPIO_Pin = USART3_TX_PIN;\
       GPIO_Init(USART3_TX_GPIO,&GPIOinitype);\
       \
       GPIOinitype.GPIO_Pin = USART3_RX_PIN;\
       GPIO_Init(USART3_RX_GPIO,&GPIOinitype);\
      }while(0)

#define USART3_RCC_INIT \
      do{\
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);\
        }while(0)
      
#define USART3_IRQ_PRE_PRIOR   		 configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3
#define USART3_IRQ_SUB_PRIOR   		 0
        
#define USART3_BaudRate        		 115200
#define USART3_Parity          		 USART_Parity_No


#define USART3_TX_DMA_RCC					RCC_AHB1Periph_DMA1
#define USART3_TX_DMA_STREAM 			DMA1_Stream3	
#define USART3_TX_DMA_CHANNEL   	DMA_Channel_4	
#define USART3_TX_DMA_IRQ					DMA1_Stream3_IRQn
#define USART3_TX_DMA_PRE_PRIOR		3
#define USART3_TX_DMA_SUB_PRIOR		0
#define USART3_TX_DMA_IRQHandler  DMA1_Stream3_IRQHandler
				
#define USART3_RX_DMA_RCC					RCC_AHB1Periph_DMA1
#define USART3_RX_DMA_STREAM			DMA1_Stream1
#define USART3_RX_DMA_CHANNEL			DMA_Channel_4
#define USART3_RX_DMA_IRQ					DMA1_Stream1_IRQn
#define USART3_RX_DMA_PRE_PRIOR		4
#define USART3_RX_DMA_SUB_PRIOR		0
#define USART3_RX_DMA_IRQHandler  DMA1_Stream1_IRQHandler

#define USART3_TX_DMA_BUF_SIZE		128
#define USART3_RX_DMA_BUF_SIZE		64

#define USART3_IDLE_No					0
#define USART3_IDLE_Yes				(!USART3_IDLE_No)		

struct USART3_Send
{
   uint16_t remain;
   
   uint16_t index;
   
   void * addr;
};

struct USART3_Receive
{
	uint16_t len;
	
	uint16_t idleindex;
	
	void *addr;
};

struct USART3_RcvDMADataAddrEx {
	/*! dma current target memory last time */
	uint8_t memTarget;
	/*! dma receive memory start address */
	uint8_t * addrStart;
	struct USART_RcvDMADataAddr data;
};
				
#endif
