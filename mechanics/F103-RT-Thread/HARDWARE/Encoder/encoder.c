#include "stm32f10x.h"
#include "encoder.h"
#include "Step_Motor.h"
#include "math.h"
extern 		u32 KeyValue;   //按键值
int16_t r_now;
TIM_TypeDef* Tim_S=TIM4;
uint16_t CPR;

struct Encoder_Stat Encoders={0, 0, 0, 1, 1, 1,0};
void QEI_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	CPR = (u16)ENC_PPR<<2;
	/*
	CH1A--PA6_T3_C1      CH2A--PA0_T2_C1      CH3A--PC6_T8_C1      CH4A--PB6_T4_C1
	CH1B--PA7_T3_C2      CH2B--PA1_T2_C2      CH3B--PC7_T8_C2      CH4B--PB7_T4_C2
	*/
	/* enable the periph clock */

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);

	/* configue IO */
	// configue CH1 & CH2 IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* configue timer */
	// configue CH1****TIM3****
	TIM_DeInit(TIM4);		
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; 
	TIM_TimeBaseStructure.TIM_Period = CPR-1; 			
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = (u8)0;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);

	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	/* reset the timer3   */
	TIM4->CNT = (u16)0;

	/* enable the timer3 */
	TIM_Cmd(TIM4, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM4_GPIO_Config(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 
	 //GPIOA的时钟
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	 GPIO_Init(GPIOA, &GPIO_InitStructure);
 	 
 	 GPIO_SetBits(GPIOA,GPIO_Pin_4); 
}

int32_t Int32Abs(int32_t Data)
{
	if(Data<0)
		return -Data;
	else 
		return Data;
}
void TIM4_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
    if(!(TIM4->CR1&0x0010))
    {/* count up overflow interrupt */
        //Com_Printf("up overflow\n\r");
        r_now++;
    }
    else
    {/* count down overflow interrupt */
        //Com_Printf("down overflow\n\r");
        r_now--;
    }
}

int32_t GET_ENCODER(void)
{
	int32_t temp;
// 	assert_param((i>0)&&(i<5));
	temp = (int32_t)(	((int32_t)(r_now))*CPR + (int32_t)(Tim_S->CNT) );
	return temp;
}
 void  Encoder_Update(void)
{
	int64_t now;
	int32_t step;

	now	= GET_ENCODER();
	step = now-Encoders.Now;
	if(step<2000&&step>-2000)
	{
		Encoders.Now = now;
	  Encoders.Total = Encoders.Now-Encoders.Last;
	  if (step == Int32Abs(step))
			Encoders.Distance += Int32Abs(step)*Encoders.Convert1*Encoders.dir;
	  else
			Encoders.Distance -= Int32Abs(step)*Encoders.Convert2*Encoders.dir;
	 }
	
	 //归一化处理
	 if(Encoders.Distance>=period_clk/2)
	 {
		 Encoders.Distance -=period_clk;
	 }
	 	 if(Encoders.Distance<-period_clk/2)
	 {
		 Encoders.Distance +=period_clk;
	 }
	 //步进电机3判断位置
	 if(step_motor_3_flag)
	 {
		 if(fabs(step_spot_3_target-Encoders.Distance)<10)  //小于误差允许范围的话
		 {
			  TIM_Cmd(TIM1, DISABLE);//停止转动
				step_motor_3_flag=0;  //开始旋转
		 }
	 }
	 
	 
}
void Encoder_Init(void)
{

	Encoders.Distance=0.0;
	Encoders.Last=Encoders.Now;
	Encoders.Total=0;
}
 

//后期通过Tim6基本定时器更新码盘参数    不用系统滴答定时器
void TIM6_Int_Init(void)        //1ms一次中断
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = 999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式  //基本定时器只能向上计数
	
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
  TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM6_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx更新中断标志 
		Encoder_Update(); 
		}
}


/*************************************************
函数: void RCC_Configuration(void)
功能: 复位和时钟控制 配置
参数: 无
返回: 无
**************************************************/
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;                    //定义外部高速晶体启动状态枚举变量
  RCC_DeInit();                                    //复位RCC外部设备寄存器到默认值
  RCC_HSEConfig(RCC_HSE_ON);                       //打开外部高速晶振
  HSEStartUpStatus = RCC_WaitForHSEStartUp();      //等待外部高速时钟准备好
  if(HSEStartUpStatus == SUCCESS)                  //外部高速时钟已经准别好
  {
   FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); //开启FLASH预读缓冲功能，加速FLASH的读取。所有程序中必须的用法.位置：RCC初始化子函数里面，时钟起振之后
   FLASH_SetLatency(FLASH_Latency_2);                    //flash操作的延时
      	
    RCC_HCLKConfig(RCC_SYSCLK_Div1);               //配置AHB(HCLK)时钟等于==SYSCLK
    RCC_PCLK2Config(RCC_HCLK_Div1);                //配置APB2(PCLK2)钟==AHB时钟
    RCC_PCLK1Config(RCC_HCLK_Div1);                //配置APB1(PCLK1)钟==AHB1/2时钟
         
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟 * 9 = 72MHz
    RCC_PLLCmd(ENABLE);                                   //使能PLL时钟
   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)    //等待PLL时钟就绪
    {
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);            //配置系统时钟 = PLL时钟
    while(RCC_GetSYSCLKSource() != 0x08)                  //检查PLL时钟是否作为系统时钟
    {
    }
  }

}
void Encoder_Configuration(void)
{
  //RCC_Configuration();
	QEI_Init();   //码盘引脚初始化
	Encoder_Init();  //参数初始化
	TIM6_Int_Init();          //更新码盘参数初始化
	//TIM4_GPIO_Config();   //拉低PA4，只有队里的协主控需求
}

