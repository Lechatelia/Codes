#include "stm32f10x.h"
#include "encoder.h"
extern 		u32 KeyValue;   //����ֵ
int16_t r_now;
TIM_TypeDef* Tim_S=TIM4;
uint16_t CPR;

struct Encoder_Stat Encoders={0, 0, 0, 1, 1, 1,0};
void QEI_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	CPR = (u16)ENC_PPR<<2;
	/*
	CH1A--PA6_T3_C1      CH2A--PA0_T2_C1      CH3A--PC6_T8_C1      CH4A--PB6_T4_C1
	CH1B--PA7_T3_C2      CH2B--PA1_T2_C2      CH3B--PC7_T8_C2      CH4B--PB7_T4_C2
	*/
	/* enable the periph clock */

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	/* configue IO */
	// configue CH1 & CH2 IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* configue timer */
	// configue CH1****TIM3****
	TIM_DeInit(TIM4);		
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = CPR-1; 			
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = (u8)0;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/* reset the timer3   */
	TIM4->CNT = (u16)0;

	/* enable the timer3 */
	TIM_Cmd(TIM4, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM4_GPIO_Config(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 
	 //GPIOA��ʱ��
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
 	 
 	 GPIO_SetBits(GPIOA,GPIO_Pin_4); 
}

int32_t Int32Abs(int32_t Data)
{
	if(Data<0)
		return -Data;
	else 
		return Data;
}
void TIM4_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    if(!(TIM4->CR1&0x0010))
    {/* count up overflow interrupt */
        //Com_Printf("up overflow\n\r");
        r_now++;
    }
    else
    {/* count down overflow interrupt */
        //Com_Printf("down overflow\n\r");
        r_now--;
    }
}

int32_t GET_ENCODER(void)
{
	int32_t temp;
// 	assert_param((i>0)&&(i<5));
	temp = (int32_t)(	((int32_t)(r_now))*CPR + (int32_t)(Tim_S->CNT) );
	return temp;
}
 void  Encoder_Update(void)
{
	int64_t now;
	int32_t step;

	now	= GET_ENCODER();
	step = now-Encoders.Now;
	if(step<2000&&step>-2000)
	{
		Encoders.Now = now;
	  Encoders.Total = Encoders.Now-Encoders.Last;
	  if (step == Int32Abs(step))
			Encoders.Distance += Int32Abs(step)*Encoders.Convert1*Encoders.dir;
	  else
			Encoders.Distance -= Int32Abs(step)*Encoders.Convert2*Encoders.dir;
	 }
	 if(Encoders.Distance>=period_clk)
	 {
		 Encoders.Distance -=period_clk;
	 }
	 	 if(Encoders.Distance<0)
	 {
		 Encoders.Distance +=period_clk;
	 }
	 
}
void Encoder_Init(void)
{

	Encoders.Distance=0.0;
	Encoders.Last=Encoders.Now;
	Encoders.Total=0;
}
 

//����ͨ��Tim6������ʱ���������̲���    ����ϵͳ�δ�ʱ��
void TIM6_Int_Init(void)        //1msһ���ж�
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ  //������ʱ��ֻ�����ϼ���
	
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM6_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		Encoder_Update(); 
		}
}


/*************************************************
����: void RCC_Configuration(void)
����: ��λ��ʱ�ӿ��� ����
����: ��
����: ��
**************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //�����ⲿ���پ�������״̬ö�ٱ���
  RCC_DeInit();                                    //��λRCC�ⲿ�豸�Ĵ�����Ĭ��ֵ
  RCC_HSEConfig(RCC_HSE_ON);                       //���ⲿ���پ���
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //�ȴ��ⲿ����ʱ��׼����
  if(HSEStartUpStatus == SUCCESS)                  //�ⲿ����ʱ���Ѿ�׼���
  {
   FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //����FLASHԤ�����幦�ܣ�����FLASH�Ķ�ȡ�����г����б�����÷�.λ�ã�RCC��ʼ���Ӻ������棬ʱ������֮��
   FLASH_SetLatency(FLASH_Latency_2);                    //flash��������ʱ
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //����AHB(HCLK)ʱ�ӵ���==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //����APB2(PCLK2)��==AHBʱ��
    RCC_PCLK1Config(RCC_HCLK_Div1);                //����APB1(PCLK1)��==AHB1/2ʱ��
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //����PLLʱ�� == �ⲿ���پ���ʱ�� * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //ʹ��PLLʱ��
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //�ȴ�PLLʱ�Ӿ���
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //����ϵͳʱ�� = PLLʱ��
    while(RCC_GetSYSCLKSource() != 0x08)                  //���PLLʱ���Ƿ���Ϊϵͳʱ��
    {
    }
  }

}
void Encoder_Configuration(void)
{
  //RCC_Configuration();
	QEI_Init();   //�������ų�ʼ��
	Encoder_Init();  //������ʼ��
	TIM6_Int_Init();          //�������̲�����ʼ��
	//TIM4_GPIO_Config();   //����PA4��ֻ�ж����Э��������
}

