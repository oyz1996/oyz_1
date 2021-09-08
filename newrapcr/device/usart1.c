#include "usart1_in.h"


extern QueueHandle_t xSemaphore1;

static void USART1_RCC_Config(void)
{
   USART1_PIN_RCC_INIT;
   USART1_RCC_INIT;
}

static void USART1_GPIO_Config(void)
{
    USART1_GPIO_INIT;
}

static void USART1_NVIC_Config(void)
{
   NVIC_InitTypeDef NVICinit;
   
   NVICinit.NVIC_IRQChannel = USART1_IRQn;
   NVICinit.NVIC_IRQChannelPreemptionPriority = USART1_IRQ_PRE_PRIOR ;
   NVICinit.NVIC_IRQChannelSubPriority = USART1_IRQ_SUB_PRIOR;
   NVICinit.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVICinit);
   
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);
   
   USART_ClearFlag(USART1,USART_FLAG_IDLE);
   USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);
  
}
static void  USART1_Mode_Config(void)
{
   USART_DeInit(USART1);
   
   USART_OverSampling8Cmd(USART1,ENABLE);
   
   USART_InitTypeDef USART1init;
   
   USART1init.USART_BaudRate = USART1_BaudRate;
   USART1init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
   USART1init.USART_Parity = USART1_Parity;
   USART1init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART1init.USART_StopBits = USART_StopBits_1;
   USART1init.USART_WordLength = USART_WordLength_8b;
   
   USART_Init(USART1,&USART1init);
}

void USART1_Config(void)
{
   USART1_RCC_Config();
   USART1_GPIO_Config();
   USART1_Mode_Config();
   USART1_NVIC_Config();
   USART_Cmd(USART1,ENABLE);
}
   
static void USART1_DMA_RCC_Config(void)
{
   RCC_AHB1PeriphClockCmd(USART1_TX_DMA_RCC | USART1_RX_DMA_RCC,ENABLE);
}

static uint8_t usart1rxbuf0[USART1_RX_BUF_SIZE];
static uint8_t usart1rxbuf1[USART1_RX_BUF_SIZE];
static uint8_t usart1txbuf[USART1_TX_BUF_SIZE];

static void USART1_DMA_Mode_Config(void)
{
   DMA_DeInit(USART1_TX_DMA_STREAM);
   DMA_DeInit(USART1_RX_DMA_STREAM);
   DMA_InitTypeDef DMAinit;
   
   DMAinit.DMA_FIFOMode = DMA_FIFOMode_Disable;
   DMAinit.DMA_FIFOThreshold = DMA_FIFOStatus_1QuarterFull;
   DMAinit.DMA_MemoryBurst = DMA_MemoryBurst_Single;
   DMAinit.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
   DMAinit.DMA_PeripheralBaseAddr = (uint32_t)(&(USART1->DR));
   DMAinit.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
   DMAinit.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
   DMAinit.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMAinit.DMA_Priority = DMA_Priority_High;
   
   DMAinit.DMA_BufferSize = 0;
   DMAinit.DMA_Channel = USART1_TX_DMA_CHANNEL;
   DMAinit.DMA_Memory0BaseAddr = (uint32_t)usart1txbuf;
   DMAinit.DMA_Mode = DMA_Mode_Normal;
   DMAinit.DMA_DIR = DMA_DIR_MemoryToPeripheral;
   DMAinit.DMA_MemoryInc = DMA_MemoryInc_Enable;
   
   DMA_Init(USART1_TX_DMA_STREAM,&DMAinit);
//   DMA_Cmd(USART1_TX_DMA_CHANNEL,ENABLE);
   
   USART_DMACmd(USART1, USART_DMAReq_Tx,ENABLE);
   
   DMAinit.DMA_BufferSize = USART1_RX_BUF_SIZE;
   DMAinit.DMA_Channel = USART1_RX_DMA_CHANNEL;
   DMAinit.DMA_Memory0BaseAddr = (uint32_t)usart1rxbuf0;
   DMAinit.DMA_Mode = DMA_Mode_Circular;
   DMAinit.DMA_DIR = DMA_DIR_PeripheralToMemory;
   DMAinit.DMA_MemoryInc = DMA_MemoryInc_Enable;
   
   DMA_Init(USART1_RX_DMA_STREAM,&DMAinit);
   DMA_DoubleBufferModeConfig(USART1_RX_DMA_STREAM,(uint32_t)usart1rxbuf1,0);
   DMA_DoubleBufferModeCmd(USART1_RX_DMA_STREAM,ENABLE);
   
   USART_DMACmd(USART1, USART_DMAReq_Rx,ENABLE);
   
   DMA_Cmd(USART1_RX_DMA_STREAM,ENABLE);
	 while (DMA_GetCmdStatus(USART1_RX_DMA_STREAM) == DISABLE);
}

