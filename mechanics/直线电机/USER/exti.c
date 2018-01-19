#include "exti.h"
 void init_exti(){  //pa2
   EXTI_InitTypeDef EXTI_InitStructure;
	 GPIO_InitTypeDef  GPIO_InitStructure;	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE);
  
	 
		GPIO_InitStructure.GPIO_Pin = KEY_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	 
		GPIO_Init(GPIOA, &GPIO_InitStructure);		

		GPIO_InitStructure.GPIO_Pin = KEY_1|KEY_2|KEY_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;	 
		GPIO_Init(GPIOE, &GPIO_InitStructure);		 
	 
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    my_NVIC_configuration();
 
	
		EXTI_InitStructure.EXTI_Line=EXTI_Line0;
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);

		
		EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4;
		EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
 }
                          
 void my_NVIC_configuration()
	{
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		
		NVIC_InitStructure.NVIC_IRQChannel=EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel=EXTI4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	} 
