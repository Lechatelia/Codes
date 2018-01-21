#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart1.h"
#include "usart_3.h"
#include "led.h"
#include "remote.h"
#include "encoder.h"
#include "motor.h"
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

//主程序
 void main_function_entry( )
{
	while(1)
	{
		LED0=0;  
		delay_ms(1000);
		
        
		LED0=1;
		delay_ms(1000);		
		


		  if(exti_flag==1)
			{
				exti_flag=0;
				switch(key_number)            //步进电机+舵机测试代码，变量通过按键更改
					{
						case 0:
							step_motor_1(0);
							step_motor_2(10);       //步进电机
						  step_motor_3(0);
							set_Servp_angle(45);      //舵机
							setspeed_motor1(stop,99,100000);  //直线电机
							break;                    
						case 1:
							step_motor_1(100000);
							step_motor_2(100000);
							step_motor_3(1);
							set_Servp_angle(0);
							setspeed_motor1(backward,99,100000);
							break;
						case 2:
							step_motor_1(-100000);
							step_motor_2(-100000);
						  step_motor_3(-1);
							set_Servp_angle(90);
							setspeed_motor1(forward,99,50000);
							
							break;
						default:
							break;
					}
			}
		}
}



int main(void)
{  
		RCC_Configuration();    //高速时钟的启用需要放在前面，不然后面uart3的时序会发生紊乱
	  delay_init();  //主要是在中断函数里面用该函数用于防抖  
	  Step_Motor_Init(); //三个步进电机的初始化
		My_EXTI_Init(); //外部中断初始化，用于步进电机的零点
    LED_Init();    	//初始化LED
    //Remote_Init() ;  //红外遥控
  	USART1_DMA_Init(); //uart1初始化
	  USART3_DMA_Init();  //uart3初始化
	  Encoder_Configuration();//码盘初始化
	  DC_Motor_init_motor(); //直流电机初始化
		Servo_init(900);//舵机初始化  需注意这里需要初始化就为0度
	
    
		main_function_entry();
   
}


 

