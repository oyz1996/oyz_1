#ifndef _USART2_IN_H_
#define _USART2_IN_H_

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dma.h>

#include "usart2.h"
#include "freehead.h"

#define USART2_TX_RCC            RCC_AHB1Periph_GPIOD  
#define USART2_TX_GPIO           GPIOD
#define USART2_TX_PIN            GPIO_Pin_5
#define USART2_TX_PIN_SRC        GPIO_PinSource5

#define USART2_RX_RCC            RCC_AHB1Periph_GPIOD
#define USART2_RX_GPIO           GPIOD
#define USART2_RX_PIN            GPIO_Pin_6
#define USART2_RX_PIN_SRC        GPIO_PinSource6

#define USART2_PIN_RCC_INIT \
   do{\
      RCC_AHB1PeriphClockCmd(USART2_TX_RCC | USART2_RX_RCC,ENABLE);\
     }while(0)

#define USART2_GPIO_INIT   \
    do{\
       GPIO_InitTypeDef GPIOinitype;\
       GPIO_PinAFConfig(USART2_TX_GPIO,USART2_TX_PIN_SRC,GPIO_AF_USART2);\
       GPIO_PinAFConfig(USART2_RX_GPIO,USART2_RX_PIN_SRC,GPIO_AF_USART2);\
       \
       GPIOinitype.GPIO_Mode = GPIO_Mode_AF; \
       GPIOinitype.GPIO_Speed = GPIO_Speed_50MHz;\
       GPIOinitype.GPIO_OType = GPIO_OType_PP;\
       GPIOinitype.GPIO_PuPd = GPIO_PuPd_UP;\
       \
       GPIOinitype.GPIO_Pin = USART2_TX_PIN;\
       GPIO_Init(USART2_TX_GPIO,&GPIOinitype);\
       \
       GPIOinitype.GPIO_Pin = USART2_RX_PIN;\
       GPIO_Init(USART2_RX_GPIO,&GPIOinitype);\
      }while(0)

#define USART2_RCC_INIT \
      do{\
         RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);\
        }while(0)
      
#define USART2_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2
#define USART2_IRQ_SUB_PRIOR    0
        	
        
#define USART2_BaudRate        		 115200
#define USART2_Parity          		 USART_Parity_No


#define USART2_TX_DMA_RCC					RCC_AHB1Periph_DMA1
#define USART2_TX_DMA_STREAM 			DMA1_Stream6	
#define USART2_TX_DMA_CHANNEL   	DMA_Channel_4	
#define USART2_TX_DMA_IRQ					DMA1_Stream6_IRQn
#define USART2_TX_DMA_PRE_PRIOR		1
#define USART2_TX_DMA_SUB_PRIOR		0
#define USART2_TX_DMA_IRQHandler  DMA1_Stream6_IRQHandler
				
#define USART2_RX_DMA_RCC					RCC_AHB1Periph_DMA1
#define USART2_RX_DMA_STREAM			DMA1_Stream5
#define USART2_RX_DMA_CHANNEL			DMA_Channel_4
#define USART2_RX_DMA_IRQ					DMA1_Stream5_IRQn
#define USART2_RX_DMA_PRE_PRIOR		2
#define USART2_RX_DMA_SUB_PRIOR		0
#define USART2_RX_DMA_IRQHandler  DMA1_Stream1_IRQHandler

#define USART2_TX_DMA_BUF_SIZE		128
#define USART2_RX_DMA_BUF_SIZE		64


#define USART2_IDLE_No					0
#define USART2_IDLE_Yes				(!USART2_IDLE_No)		

struct USART2_Send
{
   uint16_t remain;
   
   uint16_t index;
   
   void * addr;
};

struct USART2_Receive
{
	uint16_t len;
	
	uint16_t idleindex;
	
	void *addr;
};

struct USART2_RcvDMADataAddrEx {
	/*! dma current target memory last time */
	uint8_t memTarget;
	/*! dma receive memory start address */
	uint8_t * addrStart;
	struct USART_RcvDMADataAddr data;
};

#endif

