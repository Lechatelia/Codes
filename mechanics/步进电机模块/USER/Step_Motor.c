#include "Step_Motor.h"
#include "delay.h"
#include "led.h"


int step_num_1=0;//步进电机步数
int step_num_2=0;//步进电机步数
u8 unlimit_flag_1=0;
u8 unlimit_flag_2=0;
long step_spot_1=0;//步进电机当前位置
long step_spot_2=0;//步进电机当前位置

//PA2 TIM5-CH3
void TIM5_Init(void)                         //定时器初始化，步进电机的定时器
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM5 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* Enable the TIM3 gloabal Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 6000000)-1;
  TIM_TimeBaseStructure.TIM_Period = 100-1;//100us
  TIM_TimeBaseStructure.TIM_Prescaler = 72;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 50-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

  TIM_OC3Init(TIM5, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM5, ENABLE);
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM5, ENABLE);
	
}


void TIM2_Init(void)                         //定时器初始化，步进电机的定时器
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* TIM5 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	 /* Enable the TIM3 gloabal Interrupt */

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	//PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 6000000)-1;
  TIM_TimeBaseStructure.TIM_Period = 1000-1;//100us
  TIM_TimeBaseStructure.TIM_Prescaler = 9;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 500-1;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

  TIM_OC3Init(TIM2, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);
	
}

void step_dir_init(void)//步进电机的设置，步进电机控制包含方向，PWM脉冲输出，共阴共阳接法，以及地
{
		//步进电机方向控制
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIO外设时钟使能 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);	
}

void step_motor_1(long num)//dir=0,向左，负的
{
	
	unlimit_flag_1=0;
	step_num_1=num;

	if(num<0)
	{
		step_num_1=-num;
		step_spot_1+=num;
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);				
	}
	else
	{
		step_num_1=num;
		step_spot_1+=num;
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}
	TIM_Cmd(TIM3, ENABLE);
	//TIM3_Init();
	LED1_off;
}

void step_motor_2(long num)//dir=0,向左，负的
{
	
	unlimit_flag_2=0;
	step_num_2=num;

	if(num<0)
	{
		step_num_2=-num;
		step_spot_2+=num;
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);				
	}
	else
	{
		step_num_2=num;
		step_spot_2+=num;
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
	}
	TIM_Cmd(TIM2, ENABLE);
	//TIM3_Init();
	LED1_off;
}

void unlimit_step_1(void)
{
	unlimit_flag_1=1;
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);	//复位方向
	TIM3_Init();
	LED1_off;
}

void unlimit_step_2(void)
{
	unlimit_flag_2=1;
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);	//复位方向
	TIM2_Init();
	LED2_off;
}

void TIM3_IRQHandler(void)                      //控制中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		if(unlimit_flag_1==0)
		{
		
		     if(step_num_1<=0)
	      	{
			        TIM_Cmd(TIM3, DISABLE);//关闭PWM波
						  LED1_on;
	        }
	        else
	      	{
		        	step_num_1--;
	        }
    	}
	}
	  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM2_IRQHandler(void)                      //控制中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if(unlimit_flag_2==0)
		{
		
		     if(step_num_2<=0)
	      	{
			        TIM_Cmd(TIM2, DISABLE);//关闭PWM波
						  LED2_on;
	        }
	        else
	      	{
		        	step_num_2--;
	        }
    	}
	}
				TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}