static void USART1_DMA_NVIC_Config(void)
{
   NVIC_InitTypeDef NVICinit;
   
   NVICinit.NVIC_IRQChannel = USART1_TX_DMA_IRQ;
   NVICinit.NVIC_IRQChannelPreemptionPriority = USART1_TX_DMA_PRE_PRIOR;
   NVICinit.NVIC_IRQChannelSubPriority = USART1_TX_DMA_SUB_PRIOR;
   NVICinit.NVIC_IRQChannelCmd = ENABLE;
   
   NVIC_Init(&NVICinit);
   
   NVICinit.NVIC_IRQChannel = USART1_RX_DMA_IRQ;
   NVICinit.NVIC_IRQChannelPreemptionPriority = USART1_RX_DMA_PRE_PRIOR;
   NVICinit.NVIC_IRQChannelSubPriority = USART1_RX_DMA_SUB_PRIOR;
   NVICinit.NVIC_IRQChannelCmd = ENABLE;
   
   NVIC_Init(&NVICinit);
   
   DMA_ClearFlag(USART1_TX_DMA_STREAM,DMA_FLAG_TCIF7);
   DMA_ITConfig(USART1_TX_DMA_STREAM,DMA_IT_TC,ENABLE);
 
	DMA_ClearFlag(USART1_RX_DMA_STREAM, DMA_FLAG_TCIF1);
   
}

void USART1_DMA_Config(void)
{
   USART1_DMA_RCC_Config();
   USART1_DMA_Mode_Config();
   USART1_DMA_NVIC_Config();
}

struct USART1_Send usart1_dma_send;


uint8_t USART1_DMA_Send(void *addr , uint8_t len)
{
   if( !addr || !len)
   {
      return 1;  
   }

	 while (DMA_GetCurrDataCounter(USART1_TX_DMA_STREAM));
	 
   uint8_t sendsize = 0 ;
   if(len > USART1_TX_BUF_SIZE)
   {
      sendsize = USART1_TX_BUF_SIZE;
      usart1_dma_send.remain = len - USART1_TX_BUF_SIZE;
      usart1_dma_send.index = USART1_TX_BUF_SIZE;
   }
   else
   {
      sendsize = len;
      usart1_dma_send.remain = 0;
      usart1_dma_send.index = 0;
   }
   usart1_dma_send.addr = addr;
  
   USART1_TX_DMA_STREAM->M0AR = (uint32_t)(usart1_dma_send.addr);

   DMA_SetCurrDataCounter(USART1_TX_DMA_STREAM,sendsize);
   DMA_Cmd(USART1_TX_DMA_STREAM,ENABLE);
   while (DMA_GetCmdStatus(USART1_TX_DMA_STREAM) != DISABLE);
   return 0;
}
struct USART1_RcvDMADataAddrEx rcvDMADataAddrEx;

