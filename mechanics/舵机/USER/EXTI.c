#include "EXTI.h"
 void Init_Exti(){ 
   EXTI_InitTypeDef EXTI_InitStructure;
	 GPIO_InitTypeDef  GPIO_InitStructure;	 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO,ENABLE);
  
	 My_NVIC_Configuration();
	 //pd2,pc10,11,12
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;		 
	 GPIO_Init(GPIOD, &GPIO_InitStructure);	
   
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);		 

		GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource2);
	 EXTI_InitStructure.EXTI_Line=EXTI_Line2;
	 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	 EXTI_Init(&EXTI_InitStructure);
		 
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
		EXTI_InitStructure.EXTI_Line=EXTI_Line10;
		 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	 
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
		EXTI_InitStructure.EXTI_Line=EXTI_Line11;
		 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	 
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource12);
	  EXTI_InitStructure.EXTI_Line=EXTI_Line12;
		 EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	 EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	  EXTI_Init(&EXTI_InitStructure);
 

 }
 
  void My_NVIC_Configuration()
 {
	 NVIC_InitTypeDef NVIC_InitStructure;
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
   
	
	 NVIC_InitStructure.NVIC_IRQChannel=EXTI2_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	 

	  NVIC_InitStructure.NVIC_IRQChannel=EXTI15_10_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	 NVIC_Init(&NVIC_InitStructure);
	 
 }