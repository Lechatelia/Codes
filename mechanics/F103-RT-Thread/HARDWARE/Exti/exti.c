#include "exti.h"
#include "delay.h"
#include "led.h"
#include "Step_Motor.h"
#include "motor.h"



int exti_flag=0;       //刚刚产生中断
int key_number=0; //用于调试
//extern int flag ;

/*******************************************************************************
* 函 数 名         : KEY_Init
* 函数功能		   : 按键初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;	   //选择你要设置的IO口
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入  
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	   //设置传输速率
//	GPIO_Init(GPIOB,&GPIO_InitStructure);		  /* 初始化GPIO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : My_EXTI_Init
* 函数功能		   : 外部中断初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	KEY_Init();         //GPIO_Init
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//选择GPIO管脚用作外部中断线路
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);//选择GPIO管脚用作外部中断线路
	
	
//	//EXTI0 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0中断通道
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	//EXTI2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	//EXTI3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//EXTI4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
//	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}

/*******************************************************************************
* 函 数 名         : EXTI0_IRQHandler
* 函数功能		   : 外部中断0函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
//void EXTI0_IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line0)==1)
//	{
//		delay_ms(10);
//		if(K_UP==1)
//		{
//			LED3_on;
//		}	
//	}
//	EXTI_ClearITPendingBit(EXTI_Line0);
//}

/*******************************************************************************
* 函 数 名         : EXTI3_IRQHandler
* 函数功能		   : 外部中断3函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3)==1)
	{
		if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7)!=1)//首先判断步进电机行走的方向正向能够行走
		{	
	///	delay_ms(10);
		if(K_DOWN==0)
		{   
						unlimit_flag_2=0;
		        TIM_Cmd(TIM8, DISABLE);
			     // LED2_on;
		        step_num_2=0;//置电机步数为0
		        step_spot_2=0;//电机的坐标为0
		}
	}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}

/*******************************************************************************
* 函 数 名         : EXTI2_IRQHandler
* 函数功能		   : 外部中断2函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2)==1)
	{
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3)!=1)//首先判断步进电机行走的方向正向能够行走
		{	
	//	delay_ms(10);
		if(K_LEFT==0)
		{   unlimit_flag_1=0;
			  TIM_Cmd(TIM5, DISABLE);
			    //  LED1_on;
		        step_num_1=0;//置电机步数为0
		        step_spot_1=0;//电机的坐标为0
		      	unlimit_step_2(); 
//				if(flag==0)
//					flag=1;
		}
		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

/*******************************************************************************
* 函 数 名         : EXTI4_IRQHandler
* 函数功能		   : 外部中断4函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void EXTI4_IRQHandler(void)
{  
	if(EXTI_GetITStatus(EXTI_Line4)==1&&exti_flag==0) //如果按键信息未处理则不进入
	{
	//	delay_ms(10);
		if(K_RIGHT==0)
		{
      key_number=(key_number+1)%3;
			exti_flag=1;

		}
		
	}
	EXTI_ClearITPendingBit(EXTI_Line4);
}


