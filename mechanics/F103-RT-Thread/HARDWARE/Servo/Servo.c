#include "Servo.h"
void Servo_init(int ccr)
{
	gpio_init();
  mode_init(ccr);
}
void gpio_init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
void mode_init(int ccr)
{  
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_TimeBaseStructure.TIM_Period=20000;  //0到7199刚好7200个数
TIM_TimeBaseStructure.TIM_Prescaler=72; //设定分频值
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//时基初始化完成
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
//时基初始化应用
	
	//设置通道1
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0.075*20000;//设置跳变值，此时电平发生跳变    初始宽度
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3,ENABLE); //使能重载寄存器ARR
	TIM_Cmd(TIM3,ENABLE);    //使能定时器
}


//用于舵机调角度，这里只用于舵机调整90度
void set_Servp_angle(int angle)
{ int CCR;
	CCR=900+angle*(2100-900)/90;  //需要预先知道0与90度的占空比
	TIM_SetCompare1(TIM3,(int)(CCR));
}
