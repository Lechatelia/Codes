#include "pwm.h"
void pwm_init(double ratio)
{
	gpio_init();
  mode_init(ratio);

}
void gpio_init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4;	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
void mode_init(double ratio)
{  
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_TimeBaseStructure.TIM_Period=20000;  //0到7199刚好7200个数
TIM_TimeBaseStructure.TIM_Prescaler=72; //设定分频值
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//时基初始化完成
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
//时基初始化应用
	
	//设置通道1
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0.075*20000;//设置跳变值，此时电平发生跳变    初始宽度
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2,ENABLE); //使能重载寄存器ARR
	TIM_Cmd(TIM2,ENABLE);    //使能定时器
}
