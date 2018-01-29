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
TIM_TimeBaseStructure.TIM_Period=20000;  //0��7199�պ�7200����
TIM_TimeBaseStructure.TIM_Prescaler=72; //�趨��Ƶֵ
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//ʱ����ʼ�����
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
//ʱ����ʼ��Ӧ��
	
	//����ͨ��1
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=2080;//��������ֵ����ʱ��ƽ��������    ��ʼ���
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3,ENABLE); //ʹ�����ؼĴ���ARR
	TIM_Cmd(TIM3,ENABLE);    //ʹ�ܶ�ʱ��
}


//���ڶ�����Ƕȣ�����ֻ���ڶ������90��
void set_Servp_angle(int angle)
{ int CCR;
	CCR=2080-(double)angle*(880)/90;  //��ҪԤ��֪��0��90�ȵ�ռ�ձ�
	TIM_SetCompare1(TIM3,(int)(CCR));
}
