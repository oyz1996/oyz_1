#include "usart3_in.h"


static void USART3_RCC_Config(void)
{
   USART3_PIN_RCC_INIT;
   USART3_RCC_INIT;
}

static void USART3_GPIO_Config(void)
{
    USART3_GPIO_INIT;
}

static void USART3_NVIC_Config(void)
{
   NVIC_InitTypeDef NVICinit;
   
   NVICinit.NVIC_IRQChannel = USART3_IRQn;
   NVICinit.NVIC_IRQChannelPreemptionPriority = USART3_IRQ_PRE_PRIOR;
   NVICinit.NVIC_IRQChannelSubPriority = USART3_IRQ_SUB_PRIOR;
   NVICinit.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVICinit);
   
	 USART_ClearFlag(USART3, USART_FLAG_TC);
	 USART_ITConfig(USART3, USART_IT_TC, ENABLE);
   
   USART_ClearFlag(USART3,USART_FLAG_IDLE);
   USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
  
}

static void  USART3_Mode_Config(void)
{
   USART_DeInit(USART3);
	USART_OverSampling8Cmd(USART3,ENABLE);
   USART_InitTypeDef USART3init;
   
   USART3init.USART_BaudRate = USART3_BaudRate;
   USART3init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART3init.USART_Parity = USART3_Parity;
   USART3init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART3init.USART_StopBits = USART_StopBits_1;
   USART3init.USART_WordLength = USART_WordLength_8b;
   
   USART_Init(USART3,&USART3init);
}
void USART3_Config(void)
{
   USART3_RCC_Config();
   USART3_GPIO_Config();
   USART3_Mode_Config();
   USART3_NVIC_Config();
   USART_Cmd(USART3,ENABLE);
}

static void USART3_DMA_RCC_Config(void)
{
	RCC_AHB1PeriphClockCmd( USART3_TX_DMA_RCC | USART3_RX_DMA_RCC,ENABLE);
}

static uint8_t usart3rxbuf0[USART3_RX_DMA_BUF_SIZE];
static uint8_t usart3rxbuf1[USART3_RX_DMA_BUF_SIZE];
static uint8_t usart3txbuf[USART3_RX_DMA_BUF_SIZE];

static void USART3_DMA_Mode_Config(void)
{
	DMA_DeInit(USART3_TX_DMA_STREAM);
	DMA_DeInit(USART3_RX_DMA_STREAM);

	DMA_InitTypeDef DMAinit;
	
	DMAinit.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMAinit.DMA_FIFOThreshold = DMA_FIFOStatus_1QuarterFull;
	DMAinit.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMAinit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMAinit.DMA_PeripheralBaseAddr = (uint32_t)(&(USART3->DR));
	DMAinit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMAinit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMAinit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMAinit.DMA_Priority = DMA_Priority_High;
	
	DMAinit.DMA_BufferSize = 0;
	DMAinit.DMA_Channel = USART3_TX_DMA_CHANNEL;
	DMAinit.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMAinit.DMA_Memory0BaseAddr = (uint32_t)usart3txbuf;
	DMAinit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMAinit.DMA_Mode = DMA_Mode_Normal;
	DMA_Init(USART3_TX_DMA_STREAM,&DMAinit);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	
	DMAinit.DMA_BufferSize = USART3_RX_DMA_BUF_SIZE;
	DMAinit.DMA_Channel = USART3_RX_DMA_CHANNEL;
	DMAinit.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMAinit.DMA_Memory0BaseAddr = (uint32_t)usart3rxbuf0;
	DMAinit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMAinit.DMA_Mode = DMA_Mode_Circular;
  DMA_Init(USART3_RX_DMA_STREAM,&DMAinit);

	DMA_DoubleBufferModeConfig(USART3_RX_DMA_STREAM,(uint32_t)usart3rxbuf1,0);
	DMA_DoubleBufferModeCmd(USART3_RX_DMA_STREAM,ENABLE);
	
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(USART3_RX_DMA_STREAM,ENABLE);
	
	 while (DMA_GetCmdStatus(USART3_RX_DMA_STREAM) == DISABLE);
}
static void USART3_DMA_NVIC_Config(void)
{
	NVIC_InitTypeDef	NVICinit;
	
	NVICinit.NVIC_IRQChannel = USART3_TX_DMA_IRQ;
	NVICinit.NVIC_IRQChannelPreemptionPriority = USART3_TX_DMA_PRE_PRIOR;
	NVICinit.NVIC_IRQChannelSubPriority = USART3_TX_DMA_SUB_PRIOR;
	NVICinit.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICinit);

	
	NVICinit.NVIC_IRQChannel = USART3_RX_DMA_IRQ;
	NVICinit.NVIC_IRQChannelPreemptionPriority = USART3_RX_DMA_PRE_PRIOR;
	NVICinit.NVIC_IRQChannelSubPriority = USART3_RX_DMA_SUB_PRIOR;
	NVICinit.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICinit);
	
	DMA_ClearFlag(USART3_TX_DMA_STREAM,DMA_FLAG_TCIF3);
	DMA_ITConfig(USART3_TX_DMA_STREAM,DMA_IT_TC,ENABLE);
	
	DMA_ClearFlag(USART3_RX_DMA_STREAM, DMA_FLAG_TCIF1);
	
}
static struct USART3_Send usart3sendstr;

