#include "motor.h"
#include "delay.h"

enum Motor_Dir motor1_dir=stop; //Ĭ��ֹͣ

void GPIO_init_motor(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //ʹ��PC�˿�ʱ��
		 	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;				 //LED0-->PC.0 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //�������  ע�����ѡ��ͬ�Ļ��п���û�취����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.13
	GPIO_ResetBits(GPIOA,GPIO_Pin_0| GPIO_Pin_1);						 //PC.0 �����

	

}
 //�Ƚ�ֵ�� 0��1000�仯���ٶ���������
void PWM_init_motor( )
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    //ʱ��ʹ�ܺ���һ��Ҫ������
TIM_TimeBaseStructure.TIM_Period=999;  //0��999�պ�1000����
TIM_TimeBaseStructure.TIM_Prescaler=71; //�趨��Ƶֵ
TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
//ʱ����ʼ�����
TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
//ʱ����ʼ��Ӧ��
	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;	
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //����Ϊ��
	
	//����ͨ��1
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;//��������ֵ����ʱ��ƽ��������
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
	//����ͨ��2
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse=0;
//	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; //����Ϊ��
//	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
//	
//	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse=0;
//	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2,ENABLE); //ʹ�����ؼĴ���ARR
	TIM_Cmd(TIM2,ENABLE);    //ʹ�ܶ�ʱ��
}
void setspeed_motor1(enum Motor_Dir dir,int speed)   //�ı䷽������ٶ�
{  
	if(motor1_dir==dir)        //����δ�ı�
	{
		  if(dir==forward)
		 {
			TIM_SetCompare1(TIM2,speed); 
		 }
		 else if(dir==backward)
		 {
			TIM_SetCompare2(TIM2,speed);
		 }
		 else
		 {
			 TIM_SetCompare1(TIM2,0);
			 TIM_SetCompare2(TIM2,0);
		 }
	 }
	else     //����ı�
	{   motor1_dir=dir;  //ע��ñ���ֻ��������ı�
		   if(dir==forward)
		 {
			TIM_SetCompare2(TIM2,0);
			delay_ms(1);
			TIM_SetCompare1(TIM2,speed); 
		 }
		 else if(dir==backward)
		 {
			TIM_SetCompare1(TIM2,0); 
			delay_ms(1);
			TIM_SetCompare1(TIM2,speed);
		 }
		 else
		 {
			 TIM_SetCompare1(TIM2,0);
			 TIM_SetCompare2(TIM2,0);
		 }
		 
	}

		
}

void DC_Motor_init_motor( )
{
	 GPIO_init_motor();
	 PWM_init_motor();
}
