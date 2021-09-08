#include "usart2_in.h"


static void USART2_RCC_Config(void)
{
   USART2_PIN_RCC_INIT;
   USART2_RCC_INIT;
}

static void USART2_GPIO_Config(void)
{
    USART2_GPIO_INIT;
}

static void USART2_NVIC_Config(void)
{
   NVIC_InitTypeDef NVICinit;
   
   NVICinit.NVIC_IRQChannel = USART2_IRQn;
   NVICinit.NVIC_IRQChannelPreemptionPriority = USART2_IRQ_PRE_PRIOR;
   NVICinit.NVIC_IRQChannelSubPriority = USART2_IRQ_SUB_PRIOR;
   NVICinit.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVICinit);
   
	 USART_ClearFlag(USART2, USART_FLAG_TC);
	 USART_ITConfig(USART2, USART_IT_TC, ENABLE);
   
   USART_ClearFlag(USART2,USART_FLAG_IDLE);
   USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);
  
}

static void  USART2_Mode_Config(void)
{
   USART_DeInit(USART2);
   USART_InitTypeDef USART2init;
   
   USART2init.USART_BaudRate = USART2_BaudRate;
   USART2init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART2init.USART_Parity = USART2_Parity;
   USART2init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART2init.USART_StopBits = USART_StopBits_1;
   USART2init.USART_WordLength = USART_WordLength_8b;
   
   USART_Init(USART2,&USART2init);
}
void USART2_Config(void)
{
   USART2_RCC_Config();
   USART2_GPIO_Config();
   USART2_Mode_Config();
   USART2_NVIC_Config();
   USART_Cmd(USART2,ENABLE);
}
     
static void USART2_DMA_RCC_Config(void)
{
	RCC_AHB1PeriphClockCmd( USART2_TX_DMA_RCC | USART2_RX_DMA_RCC,ENABLE);
}

static uint8_t USART2rxbuf0[USART2_RX_DMA_BUF_SIZE];
static uint8_t USART2rxbuf1[USART2_RX_DMA_BUF_SIZE];
static uint8_t USART2txbuf[USART2_RX_DMA_BUF_SIZE];

static void USART2_DMA_Mode_Config(void)
{
	DMA_DeInit(USART2_TX_DMA_STREAM);
	DMA_DeInit(USART2_RX_DMA_STREAM);

	DMA_InitTypeDef DMAinit;
	
	DMAinit.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMAinit.DMA_FIFOThreshold = DMA_FIFOStatus_1QuarterFull;
	DMAinit.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMAinit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMAinit.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR));
	DMAinit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMAinit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMAinit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMAinit.DMA_Priority = DMA_Priority_High;
	
	DMAinit.DMA_BufferSize = 0;
	DMAinit.DMA_Channel = USART2_TX_DMA_CHANNEL;
	DMAinit.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMAinit.DMA_Memory0BaseAddr = (uint32_t)USART2txbuf;
	DMAinit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMAinit.DMA_Mode = DMA_Mode_Normal;
	DMA_Init(USART2_TX_DMA_STREAM,&DMAinit);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	
	DMAinit.DMA_BufferSize = USART2_RX_DMA_BUF_SIZE;
	DMAinit.DMA_Channel = USART2_RX_DMA_CHANNEL;
	DMAinit.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMAinit.DMA_Memory0BaseAddr = (uint32_t)USART2rxbuf0;
	DMAinit.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMAinit.DMA_Mode = DMA_Mode_Circular;
  DMA_Init(USART2_RX_DMA_STREAM,&DMAinit);

	DMA_DoubleBufferModeConfig(USART2_RX_DMA_STREAM,(uint32_t)USART2rxbuf1,0);
	DMA_DoubleBufferModeCmd(USART2_RX_DMA_STREAM,ENABLE);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(USART2_RX_DMA_STREAM,ENABLE);
	
	 while (DMA_GetCmdStatus(USART2_RX_DMA_STREAM) == DISABLE);
}
static void USART2_DMA_NVIC_Config(void)
{
	NVIC_InitTypeDef	NVICinit;
	
	NVICinit.NVIC_IRQChannel = USART2_TX_DMA_IRQ;
	NVICinit.NVIC_IRQChannelPreemptionPriority = USART2_TX_DMA_PRE_PRIOR;
	NVICinit.NVIC_IRQChannelSubPriority = USART2_TX_DMA_SUB_PRIOR;
	NVICinit.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICinit);

	
	NVICinit.NVIC_IRQChannel = USART2_RX_DMA_IRQ;
	NVICinit.NVIC_IRQChannelPreemptionPriority = USART2_RX_DMA_PRE_PRIOR;
	NVICinit.NVIC_IRQChannelSubPriority = USART2_RX_DMA_SUB_PRIOR;
	NVICinit.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVICinit);
	
	DMA_ClearFlag(USART2_TX_DMA_STREAM,DMA_FLAG_TCIF6);
	DMA_ITConfig(USART2_TX_DMA_STREAM,DMA_IT_TC,ENABLE);
	
	DMA_ClearFlag(USART2_RX_DMA_STREAM, DMA_FLAG_TCIF1);
	
}
static struct USART2_Send USART2sendstr;

