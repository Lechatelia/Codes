#include "motor.h"
#include "delay.h"
#include "parameters.h"

enum Motor_Dir motor1_dir=stop; //默认停止
int delay_flag=0;
int DC_Motor_flag=0; //电机是否正在移动
long dc_place=0;      //需要移动的移动位置
long place_reset=1;      //直线电机复位

void GPIO_init_motor(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC端口时钟
		 	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;				 //LED0-->PC.0 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //推挽输出  注意如果选择不同的话有可能没办法仿真
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC.13
	GPIO_ResetBits(GPIOA,GPIO_Pin_0| GPIO_Pin_1);						 //PC.0 输出高

	

}
 //比较值从 0到99变化，速度慢慢增大
void PWM_init_motor( )
{
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    //时钟使能好像一定要在这里

TIM_TimeBaseStructure.TIM_Period=100-1;  //100us
TIM_TimeBaseStructure.TIM_Prescaler=72-1; //设定分频值
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//时基初始化完成
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
//时基初始化应用
	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //极性为高
	
	//设置通道1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//设置跳变值，此时电平发生跳变
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
		
	//设置通道2
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse=0;
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //极性为高
//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
//	
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse=0;
//	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	  	
		 
	    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
  TIM_ARRPreloadConfig(TIM2,ENABLE); //使能重载寄存器ARR
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE);    //使能定时器
}

void TIM2_IRQHandler(void)                      //控制中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if(DC_Motor_flag==1)  //正在移动
		{
		
		     if(dc_place<=0)
	      	{
			      setspeed_motor1(stop,0,0); //不可以关闭定时器，应该是使得两个引脚都输出低电平
						DC_Motor_flag=0; //电机静止
						 
	        }
	        else
	      	{
		        	dc_place--;
	        }
    	}
	}
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

//参数1：方向  ，参数2：速度0~99  参数3：开关周期数，对应着开启时间一个开关周期为0.1ms
void setspeed_motor1(enum Motor_Dir dir,int speed ,long number)   //改变方向或者速度
{  
	if(motor1_dir==dir)        //走向未改变
	{
		  if(dir==forward)
		 {
			DC_Motor_flag=1;
			dc_place=number;
			TIM_SetCompare1(TIM2,speed); 
		 }
		 else if(dir==backward)
		 {
			DC_Motor_flag=1;
			dc_place=number;
			TIM_SetCompare2(TIM2,speed);
		 }
		 else    //停止
		 {
			 DC_Motor_flag=0;
			 TIM_SetCompare1(TIM2,0);
			 TIM_SetCompare2(TIM2,0);
		 }
	 }
	else     //走向改变
	{   motor1_dir=dir;  //注意该变量只能在这里改变
		   if(dir==forward)
		 {
			 DC_Motor_flag=1;
			dc_place=number;
			TIM_SetCompare2(TIM2,0);
			TIM7_delay_1ms();
			TIM_SetCompare1(TIM2,speed); 
		 }
		 else if(dir==backward)
		 {
			 DC_Motor_flag=1;
			dc_place=number;
			TIM_SetCompare1(TIM2,0); 
		  TIM7_delay_1ms();
			TIM_SetCompare2(TIM2,speed);
		 }
		 else
		 {
			 DC_Motor_flag=0;
			 TIM_SetCompare1(TIM2,0);
			 TIM_SetCompare2(TIM2,0);
		 }
		 
	}

		
}

//死区时间为了防止系统滴答定时器的混乱使用，这里使用基本定时器tim7
void TIM7_Int_Init(void)        //1ms一次中断
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 99; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式  //基本定时器只能向上计数
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
  
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM7, DISABLE);  //使能TIMx					 
}

//定时器延时1ms
 void TIM7_delay_1ms(void)   
{
	TIM7->CNT = 0;
	TIM_Cmd(TIM7, ENABLE);
	delay_flag=1;
	while(delay_flag==1);
	TIM_Cmd(TIM7, DISABLE); 
}
//定时器7中断服务程序
void TIM7_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //清除TIMx更新中断标志 
	    delay_flag=0;
		
		}
}

void DC_Motor_init_motor( )
{
	 GPIO_init_motor();
	 PWM_init_motor();
	 TIM7_Int_Init();
	DC_Motor_flag=0; //初始状态为未移动状态
	//setspeed_motor1(forward,99);
}

//直线电机夹持轮胎
void DC_Motor_positive( long number) 
{
		setspeed_motor1(forward,50,number);
}

//直线电机松开轮胎
void DC_Motor_negative( long number) 
{
		setspeed_motor1(backward,50,number);
}

//直线电机复位
void DC_Motor_reset( void) 
{
	place_reset=1;
	DC_Motor_positive(100000);
	while(place_reset==1);	
  DC_Motor_negative(Time_start);//松开轮子
}
