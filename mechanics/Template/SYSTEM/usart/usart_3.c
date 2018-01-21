#include "usart_3.h"


/*
USART3_TX      ->  PB10
USART3_RX      ->  PB11
USART3_RX_DMA  ->  Channel3
USART3_Base       0x40004800
USART_DR Offset   0x04
*/

#define USART3_DR_Address ((u32)0x40004804)     //USART3-DR 地址
#define RxBufferSize_3 10
__IO u8 USART3_RxBuffer[RxBufferSize_3];

 #define TxBufferSize_3 8
u8 USART3_TxBuffer[TxBufferSize_3]={0x12,0x34,0x56,0x78,0X90,0xab,0xcd,0xef};


void USART3_DMA_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//Clock_Enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	//GPIO_Init
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	//DMA_IRQ_Init  USART_RX -> Channel3
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//USART3_DMA_Init
	DMA_DeInit(DMA1_Channel3); 												// 
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Address;			//DMA通道1的地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_RxBuffer;		//DMA传送地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//传送方向 USART是外设
	DMA_InitStructure.DMA_BufferSize = RxBufferSize_3;						//传送内存大小
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//传送源地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//传送内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//源地址的数据长度是8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//传送的目的地址是8位宽度
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//传送模式循环
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//优先级设置
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMA通道没有设置为内存到内存传送 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	//USART3_init
	USART_InitStructure.USART_BaudRate = 115200;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	//Enable USARTy DMA Rx request
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	//允许DMA1通道1传输结束中断
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC, ENABLE);
	//Enable USARTy RX DMA1 Channel 
	DMA_Cmd(DMA1_Channel3, ENABLE);
   
  USART_Cmd(USART3, ENABLE);                    //使能串口
}

void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_GL3))
	{
		DMA_ClearITPendingBit(DMA1_IT_GL3);
		USART3_Sendstring(USART3_TxBuffer,TxBufferSize_3);
		
	}

}

void USART3_SendByte(u8 data)
{
	USART_SendData(USART3, (uint8_t) data);
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

 //发送字符串
void USART3_Sendstring (uint8_t *str ,int string_length)    
{    
	     int i;
   for( i=0;i<string_length;i++)
	{
		USART3_SendByte(str[i]);
	}
}  
