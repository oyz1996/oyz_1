#include "usart6_in.h"

static void Choose_RS (uint8_t n)
{
	Set_GPIO(n,STEP_485_DE);
	Set_GPIO(n,STEP_485_RE);
}
static void USART6_RCC_Config(void)
{
   USART6_PIN_RCC_INIT;
   USART6_RCC_INIT;
}

static void USART6_GPIO_Config(void)
{
    USART6_GPIO_INIT;
}

static void USART6_NVIC_Config(void)
{
   NVIC_InitTypeDef NVICinit;
   
   NVICinit.NVIC_IRQChannel = USART6_IRQn;
   NVICinit.NVIC_IRQChannelPreemptionPriority = USART6_IRQ_PRE_PRIOR;
   NVICinit.NVIC_IRQChannelSubPriority = USART6_IRQ_SUB_PRIOR;
   NVICinit.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVICinit);
	
   USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
   USART_ClearITPendingBit(USART6,USART_IT_IDLE);
   USART_ClearFlag(USART6,USART_FLAG_IDLE);
   USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);
  
}

static void  USART6_Mode_Config(void)
{
   USART_DeInit(USART6);
   USART_InitTypeDef USART6init;
   
   USART6init.USART_BaudRate = USART6_BaudRate;
   USART6init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART6init.USART_Parity = USART6_Parity;
   USART6init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART6init.USART_StopBits = USART_StopBits_1;
   USART6init.USART_WordLength = USART_WordLength_8b;
   
   USART_Init(USART6,&USART6init);
}

void USART6_Config(void)
{
   USART6_RCC_Config();
   USART6_GPIO_Config();
   USART6_Mode_Config();
   USART6_NVIC_Config();
   USART_Cmd(USART6,ENABLE);
}

void USART6_Send_Array(void *addr,uint16_t len)
{
	uint8_t * point = (uint8_t *)addr;
	Choose_RS(1);
	for(int i = 0; i < len;i++)
	{
			USART_ClearFlag(USART6,USART_FLAG_TC);
			USART_SendData(USART6,point[i]);
			while(USART_GetFlagStatus(USART6,USART_FLAG_TC) == RESET);
	}
	Choose_RS(0);
}

static struct USART6_Receive   	USART6rece;
struct USART_HOOKHanler 	USART6hook;

void USART6_IRQHandler(void)
{
	 if (USART_GetFlagStatus(USART6, USART_FLAG_ORE) != RESET)  
    {  
        USART_ReceiveData(USART6);  
    }
		
	if(USART_GetITStatus(USART6,USART_IT_RXNE) != RESET)
	{
		uint8_t temp = USART_ReceiveData(USART6);
		USART6rece.addr[USART6rece.index] = temp;
		USART6rece.index++;
		USART6rece.idleindex = USART6_IDLE_No;
		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
	} 
	
	if(USART_GetITStatus(USART6,USART_IT_IDLE) != RESET)
	{
		uint8_t temp = USART_ReceiveData(USART6);
		uint8_t temp1 = USART6->SR;
		uint8_t temp3 = USART6->DR;
		
		USART6rece.idleindex = USART6_IDLE_Yes;
		if(USART6hook.tmcprocess != 0)
				USART6hook.tmcprocess(USART6rece.addr,&USART6rece.index,USART6rece.idleindex);

		USART_ClearITPendingBit(USART6,USART_IT_IDLE);
	} 
}


void USART6_VarInit(void)
{
	USART6rece.index = 0;
	USART6rece.idleindex = 0;
	
	USART6hook.tmcprocess = 0;
	USART6hook.receiveProcess = 0;
	USART6hook.prtmprocess = 0;
}
void USART6_Init(void)
{
	USART6_VarInit();

	USART6_Config();
}
