#ifndef _USART6_IN_H_
#define _USART6_IN_H_

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_dma.h>

#include "usart6.h"


#define USART6_TX_RCC            RCC_AHB1Periph_GPIOC  
#define USART6_TX_GPIO           GPIOC
#define USART6_TX_PIN            GPIO_Pin_6
#define USART6_TX_PIN_SRC        GPIO_PinSource6

#define USART6_RX_RCC            RCC_AHB1Periph_GPIOC
#define USART6_RX_GPIO           GPIOC
#define USART6_RX_PIN            GPIO_Pin_7
#define USART6_RX_PIN_SRC        GPIO_PinSource7

#define USART6_PIN_RCC_INIT \
   do{\
      RCC_AHB1PeriphClockCmd(USART6_TX_RCC | USART6_RX_RCC,ENABLE);\
     }while(0)

#define USART6_GPIO_INIT   \
    do{\
       GPIO_InitTypeDef GPIOinitype;\
       GPIO_PinAFConfig(USART6_TX_GPIO,USART6_TX_PIN_SRC,GPIO_AF_USART6);\
       GPIO_PinAFConfig(USART6_RX_GPIO,USART6_RX_PIN_SRC,GPIO_AF_USART6);\
			\
       GPIOinitype.GPIO_Mode = GPIO_Mode_AF; \
       GPIOinitype.GPIO_Speed = GPIO_Speed_50MHz;\
       GPIOinitype.GPIO_OType = GPIO_OType_PP;\
       GPIOinitype.GPIO_PuPd = GPIO_PuPd_UP;\
			\
       GPIOinitype.GPIO_Pin = USART6_TX_PIN;\
       GPIO_Init(USART6_TX_GPIO,&GPIOinitype);\
			\
       GPIOinitype.GPIO_Pin = USART6_RX_PIN;\
       GPIO_Init(USART6_RX_GPIO,&GPIOinitype);\
      }while(0)

#define USART6_RCC_INIT \
      do{\
         RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);\
        }while(0)
      
#define USART6_IRQ_PRE_PRIOR    configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3
#define USART6_IRQ_SUB_PRIOR    0
        
#define USART6_BaudRate         9600
#define USART6_Parity           USART_Parity_No

#define USART6_RX_MAX_SIZE			 255
#define USART6_TX_MAX_SIZE			 255

#define USART6_IDLE_No					0
#define USART6_IDLE_Yes				(!USART6_IDLE_No)				
				
			
struct USART6_Receive
{
	uint16_t  idleindex;
	
	uint16_t index;
	
	uint8_t addr[USART6_RX_MAX_SIZE];
};



				
#endif



