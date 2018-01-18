#include "lcd.h"
#include "delay.h"

uint8_t LCD_Table[LCD_LEN];
uint8_t LCD_Index=0;

void all_lcd_init(void)
{
	delay_init();
  USART3_Init();
 	SysTick_init();
}

volatile uint8_t USART1_RxBuffer[RxBufferSize];
volatile float USART3_TxBuffer[TxBufferSize];

		u32 KeyValue;   //����ֵ
		u8	KeyReady=0;	//����ȷ��
		u8  ChangK=0;

void LCD_PushByte(uint8_t data)		//����һ���ַ�
{
	LCD_Table[LCD_Index]=data;
	if(LCD_Index >= (LCD_LEN-1))
		LCD_Index=0;
	else
		LCD_Index++;
}

void LCD_PushString(char *data)		//����һ���ַ���
{
	while(*data)
	{
		LCD_PushByte(*data++);
	}
}
void LCD_SetXY(int x,int y)
{
	LCD_Index=x+y*20;
}

void LCD_WriteChar(char data)
{
	LCD_PushByte(data);
}

void LCD_WriteString(char *data)
{
	LCD_PushString(data);
}

void LCD_WriteInt(int data)//д�������ӵ�λ���λд
{
	char answer[100];
	int index=98;
	int pose=0;//�����ű�־��
	if(data<0)
	{
		pose=1;
		data=-data;
	}
	answer[99]='\0';//���λ�洢������־
	while(data>9)//�洢����λ
	{
		answer[index--]=data%10+'0';
		data=data/10;
	}
	answer[index]=data+'0';
	if(pose)//�洢������
	{
		answer[--index]='-';
	}
	LCD_PushString(answer+index);//д��������
}
void LCD_WriteDouble(double data,int m)//дdouble������mΪС�����λ�����Ӹ�λ���λд
{
	if(data<0&&data>-1)
		LCD_WriteChar('-');//д����
	LCD_WriteInt((int)data);//д��������
	if(m>0)
	{
		int i;
		LCD_WriteChar('.');//дС����
		if(data<0)
			data=-data;
		data=data-(int)data;
		for(i=0;i<m;i++)//дС������
		{
			data=data*10;
			LCD_WriteChar((int)data+'0');
			data=data-(int)data;
		}
		
	}
	
}

void LCD_WriteNum(double data)//дdouble������С�������6λ���Ӹ�λ���λд
{
	char str[5];
	int flag=0,i;
	if(data<0)
	{
		LCD_WriteChar('-');//д����
		data=-data;
	}
	LCD_WriteInt((int)data);//д��������
	data=data-(int)data;
	for(i=0;i<5;i++)//��С�����ֱ�����һ���ַ�����
	{
		data=data*10;
		str[i]=(int)data+'0';
		if(str[i]!='0')
		{
			flag=i+1;
		}
		data=data-(int)data;
		
	}
	if(flag!=0)
		LCD_WriteChar('.');//дС����
	for(i=0;i<flag;i++)
	{
		LCD_WriteChar(str[i]);//дС������
	}	
	for(i=flag;i<6;i++)
	{
		LCD_WriteChar(' ');//С����󲻹�6λ�򲹿ո�
	}
}

void LCD_WriteCharXY(char data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_PushByte(data);
}

void LCD_WriteStringXY(char *data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_PushString(data);
}

void LCD_WriteIntXY(int data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteInt(data);
}

void LCD_WriteDoubleXY(double data,int m,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteDouble(data, m);
}

void LCD_WriteNumXY(double data,int x,int y)
{
	LCD_SetXY(x,y);
	LCD_WriteNum(data);
}

