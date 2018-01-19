#include "motor.h"
#include "delay.h"

enum Motor_Dir motor1_dir=stop; //默认停止

void GPIO_init_motor(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PC端口时钟
		 	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PC.0 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //推挽输出  注意如果选择不同的话有可能没办法仿真
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.13
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);						 //PC.0 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		 //LED1-->PC.1 端口配置, 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_ResetBits(GPIOB,GPIO_Pin_9); 						 //PC.1 输出高
	

}
 //比较值从 0到1000变化，速度慢慢增大
void PWM_init_motor( )
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);    //时钟使能好像一定要在这里
TIM_TimeBaseStructure.TIM_Period=999;  //0到999刚好1000个数
TIM_TimeBaseStructure.TIM_Prescaler=71; //设定分频值
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//时基初始化完成
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
//时基初始化应用
	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //极性为高
	
//	//设置通道1
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse=0;//设置跳变值，此时电平发生跳变

//	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
//	
//	//设置通道2
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse=0;
//	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //极性为高
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM4,ENABLE); //使能重载寄存器ARR
	TIM_Cmd(TIM4,ENABLE);    //使能定时器
}
void setspeed_motor1(enum Motor_Dir dir,int speed)   //改变方向或者速度
{  
	if(motor1_dir==dir)        //走向未改变
	{
		  if(dir==forward)
		 {
			TIM_SetCompare3(TIM4,speed); 
		 }
		 else if(dir==backward)
		 {
			TIM_SetCompare4(TIM4,speed);
		 }
		 else
		 {
			 TIM_SetCompare3(TIM4,0);
			 TIM_SetCompare4(TIM4,0);
		 }
	 }
	else     //走向改变
	{   motor1_dir=dir;  //注意该变量只能在这里改变
		   if(dir==forward)
		 {
			TIM_SetCompare4(TIM4,0);
			delay_ms(1);
			TIM_SetCompare3(TIM4,speed); 
		 }
		 else if(dir==backward)
		 {
			TIM_SetCompare3(TIM4,0); 
			delay_ms(1);
			TIM_SetCompare4(TIM4,speed);
		 }
		 else
		 {
			 TIM_SetCompare3(TIM4,0);
			 TIM_SetCompare4(TIM4,0);
		 }
		 
	}

		
}

