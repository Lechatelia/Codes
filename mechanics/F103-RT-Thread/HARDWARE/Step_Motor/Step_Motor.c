#include "Step_Motor.h"
#include "delay.h"
#include "led.h"


long step_num_1=0;//�����������Ҫ�ߵĲ���
long step_num_2=0;//�����������Ҫ�ߵĲ���
u8 unlimit_flag_1=0; //�Ƿ�λ
u8 unlimit_flag_2=0;
u8 unlimit_flag_3=0;
long step_spot_1=0;//���������ǰλ��  ����ڸ�λ���
long step_spot_2=0;//���������ǰλ��
long step_spot_3_target=0;//���������ǰλ��
int step_motor_3_flag=0;//�������3���ڹ�������Ҫ�ж�λ��
//PA2 TIM5-CH3
void TIM5_Init(void)                         //��ʱ����ʼ������������Ķ�ʱ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM5 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* Enable the TIM8 gloabal Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 6000000)-1;
  TIM_TimeBaseStructure.TIM_Period = 100-1;//100us
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 50-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC3Init(TIM5, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM5, ENABLE);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);
	
}

//PC6 TIM8-CH1
void TIM8_Init(void)                         //��ʱ����ʼ������������Ķ�ʱ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM5 clock enable */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	
	//PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 6000000)-1;
  TIM_TimeBaseStructure.TIM_Period = 100-1;//100us
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�ظ���������   �߼���ʱ������
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
  TIM_ARRPreloadConfig(TIM8, ENABLE);
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 50-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	//����Ϊ�߼���ʱ������
	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity= TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState= TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState=  TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM8, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_ClearFlag(TIM8, TIM_FLAG_Update);
  TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	
		 /* Enable the TIM3 gloabal Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_Cmd(TIM8, ENABLE);
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	
}

//PA8 TIM1-CH1
void TIM1_Init(void)                         //��ʱ����ʼ������������Ķ�ʱ��
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM5 clock enable */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	
	//PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 6000000)-1;
  TIM_TimeBaseStructure.TIM_Period = 80-1;//100us
  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//�ظ���������   �߼���ʱ������
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM1, ENABLE);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 40-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	//����Ϊ�߼���ʱ������
	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity= TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState= TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState=  TIM_OCIdleState_Reset;

  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);      //����tim1������Ҫ
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
		 /* Enable the TIM3 gloabal Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
  TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);     //�������
	
}

void step_dir_init(void)//������������ã�����������ư�������PWM������������������ӷ����Լ���
{
		//��������������
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��GPIO����ʱ��ʹ�� 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_7);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);	

}

void step_motor_1(long num)//dir=0,���󣬸���
{
	
	
	step_num_1=num;

	if(num<0)
	{
		step_num_1=-num;
		step_spot_1+=num;
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);				
	}
	else  if(num>0)
	{
		step_num_1=num;
		step_spot_1+=num;
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
	}
	TIM_Cmd(TIM5, ENABLE);
	//TIM3_Init();
   
}

void step_motor_2(long num)//dir=0,���󣬸���
{
	
	step_num_2=num;

	if(num<0)
	{
		step_num_2=-num;
		step_spot_2+=num;
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);				
	}
	else if(num>0)
	{
		step_num_2=num;
		step_spot_2+=num;
		GPIO_SetBits(GPIOC,GPIO_Pin_7);
	}
	TIM_Cmd(TIM8, ENABLE);
	//TIM3_Init();
	
}

//���ڴ�Ĳ�����������ղ���������ȷ����ת����Ϊ0ʱֹͣת��
void step_motor_3(long num)//dir=0,���󣬸���
{
	
	unlimit_flag_3=0;

	if(num<0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);	
		TIM_Cmd(TIM1, ENABLE);		
	}
	else if(num>0)
	{

		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		TIM_Cmd(TIM1, ENABLE);
	}
	else      //
	{
		 TIM_Cmd(TIM1, DISABLE);//�ر�PWM��
	}
	
	//TIM3_Init();
	
}

void step_motor_3_work(long num,double place)//dir=0,���󣬸���
{
  step_motor_3(num);
  step_spot_3_target=place;
	step_motor_3_flag=1;  //��ʼ��ת
}

void unlimit_step_1(void)
{
	unlimit_flag_1=1;
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//��λ����
	TIM5_Init();

}

void unlimit_step_2(void)
{
	unlimit_flag_2=1;
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);	//��λ����
	TIM8_Init();
	
}
void unlimit_step_3(void)
{
	unlimit_flag_3=1;
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);	//��λ����
	TIM1_Init();	
}

void TIM5_IRQHandler(void)                      //�����ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		if(unlimit_flag_1==0)
		{
		
		     if(step_num_1<=0)
	      	{
			        TIM_Cmd(TIM5, DISABLE);//�ر�PWM��
						 
	        }
	        else
	      	{
		        	step_num_1--;
	        }
    	}
	}
	  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
}

void TIM8_UP_IRQHandler(void)                      //�����ж�
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		if(unlimit_flag_2==0)
		{
		
		     if(step_num_2<=0)
	      	{
			        TIM_Cmd(TIM8, DISABLE);//�ر�PWM��
						 
	        }
	        else
	      	{
		        	step_num_2--;
	        }
    	}
	}
				TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
}

void TIM1_UP_IRQHandler(void)                      //�����ж�
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
	
	}
				TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
}

 void Step_Motor_Init(void)
{  step_dir_init();//�������
	 unlimit_step_1();
	 unlimit_step_2();
	 unlimit_step_3();
}
