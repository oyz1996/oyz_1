#ifndef _USART1_IN_H_
#define _USART1_IN_H_

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dma.h>
#include <string.h>
#include "usart1.h"

 
#define USART1_TX_RCC            RCC_AHB1Periph_GPIOA  
#define USART1_TX_GPIO           GPIOA
#define USART1_TX_PIN            GPIO_Pin_9
#define USART1_TX_PIN_SRC        GPIO_PinSource9

#define USART1_RX_RCC            RCC_AHB1Periph_GPIOA
#define USART1_RX_GPIO           GPIOA
#define USART1_RX_PIN            GPIO_Pin_10
#define USART1_RX_PIN_SRC        GPIO_PinSource10

#define USART1_PIN_RCC_INIT \
   do{\
      RCC_AHB1PeriphClockCmd(USART1_TX_RCC | USART1_RX_RCC,ENABLE);\
     }while(0)

#define USART1_GPIO_INIT   \
    do{\
       GPIO_InitTypeDef GPIOinitype;\
       GPIO_PinAFConfig(USART1_TX_GPIO,USART1_TX_PIN_SRC,GPIO_AF_USART1);\
       GPIO_PinAFConfig(USART1_RX_GPIO,USART1_RX_PIN_SRC,GPIO_AF_USART1);\
       GPIOinitype.GPIO_Mode = GPIO_Mode_AF; \
       GPIOinitype.GPIO_Speed = GPIO_Speed_50MHz;\
       GPIOinitype.GPIO_OType = GPIO_OType_PP;\
       GPIOinitype.GPIO_PuPd = GPIO_PuPd_UP;\
       GPIOinitype.GPIO_Pin = USART1_TX_PIN;\
       GPIO_Init(USART1_TX_GPIO,&GPIOinitype);\
       GPIOinitype.GPIO_Pin = USART1_RX_PIN;\
       GPIO_Init(USART1_RX_GPIO,&GPIOinitype);\
      }while(0)

#define USART1_RCC_INIT \
      do{\
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);\
        }while(0)
      
#define USART1_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1
#define USART1_IRQ_SUB_PRIOR    0
        
#define USART1_BaudRate         9600
#define USART1_Parity           USART_Parity_No
   
#define USART1_TX_DMA_RCC         RCC_AHB1Periph_DMA2
#define USART1_TX_DMA_STREAM      DMA2_Stream7    
#define USART1_TX_DMA_CHANNEL     DMA_Channel_4
#define USART1_TX_DMA_IRQ         DMA2_Stream7_IRQn
#define USART1_TX_DMA_PRE_PRIOR   2
#define USART1_TX_DMA_SUB_PRIOR  	0
#define USART1_TX_DMA_IRQHandler  DMA2_Stream7_IRQHandler
        
#define USART1_RX_DMA_RCC         RCC_AHB1Periph_DMA2    
#define USART1_RX_DMA_STREAM      DMA2_Stream2
#define USART1_RX_DMA_CHANNEL     DMA_Channel_4    
#define USART1_RX_DMA_IRQ         DMA2_Stream2_IRQn
#define USART1_RX_DMA_PRE_PRIOR   3
#define USART1_RX_DMA_SUB_PRIOR   0
#define USART1_RX_DMA_IRQHandler  DMA2_Stream2_IRQHandler

#define USART1_TX_BUF_SIZE        254
#define USART1_RX_BUF_SIZE        512        

struct USART1_Send
{
   uint16_t remain;
   
   uint16_t index;
   
   void * addr;
};

struct USART1_RcvDMADataAddrEx {
	/*! dma current target memory last time */
	uint8_t memTarget;
	/*! dma receive memory start address */
	uint8_t * addrStart;
	struct USART_RcvDMADataAddr data;
};
#endif