void LCD_Clear(void)//����ʱ��ͬʱ�������(0,0)
{
	//LCD_WriteChar(12);//������������ʱΪ��LCD_PushByte()�������Լ�¼LCD����
	LCD_WriteStringXY("                                                                                ",0,0); 
	LCD_WriteStringXY("                                                                                ",0,4); 

	LCD_SetXY(0,0);
}
extern int clk;
extern uint8_t Can_State;
//4��20�й�80��Ԫ��ÿ10����ԪΪһ��ˢ��
void TaskLcdRefresh(void)
{
	static uint8_t i = 0,j = 0;
		UART3_SendByte(0xff);
		UART3_SendByte(30*(i/2)+(i%2)*10);
		for(j=0;j<=9;j++)
		{
			 UART3_SendByte (LCD_Table[10*i+j]);
			 //OSTimeDly(1);
		}
    if(i>=15)
			i = 0;
		else
			i++;
}

void Input_IntValue(int *address,char *name)//��������
{    
	char str[20]={'\0'};
	int temp;
	int i,j;

	str[0]=' ';

	i=1;

	for(;;)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteInt(*address);
		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		
		if(i<=1)
		{
			LCD_SetXY(5,2);
			LCD_WriteString("Value=0!!!");
		}
		
		delay_ms(5);
		
        if(KeyReady)
		{
			      KeyReady = 0;
            switch(KeyValue)
            {
                case 1://1
                    if(i<14)
                        str[i++]='1';
                break;
                case 2://2
                    if(i<14)
                        str[i++]='2';
                break;
                case 3://3
                    if(i<14)
                        str[i++]='3';
                break;
                case 4://4
                    if(i<14)
                        str[i++]='4';
                break;
                case 7://5
                    if(i<14)
                        str[i++]='5';
                break;
                case 8://6
                    if(i<14)
                        str[i++]='6';
                break;
                case 9://7
                    if(i<14)
                        str[i++]='7';
                break;
                case 10://8
                    if(i<14)
                        str[i++]='8';
                break;
                case 13://9
                    if(i<14)
                        str[i++]='9';
                break;
                case 14://0
                    if(i<14)
                        str[i++]='0';
                break;
				case 16://-
                    if(str[0] == ' ')
                        str[0] = '-';
                    else
                        str[0] = ' ';
                break;
                case 17://del
                    if(i>1)
                    {
                        str[--i]='\0';
                    }
                break;
                case 18://ok
                    temp=0;
                    
                    for(j=1;j<i;j++)//��������ֵ
                    {
                        temp=temp*10+str[j]-'0';
                    }
                    if(str[0]=='-')
                        temp=-temp;
                    *address=temp;
                    return;
                
                case 5://cancel
                    return;
            }
        }
	}
}

