#include "exti.h"
#include "led.h"
#include "Step_Motor.h"
extern long step_num_1;
extern long step_spot_1;
extern long step_num_2;
extern long step_spot_2;
u8 exti_flag=0;
extern int flag ;
void NVIC_Exti_Configuration(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI_PA2_config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	 	EXTI_InitTypeDef EXTI_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO,ENABLE);

    NVIC_Exti_Configuration();
	    
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化Pd2
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //上拉输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化Pd2
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	
	  EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
}

void EXTI9_5_IRQHandler(void)//定义外部中断，接近开关
{
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
//		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)!=1)//首先判断步进电机行走的方向避免反向出错
//		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)
			{
		        TIM_Cmd(TIM3, DISABLE);
			    //  LED1_on;
		        step_num_1=0;//置电机步数为0
		        step_spot_1=0;//电机的坐标为0
				if(flag==0)
					flag=1;
			}
//		}
		EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE2线路挂起位
	}
	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3)!=1)//首先判断步进电机行走的方向避免反向出错
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
			{
		        TIM_Cmd(TIM2, DISABLE);
			     // LED2_on;
		        step_num_2=0;//置电机步数为0
		        step_spot_2=0;//电机的坐标为0
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);  //清除LINE2线路挂起位
	}
}


