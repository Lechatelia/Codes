#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart1.h"
#include "usart_3.h"
#include "led.h"
#include "remote.h"
#include "encoder.h"
#include "motor.h"
#include <rtthread.h>
#include "Servo.h"
#include "Step_Motor.h"
#include "exti.h"
//#include "stm32f10x.h"
  extern int exti_flag ; 
  extern int key_number;
/************************************************
 ALIENTEK战舰STM32开发板实验1
 跑马灯实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 
static struct rt_thread led0_thread;//线程控制块
static struct rt_thread led1_thread;//线程控制块
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led0_thread_stack[1024];//线程栈 
static rt_uint8_t rt_led1_thread_stack[1024];//线程栈 

//线程LED0
static void led0_thread_entry(void* parameter)
{
	while(1)
	{
		LED0=0;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 
        
		LED0=1;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 

//	switch(key_number)            //DC_Motor测试代码，变量通过按键更改
//			{
//					case 0:
//					setspeed_motor1(stop,500);
//				  break;
//				case 1:
//					setspeed_motor1(backward,500);
//					break;
//				case 2:
//					setspeed_motor1(stop,500);	
//					break;
//				default:
//					setspeed_motor1(stop,500);
//			}
		  if(exti_flag==1)
			{
				exti_flag=0;
				switch(key_number)            //步进电机+舵机测试代码，变量通过按键更改
					{
						case 0:
							step_motor_1(10);
							step_motor_2(10);
						  step_motor_3(0);
							set_Servp_angle(45);
							setspeed_motor1(stop,999);
							break;
						case 1:
							step_motor_1(32000);
							step_motor_2(32000);
							step_motor_3(1);
							set_Servp_angle(0);
							setspeed_motor1(backward,999);
							break;
						case 2:
							step_motor_1(-32000);
							step_motor_2(-32000);
						  step_motor_3(-1);
							set_Servp_angle(90);
							setspeed_motor1(forward,999);
							
							break;
						default:
							break;
					}
			}
		}
}

//线程LED1
static void led1_thread_entry(void* parameter)
{
	while(1)
	{
		LED1=0;  
		//delay_ms(1000);
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 
        
		LED1=1;  
	  //delay_ms(1000);		
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 
		
		
	}
}


int main(void)
{  
		RCC_Configuration();    //高速时钟的启用需要放在前面，不然后面uart3的时序会发生紊乱
	  delay_init();  //主要是在中断函数里面用该函数用于防抖  //该函数已经lechatelia更改  所以时间可能很不准确
	  Step_Motor_Init(); //三个步进电机的初始化
		My_EXTI_Init(); //外部中断初始化，用于步进电机的零点
    LED_Init();    	//初始化LED
    //Remote_Init() ;  //红外遥控
  	USART1_DMA_Init(); //uart1初始化
	  USART3_DMA_Init();  //uart3初始化
	  Encoder_Configuration();//码盘初始化
	  DC_Motor_init_motor(); //直流电机初始化
		Servo_init(900);//舵机初始化  需注意这里需要初始化就为0度
	
    // 创建静态线程
    rt_thread_init(&led0_thread,              		//线程控制块
                   "led0",                    		//线程名字，在shell里面可以看到
                   led0_thread_entry,         		//线程入口函数
                   RT_NULL,                  		//线程入口函数参数
                   &rt_led0_thread_stack[0], 		//线程栈起始地址
                   sizeof(rt_led0_thread_stack), 	//线程栈大小
                   3,    //线程的优先级
                   20);                         //线程时间片
				   
    rt_thread_startup(&led0_thread);             
				   
	rt_thread_init(&led1_thread,              		//线程控制块
                   "led1",                   		//线程名字，在shell里面可以看到
                   led1_thread_entry,        		//线程入口函数
                   RT_NULL,                     	//线程入口函数参数
                   &rt_led1_thread_stack[0], 		//线程栈起始地址
                   sizeof(rt_led1_thread_stack),  	//线程栈大小
                   2,    //线程的优先级
                   20);         				   

	rt_thread_startup(&led1_thread);  
   
}


 

