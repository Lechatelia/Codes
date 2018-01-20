#include "motor.h"
#include "delay.h"

enum Motor_Dir motor1_dir=stop; //Ĭ��ֹͣ
int delay_flag=0;
int DC_Motor_flag=0; //����Ƿ������ƶ�
long dc_place=0;      //��Ҫ�ƶ����ƶ�λ��

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
 //�Ƚ�ֵ�� 0��99�仯���ٶ���������
void PWM_init_motor( )
{
	
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    //ʱ��ʹ�ܺ���һ��Ҫ������

TIM_TimeBaseStructure.TIM_Period=100-1;  //100us
TIM_TimeBaseStructure.TIM_Prescaler=72-1; //�趨��Ƶֵ
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
	  	
		 
	    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
	
  TIM_ARRPreloadConfig(TIM2,ENABLE); //ʹ�����ؼĴ���ARR
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2,ENABLE);    //ʹ�ܶ�ʱ��
}

void TIM2_IRQHandler(void)                      //�����ж�
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		if(DC_Motor_flag==1)  //�����ƶ�
		{
		
		     if(dc_place<=0)
	      	{
			      setspeed_motor1(stop,0,0); //�����Թرն�ʱ����Ӧ����ʹ���������Ŷ�����͵�ƽ
						DC_Motor_flag=0; //�����ֹ
						 
	        }
	        else
	      	{
		        	dc_place--;
	        }
    	}
	}
	  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

//����1������  ������2���ٶ�0~99  ����3����������������Ӧ�ſ���ʱ��һ����������Ϊ0.1ms
void setspeed_motor1(enum Motor_Dir dir,int speed ,long number)   //�ı䷽������ٶ�
{  
	if(motor1_dir==dir)        //����δ�ı�
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
		 else    //ֹͣ
		 {
			 DC_Motor_flag=0;
			 TIM_SetCompare1(TIM2,0);
			 TIM_SetCompare2(TIM2,0);
		 }
	 }
	else     //����ı�
	{   motor1_dir=dir;  //ע��ñ���ֻ��������ı�
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

//����ʱ��Ϊ�˷�ֹϵͳ�δ�ʱ���Ļ���ʹ�ã�����ʹ�û�����ʱ��tim7
void TIM7_Int_Init(void)        //1msһ���ж�
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = 99; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ  //������ʱ��ֻ�����ϼ���
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM7, DISABLE);  //ʹ��TIMx					 
}

//��ʱ����ʱ1ms
 void TIM7_delay_1ms(void)   
{
	TIM7->CNT = 0;
	TIM_Cmd(TIM7, ENABLE);
	delay_flag=1;
	while(delay_flag==1);
	TIM_Cmd(TIM7, DISABLE); 
}
//��ʱ��7�жϷ������
void TIM7_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM7, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	    delay_flag=0;
		
		}
}

void DC_Motor_init_motor( )
{
	 GPIO_init_motor();
	 PWM_init_motor();
	 TIM7_Int_Init();
	DC_Motor_flag=0; //��ʼ״̬Ϊδ�ƶ�״̬
	//setspeed_motor1(forward,99);
}
