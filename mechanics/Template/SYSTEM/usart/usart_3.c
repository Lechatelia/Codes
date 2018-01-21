#include "usart_3.h"


/*
USART3_TX      ->  PB10
USART3_RX      ->  PB11
USART3_RX_DMA  ->  Channel3
USART3_Base       0x40004800
USART_DR Offset   0x04
*/

#define USART3_DR_Address ((u32)0x40004804)     //USART3-DR ��ַ
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
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Address;			//DMAͨ��1�ĵ�ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_RxBuffer;		//DMA���͵�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;						//���ͷ��� USART������
	DMA_InitStructure.DMA_BufferSize = RxBufferSize_3;						//�����ڴ��С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//����Դ��ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//�����ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//Դ��ַ�����ݳ�����8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//���͵�Ŀ�ĵ�ַ��8λ���
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;							//����ģʽѭ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//���ȼ�����
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMAͨ��û������Ϊ�ڴ浽�ڴ洫�� 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	//USART3_init
	USART_InitStructure.USART_BaudRate = 115200;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	//Enable USARTy DMA Rx request
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	//����DMA1ͨ��1��������ж�
	DMA_ITConfig(DMA1_Channel3,DMA_IT_TC, ENABLE);
	//Enable USARTy RX DMA1 Channel 
	DMA_Cmd(DMA1_Channel3, ENABLE);
   
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
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

 //�����ַ���
void USART3_Sendstring (uint8_t *str ,int string_length)    
{    
	     int i;
   for( i=0;i<string_length;i++)
	{
		USART3_SendByte(str[i]);
	}
}  