void Input_DoubleValue(double * address,char *name)//��������
{
    u32 point=0;
	char str[20]={'\0'};
	float temp;
	int i,j;

	str[0]=' ';
	
	i=1;
	point=0;
	for(;;)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteNum(*address);
		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		
		if(i<=1)
		{
			LCD_SetXY(5,2);
			LCD_WriteString("Value=0!!!");
		}
		
		delay_ms(20);
		
        if(KeyReady)
        {
					  KeyReady = 0;
            switch(KeyValue)
            {
                case 1://1
                    if(i<14)
                        str[i++]='1';
                break;
                case 2://2
                    if(i<14)
                        str[i++]='2';
                break;
                case 3://3
                    if(i<14)
                        str[i++]='3';
                break;
                case 4://4
                    if(i<14)
                        str[i++]='4';
                break;
                case 7://5
                    if(i<14)
                        str[i++]='5';
                break;
                case 8://6
                    if(i<14)
                        str[i++]='6';
                break;
                case 9://7
                    if(i<14)
                        str[i++]='7';
                break;
                case 10://8
                    if(i<14)
                        str[i++]='8';
                break;
                case 13://9
                    if(i<14)
                        str[i++]='9';
                break;
                case 14://0
                    if(i<14)
                        str[i++]='0';
                break;
                case 15://.
                    if(point==0&&i<14)
                    {
                        str[i++]='.';
                        point=1;
                    }
                break;
				case 16://-
                    if(str[0] == ' ')
                        str[0] = '-';
                    else
                        str[0] = ' ';
                break;
                case 17://del
                    if(i>1)
                    {
                        if(str[i-1]=='.')
                            point=0;
                        str[--i]='\0';
                    }
                break;
                case 18://ok
                    temp=0;
                    point=0;
                    for(j=1;j<i;j++)// 
                    {
                        if(str[j]=='.')
                        {
                            point=10;
                        }
                        else if(point==0)
                        {
                            temp=temp*10+str[j]-'0';
                        }
						else
						{
							temp+=(str[j]-'0')/(float)point;
							point=point*10;
						}
                    }
                    if(str[0]=='-')
                        temp=-temp;
                    *address=temp;
                    return;
                
                case 5://cancel
                    return;
            }
        }
	}
}
void Input_FloatValue(float * address,char *name)//��������
{
    u32 point=0;
	char str[20]={'\0'};
	float temp;
	int i,j;

	str[0]=' ';
	
	i=1;
	point=0;
	for(;;)
	{
		LCD_Clear();
		LCD_SetXY(0,0);
		LCD_WriteString(name);
		LCD_SetXY(0,1);
		LCD_WriteString("old:");
		LCD_WriteNum(*address);
		LCD_SetXY(0,2);
		LCD_WriteString("new:");
		LCD_WriteString(str);
		
		if(i<=1)
		{
			LCD_SetXY(5,2);
			LCD_WriteString("Value=0!!!");
		}

		delay_ms(20);
		
        if(KeyReady)
        {
					  KeyReady = 0;
            switch(KeyValue)
            {
                case 1://1
                    if(i<14)
                        str[i++]='1';
                break;
                case 2://2
                    if(i<14)
                        str[i++]='2';
                break;
                case 3://3
                    if(i<14)
                        str[i++]='3';
                break;
                case 4://4
                    if(i<14)
                        str[i++]='4';
                break;
                case 7://5
                    if(i<14)
                        str[i++]='5';
                break;
                case 8://6
                    if(i<14)
                        str[i++]='6';
                break;
                case 9://7
                    if(i<14)
                        str[i++]='7';
                break;
                case 10://8
                    if(i<14)
                        str[i++]='8';
                break;
                case 13://9
                    if(i<14)
                        str[i++]='9';
                break;
                case 14://0
                    if(i<14)
                        str[i++]='0';
                break;
                case 15://.
                    if(point==0&&i<14)
                    {
                        str[i++]='.';
                        point=1;
                    }
                break;
				case 16://-
                    if(str[0] == ' ')
                        str[0] = '-';
                    else
                        str[0] = ' ';
                break;
                case 17://del
                    if(i>1)
                    {
                        if(str[i-1]=='.')
                            point=0;
                        str[--i]='\0';
                    }
                break;
                case 18://ok
                    temp=0;
                    point=0;
                    for(j=1;j<i;j++)// 
                    {
                        if(str[j]=='.')
                        {
                            point=10;
                        }
                        else if(point==0)
                        {
                            temp=temp*10+str[j]-'0';
                        }
						else
						{
							temp+=(str[j]-'0')/(float)point;
							point=point*10;
						}
                    }
                    if(str[0]=='-')
                        temp=-temp;
                    *address=temp;
                    return;
                
                case 5://cancel
                    return;
            }
        }
	}
}


void USART3_Init(void)
{
// 	USART_InitTypeDef USART_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
	USART3_NVIC_Config();
	USART3_GPIO_Config();
	USART3_MODE_Config();
	USART3_DMA_TX_Config();
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
// 	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
//  DMA_Cmd(DMA1_Channel2, ENABLE);
//	USART1_DMA_TX_NVIC_Config();
}

void USART3_NVIC_Config(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
		NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
}


