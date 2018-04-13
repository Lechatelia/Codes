#include "clamp.h"

void my_clamp_delay()
{  int i;
	for(i=0;i<99999;i++) ;
}
void clamp_init(int ccr)
{
	clamp_gpio_init();
  clamp_mode_init(ccr);
}
void clamp_gpio_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
}
void clamp_mode_init(int ccr)
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
	TIM_OCInitStructure.TIM_Pulse=2100;//设置跳变值，此时电平发生跳变   
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2,ENABLE); //使能重载寄存器ARR
	TIM_Cmd(TIM2,ENABLE);    //使能定时器
}


//用于夹持装置的动作，角度化为距离信息，0-90  注意现在认为是占空比是在900到2100之间，需要调试试，还有初始电平值
void set_clamp_distance(int distance)
{ int CCR;
	CCR=2100-(double)distance*(900)/90;  //需要预先知道0与90度的占空比
	TIM_SetCompare1(TIM2,(int)(CCR));
}


////only 0 or 90
//void set_servo_angle(int angle)
//{
//	int i=0;
//	if(angle==0&&servo_angle==90)
//	{
//		for(i=8;i>=0;i--)
//		{
//			set_clamp_distance(i*10);
//			servo_angle=i*10;
//			my_clamp_delay();	
//		}
//		
//	}
//		else if(angle==90&&servo_angle==0)
//	{
//		for(i=1;i<=9;i++)
//		{
//			set_clamp_distance(i*10);
//			servo_angle=i*10;
//			my_clamp_delay();
//			
//		}
//	}
//		
//}