uint8_t USART3_DMA_Send(void *addr,uint16_t len)
{
	if(!addr || !len)
	{
		return 1;
	}
	while(DMA_GetCurrDataCounter(USART3_TX_DMA_STREAM));
	
	uint16_t  sendsize = 0;
	if(len > USART3_TX_DMA_BUF_SIZE)
	{
		sendsize = USART3_TX_DMA_BUF_SIZE;
		usart3sendstr.remain = len - USART3_TX_DMA_BUF_SIZE;
		usart3sendstr.index = USART3_TX_DMA_BUF_SIZE;
	}
	else
	{
		sendsize = len ;
		usart3sendstr.remain = 0;
		usart3sendstr.index = 0;
	}
	usart3sendstr.addr = addr;
	
	USART3_TX_DMA_STREAM->M0AR = (uint32_t)usart3sendstr.addr;
	DMA_SetCurrDataCounter(USART3_TX_DMA_STREAM,sendsize);
	DMA_Cmd(USART3_TX_DMA_STREAM,ENABLE);
  while (DMA_GetCmdStatus(USART3_TX_DMA_STREAM) != DISABLE);
  return 0;
}

void USART3_TX_DMA_IRQHandler(void)
{
   if(DMA_GetFlagStatus(USART3_TX_DMA_STREAM,DMA_FLAG_TCIF3)!=RESET)
	{ 
			DMA_ClearFlag(USART3_TX_DMA_STREAM,DMA_FLAG_TCIF3);
      DMA_Cmd(USART3_TX_DMA_STREAM, DISABLE); 
      if(usart3sendstr.remain)
      {
         uint8_t sendsize = 0;
         uint32_t * addr = 0;
         if(usart3sendstr.remain >USART3_TX_DMA_BUF_SIZE)
         {
            sendsize = USART3_TX_DMA_BUF_SIZE;
            usart3sendstr.index += USART3_TX_DMA_BUF_SIZE;
            usart3sendstr.remain += usart3sendstr.remain - USART3_TX_DMA_BUF_SIZE;     
         }
         else
         {
            sendsize = usart3sendstr.remain;
            usart3sendstr.index = 0;
            usart3sendstr.remain = 0;
         }
            addr = usart3sendstr.addr;
            usart3sendstr.addr =  addr+USART3_TX_DMA_BUF_SIZE;
            USART3_TX_DMA_STREAM->M0AR = (uint32_t)(usart3sendstr.addr);
            DMA_SetCurrDataCounter(USART3_TX_DMA_STREAM,sendsize);
            DMA_Cmd(USART3_TX_DMA_STREAM,ENABLE);
				  while (DMA_GetCmdStatus(USART3_TX_DMA_STREAM) != DISABLE);
      }
      
	}
   
}
static struct USART3_Receive usartrece;
struct USART_HOOKHanler usart3hook;
struct USART3_RcvDMADataAddrEx  usart3rcv;

static int32_t USART3_RcvDMADataAddrCal(uint8_t memTarget, uint16_t dataCnt, 
	struct USART_RcvDMADataAddr * data)
{
	uint8_t * buf = usart3rcv.memTarget == 0 ? usart3rxbuf0 : usart3rxbuf1;
	uint8_t * buf1 = usart3rcv.memTarget == 0 ? usart3rxbuf1 : usart3rxbuf0;

	
	if (usart3rcv.memTarget == memTarget) {
		/* buffer roll back */
		if (buf + USART3_RX_DMA_BUF_SIZE - dataCnt <= usart3rcv.addrStart) {
			usart3rcv.addrStart = buf + USART3_RX_DMA_BUF_SIZE - dataCnt;
			usart3rcv.memTarget = memTarget;
			return -1;
		}
		data->addr = usart3rcv.addrStart;
		//uint8_t * tmp = memTarget == 0 ? usart6RxBuf : usart6RxBuf1;
		data->len = buf + USART3_RX_DMA_BUF_SIZE - dataCnt - usart3rcv.addrStart;
		data->addr1 = 0;
		data->len1 = 0;
		usart3rcv.addrStart += data->len;
	} else {
		data->addr = usart3rcv.addrStart;
		data->addr1 = buf1;
		data->len = buf + USART3_RX_DMA_BUF_SIZE - usart3rcv.addrStart;
		data->len1 = USART3_RX_DMA_BUF_SIZE - dataCnt;
		usart3rcv.addrStart = buf1 + data->len1;
	}

	usart3rcv.memTarget = memTarget;

	return 0;
}
void USART3_IRQHandler(void)
{
	 if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)  
    {  
        USART_ReceiveData(USART3);  
    }
	if(USART_GetITStatus(USART3,USART_IT_TC) != RESET)
	{
		usartrece.idleindex = USART3_IDLE_No;
		USART_ClearITPendingBit(USART3,USART_IT_TC);
	}
	
	if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)
	{
		uint8_t temp = USART_ReceiveData(USART3);

		usartrece.idleindex = USART3_IDLE_Yes;
		if(	USART3_RcvDMADataAddrCal(DMA_GetCurrentMemoryTarget(USART3_RX_DMA_STREAM), 
										DMA_GetCurrDataCounter(USART3_RX_DMA_STREAM),
										&usart3rcv.data) == 0)
		{
			if(usart3hook.prtmprocess)
				usart3hook.prtmprocess(&usart3rcv.data,usartrece.idleindex);
		}
		USART_ClearITPendingBit(USART3,USART_IT_IDLE);
	}
   
}

void USART3_DMA_Init(void)
{
	USART3_DMA_RCC_Config();
	USART3_DMA_Mode_Config();
	USART3_DMA_NVIC_Config();
}
void USART3_VarInit(void)
{
	usartrece.addr = 0;
	usartrece.idleindex = 0;
	usartrece.len = 0;
	
	usart3hook.receiveProcess = 0;
	usart3hook.prtmprocess = 0;
	usart3hook.tmcprocess = 0;
	
	usart3rcv.addrStart = usart3rxbuf0;
	usart3rcv.memTarget = 0;
}
void USART3_Init(void)
{
	USART3_VarInit();
	USART3_Config();
	USART3_DMA_Init();
}
