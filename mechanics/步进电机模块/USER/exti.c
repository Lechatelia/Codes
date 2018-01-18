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
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;					//�����ȼ�1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
}
void EXTI_PA2_config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	 	EXTI_InitTypeDef EXTI_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO,ENABLE);

    NVIC_Exti_Configuration();
	    
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��Pd2
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
		
		EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��Pd2
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource7);
	
	  EXTI_InitStructure.EXTI_Line=EXTI_Line7;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
}

void EXTI9_5_IRQHandler(void)//�����ⲿ�жϣ��ӽ�����
{
	if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{
//		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7)!=1)//�����жϲ���������ߵķ�����ⷴ�����
//		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)
			{
		        TIM_Cmd(TIM3, DISABLE);
			    //  LED1_on;
		        step_num_1=0;//�õ������Ϊ0
		        step_spot_1=0;//���������Ϊ0
				if(flag==0)
					flag=1;
			}
//		}
		EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE2��·����λ
	}
	
	if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_3)!=1)//�����жϲ���������ߵķ�����ⷴ�����
		{
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==0)
			{
		        TIM_Cmd(TIM2, DISABLE);
			     // LED2_on;
		        step_num_2=0;//�õ������Ϊ0
		        step_spot_2=0;//���������Ϊ0
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line6);  //���LINE2��·����λ
	}
}