static int32_t USART1_RcvDMADataAddrCal(uint8_t memTarget, uint16_t dataCnt, 
	struct USART_RcvDMADataAddr * data)
{
	uint8_t * buf = rcvDMADataAddrEx.memTarget == 0 ? usart1rxbuf0 : usart1rxbuf1;
	uint8_t * buf1 = rcvDMADataAddrEx.memTarget == 0 ? usart1rxbuf1 : usart1rxbuf0;

	
	if (rcvDMADataAddrEx.memTarget == memTarget) {
		/* buffer roll back */
		if (buf + USART1_RX_BUF_SIZE - dataCnt <= rcvDMADataAddrEx.addrStart) {
			rcvDMADataAddrEx.addrStart = buf + USART1_RX_BUF_SIZE - dataCnt;
			rcvDMADataAddrEx.memTarget = memTarget;
			return -1;
		}
		data->addr = rcvDMADataAddrEx.addrStart;
		//uint8_t * tmp = memTarget == 0 ? usart6RxBuf : usart6RxBuf1;
		data->len = buf + USART1_RX_BUF_SIZE - dataCnt - rcvDMADataAddrEx.addrStart;
		data->addr1 = 0;
		data->len1 = 0;
		rcvDMADataAddrEx.addrStart += data->len;
	} else {
		data->addr = rcvDMADataAddrEx.addrStart;
		data->addr1 = buf1;
		data->len = buf + USART1_RX_BUF_SIZE - rcvDMADataAddrEx.addrStart;
		data->len1 = USART1_RX_BUF_SIZE - dataCnt;
		rcvDMADataAddrEx.addrStart = buf1 + data->len1;
	}

	rcvDMADataAddrEx.memTarget = memTarget;

	return 0;
}


void USART1_TX_DMA_IRQHandler(void)
{
   if(DMA_GetFlagStatus(USART1_TX_DMA_STREAM,DMA_FLAG_TCIF7)!=RESET)
	{ 
		DMA_ClearFlag(USART1_TX_DMA_STREAM,DMA_FLAG_TCIF7);
      DMA_Cmd(USART1_TX_DMA_STREAM, DISABLE); 
      if(usart1_dma_send.remain)
      {
         uint8_t sendsize = 0;
         uint32_t * addr = 0;
         if(usart1_dma_send.remain >USART1_TX_BUF_SIZE)
         {
            sendsize = USART1_TX_BUF_SIZE;
            usart1_dma_send.index += USART1_TX_BUF_SIZE;
            usart1_dma_send.remain += usart1_dma_send.remain - USART1_TX_BUF_SIZE;     
         }
         else
         {
            sendsize = usart1_dma_send.remain;
            usart1_dma_send.index = 0;
            usart1_dma_send.remain = 0;
         }
            addr = usart1_dma_send.addr;
            usart1_dma_send.addr =  addr+USART1_TX_BUF_SIZE;
            USART1_TX_DMA_STREAM->M0AR = (uint32_t)(usart1_dma_send.addr);
            DMA_SetCurrDataCounter(USART1_TX_DMA_STREAM,sendsize);
            DMA_Cmd(USART1_TX_DMA_STREAM,ENABLE);
				 while (DMA_GetCmdStatus(USART1_TX_DMA_STREAM) != DISABLE);
      }
	}
   
}

void USART1_IRQHandler(void)
{
  // uint8_t res = 0;
   if(USART_GetITStatus(USART1,USART_IT_TC) != RESET)
   {
      USART_ClearITPendingBit(USART1, USART_IT_TC);
   }
   
   if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
   {

      uint8_t tmp = USART_ReceiveData(USART1);
		
		 		if(	USART1_RcvDMADataAddrCal(DMA_GetCurrentMemoryTarget(USART1_RX_DMA_STREAM), 
										DMA_GetCurrDataCounter(USART1_RX_DMA_STREAM),
										&rcvDMADataAddrEx.data) == 0)
		{
			if(usart1Hook.receiveProcess)
				usart1Hook.receiveProcess(&rcvDMADataAddrEx.data,0);
		}

		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
   }
   
}


struct USART_HOOKHanler usart1Hook;
void USART1_VarInit(void)
{
	memset(usart1rxbuf0, 0, sizeof(usart1rxbuf0));
	memset(usart1rxbuf1, 0, sizeof(usart1rxbuf1));
	memset(usart1txbuf, 0, sizeof(usart1txbuf));

	usart1Hook.tmcprocess = 0;
	usart1Hook.receiveProcess = 0;
	usart1Hook.prtmprocess = 0;

	usart1_dma_send.addr = 0;
	usart1_dma_send.index = 0;
	usart1_dma_send.remain = 0;

	rcvDMADataAddrEx.addrStart = usart1rxbuf0;
	rcvDMADataAddrEx.memTarget = 0;

}

void USART1_Init(void)
{
	USART1_Config();
	USART1_DMA_Config();
	USART1_VarInit();
}