uint8_t USART2_DMA_Send(void *addr,uint16_t len)
{
	if(!addr || !len)
	{
		return 1;
	}
	while(DMA_GetCurrDataCounter(USART2_TX_DMA_STREAM));
	
	uint16_t  sendsize = 0;
	if(len > USART2_TX_DMA_BUF_SIZE)
	{
		sendsize = USART2_TX_DMA_BUF_SIZE;
		USART2sendstr.remain = len - USART2_TX_DMA_BUF_SIZE;
		USART2sendstr.index = USART2_TX_DMA_BUF_SIZE;
	}
	else
	{
		sendsize = len ;
		USART2sendstr.remain = 0;
		USART2sendstr.index = 0;
	}
	USART2sendstr.addr = addr;
	
	USART2_TX_DMA_STREAM->M0AR = (uint32_t)USART2sendstr.addr;
	DMA_SetCurrDataCounter(USART2_TX_DMA_STREAM,sendsize);
	DMA_Cmd(USART2_TX_DMA_STREAM,ENABLE);
  while (DMA_GetCmdStatus(USART2_TX_DMA_STREAM) != DISABLE);

  return 0;
}

void USART2_TX_DMA_IRQHandler(void)
{
   if(DMA_GetFlagStatus(USART2_TX_DMA_STREAM,DMA_FLAG_TCIF6)!=RESET)
	{ 
			DMA_ClearFlag(USART2_TX_DMA_STREAM,DMA_FLAG_TCIF6);
      DMA_Cmd(USART2_TX_DMA_STREAM, DISABLE); 
      if(USART2sendstr.remain)
      {
         uint8_t sendsize = 0;
         uint32_t * addr = 0;
         if(USART2sendstr.remain >USART2_TX_DMA_BUF_SIZE)
         {
            sendsize = USART2_TX_DMA_BUF_SIZE;
            USART2sendstr.index += USART2_TX_DMA_BUF_SIZE;
            USART2sendstr.remain += USART2sendstr.remain - USART2_TX_DMA_BUF_SIZE;     
         }
         else
         {
            sendsize = USART2sendstr.remain;
            USART2sendstr.index = 0;
            USART2sendstr.remain = 0;
         }
            addr = USART2sendstr.addr;
            USART2sendstr.addr =  addr+USART2_TX_DMA_BUF_SIZE;
            USART2_TX_DMA_STREAM->M0AR = (uint32_t)(USART2sendstr.addr);
            DMA_SetCurrDataCounter(USART2_TX_DMA_STREAM,sendsize);
            DMA_Cmd(USART2_TX_DMA_STREAM,ENABLE);
				  while (DMA_GetCmdStatus(USART2_TX_DMA_STREAM) != DISABLE);
      }   
	}  
}

static struct USART2_Receive usartrece;
struct USART_HOOKHanler usart2hook;
struct USART2_RcvDMADataAddrEx  USART2rcv;

static int32_t USART2_RcvDMADataAddrCal(uint8_t memTarget, uint16_t dataCnt, 
	struct USART_RcvDMADataAddr * data)
{
	uint8_t * buf = USART2rcv.memTarget == 0 ? USART2rxbuf0 : USART2rxbuf1;
	uint8_t * buf1 = USART2rcv.memTarget == 0 ? USART2rxbuf1 : USART2rxbuf0;

	
	if (USART2rcv.memTarget == memTarget) {
		/* buffer roll back */
		if (buf + USART2_RX_DMA_BUF_SIZE - dataCnt <= USART2rcv.addrStart) {
			USART2rcv.addrStart = buf + USART2_RX_DMA_BUF_SIZE - dataCnt;
			USART2rcv.memTarget = memTarget;
			return -1;
		}
		data->addr = USART2rcv.addrStart;
		//uint8_t * tmp = memTarget == 0 ? usart6RxBuf : usart6RxBuf1;
		data->len = buf + USART2_RX_DMA_BUF_SIZE - dataCnt - USART2rcv.addrStart;
		data->addr1 = 0;
		data->len1 = 0;
		USART2rcv.addrStart += data->len;
	} else {
		data->addr = USART2rcv.addrStart;
		data->addr1 = buf1;
		data->len = buf + USART2_RX_DMA_BUF_SIZE - USART2rcv.addrStart;
		data->len1 = USART2_RX_DMA_BUF_SIZE - dataCnt;
		USART2rcv.addrStart = buf1 + data->len1;
	}

	USART2rcv.memTarget = memTarget;

	return 0;
}
void USART2_IRQHandler(void)
{
	 if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  
    {  
        USART_ReceiveData(USART2);  
    }
	if(USART_GetITStatus(USART2,USART_IT_TC) != RESET)
	{
		usartrece.idleindex = USART2_IDLE_No;
		USART_ClearITPendingBit(USART2,USART_IT_TC);
	}
	
	if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
	{
		uint8_t temp = USART_ReceiveData(USART2);

		usartrece.idleindex = USART2_IDLE_Yes;
		if(	USART2_RcvDMADataAddrCal(DMA_GetCurrentMemoryTarget(USART2_RX_DMA_STREAM), 
										DMA_GetCurrDataCounter(USART2_RX_DMA_STREAM),
										&USART2rcv.data) == 0)
		{
			if(usart2hook.prtmprocess)
				usart2hook.prtmprocess(&USART2rcv.data,usartrece.idleindex);
		}
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
	}
   
}

void USART2_DMA_Init(void)
{
	USART2_DMA_RCC_Config();
	USART2_DMA_Mode_Config();
	USART2_DMA_NVIC_Config();
}
void USART2_VarInit(void)
{
	usartrece.addr = 0;
	usartrece.idleindex = 0;
	usartrece.len = 0;
	
	usart2hook.receiveProcess = 0;
	usart2hook.prtmprocess = 0;
	usart2hook.tmcprocess = 0;
	
	USART2rcv.addrStart = USART2rxbuf0;
	USART2rcv.memTarget = 0;
}
void USART2_Init(void)
{
	USART2_VarInit();
	USART2_Config();
	USART2_DMA_Init();
}