void USART3_IRQHandler(void)
{
// 		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
// 		USART_ClearFlag(USART3,USART_FLAG_RXNE);
//  		USART_ClearFlag(USART3,USART_FLAG_TC);
//		u32 KeyValue;
	
    //LED_Toggle(2);
	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
			KeyValue = USART_ReceiveData(USART3);
			KeyReady=1;
    }
	
	
	if(KeyValue == 6)
	{
//		rt_mb_send(&Mb_Emergency, KeyValue);
		ChangK=1;
	}
    else
//		rt_mb_send(&Mb_Key, KeyValue);
    
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);  
}


void USART3_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	//??USART3 GPIO??
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
		USART_DeInit(USART3);  //????3

		//USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//??????
    GPIO_Init(GPIOB, &GPIO_InitStructure); //???PA9

		//USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //???PA10
}


void USART3_MODE_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    
  USART_Init(USART3,&USART_InitStructure);   
  USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
  
  USART_Cmd(USART3,ENABLE);
  USART_ClearFlag(USART3, USART_FLAG_TC);
}
void USART3_DMA_TX_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Channel2); 												// 
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART3->DR);			//DMA??1???
	
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART3_TxBuffer;		//DMA????
	DMA_InitStructure.DMA_BufferSize = TxBufferSize;						//??????

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;						//???? USART???
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;		//????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;					//????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//?????????8?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;			//????????8???
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;							//??????DMA_Mode_Circular	DMA_Mode_Normal
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;					//?????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;							//DMA?????????????? 
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);							//
	
}

void UART3_SendByte(uint8_t ch)
{
  USART_SendData(USART3, (uint8_t) ch);
  while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

void RefreshDma(void)
{
	static uint8_t i = 0,j = 0;
		UART3_SendByte(0xff);
		UART3_SendByte(30*(i/2)+(i%2)*10);
		for(j=0;j<=9;j++)
		{
			 UART3_SendByte(LCD_Table[10*i+j]);
			 //OSTimeDly(1);
		}
    if(i>=15)
			i = 0;
		else
			i++;

}

void TIM5_LCD_INIT(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1 ; 
	TIM_TimeBaseStructure.TIM_Period = 10000; // 20ms 100HZ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM5,TIM_IT_Update);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);

}





///delay
//////////////////////////////////////////////////////////////////////////////////	 
//V1.2�޸�˵��
//�������ж��е��ó�����ѭ���Ĵ���
//��ֹ��ʱ��׼ȷ,����do while�ṹ!
//////////////////////////////////////////////////////////////////////////////////	 
static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������
//��ʼ���ӳٺ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
// void delay_init(u8 SYSCLK)
// {
// 	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
// 	fac_us=SYSCLK/8;		    
// 	fac_ms=(u16)fac_us*1000;
// }								    
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
// void delay_ms(u16 nms)
// {	 		  	  
// 	u32 temp;		   
// 	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
// 	SysTick->VAL =0x00;           //��ռ�����
// 	SysTick->CTRL=0x01 ;          //��ʼ����  
// 	do
// 	{
// 		temp=SysTick->CTRL;
// 	}
// 	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
// 	SysTick->CTRL=0x00;       //�رռ�����
// 	SysTick->VAL =0X00;       //��ռ�����	  	    
// }   
// //��ʱnus
// //nusΪҪ��ʱ��us��.		    								   
// void delay_us(u32 nus)
// {		
// 	u32 temp;	    	 
// 	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
// 	SysTick->VAL=0x00;        //��ռ�����
// 	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
// 	do
// 	{
// 		temp=SysTick->CTRL;
// 	}
// 	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
// 	SysTick->CTRL=0x00;       //�رռ�����
// 	SysTick->VAL =0X00;       //��ռ�����	 
// }



/////////////////////////////////SysTick

static __IO uint32_t TimingDelay;

void SysTick_init(void)
{
	if (SysTick_Config(SystemCoreClock / 500))   //Setup SysTick Timer for 1 ms interrupts
	{ 
		/* Capture error */ 
		while (1);
	}
}

void delay(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	
	while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}







