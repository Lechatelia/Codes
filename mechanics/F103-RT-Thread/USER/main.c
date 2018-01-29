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
#include "parameters.h"
//#include "stm32f10x.h"

#define TASK 0//调试或者任务运行1 or 0
  extern int exti_flag ; 
  extern int key_number;
	
uint8_t Task_queue=0x01;//需要运行程序编号 0x00代表目前处于空余状态
uint8_t Task_state=0x00;//当前程序运行状态 0x00代表目前处于空余状态
/************************************************
//机创比赛代码V1.0
//搬砖人：Lechatelia
************************************************/

 
static struct rt_thread led0_thread;//线程控制块
static struct rt_thread main_task_thread;//线程控制块
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led0_thread_stack[1024];//线程栈 
static rt_uint8_t main_task_thread_stack[1024];//线程栈 



//////////////////////////////////////////////////////////////////////////////////	
//停车过程 复位点出发	
void state1()
{
	    step_motor_3_work(1,0); //步进电机开始旋转到0度，便于进入车库
	
	    step_motor_2(Distance_Y_0);   //步进电机2前进Distance_Y_0
			rt_thread_delay(Distance_Y_0/10);  //为了防止此时挂着开关2
			LED3_on; 
			step_motor_1(Distance_X_1);		//步进电机1前进Distance_X_1
			rt_thread_delay(Distance_X_1/10);  //等待移动结束       
			while(step_num_1>0); //确认移动结束
			
			DC_Motor_positive(Time_Positive);//夹持轮子
			LED2_off;
			rt_thread_delay(Time_Positive/10);  //等待夹持结束
			
			set_Servp_angle(90);   //舵机旋转90
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on;
			step_motor_2(Distance_Y_1);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_1/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
			
			set_Servp_angle(0);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on; 
			step_motor_1(Distance_X_2);		//步进电机1前进Distance_X_2
			rt_thread_delay(Distance_X_2/10);  //等待移动结束       
			while(step_num_1>0); //确认移动结束
			
			set_Servp_angle(90);   //舵机旋转90
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			//进入车库
			while(step_motor_3_flag);//进入车库前判断步进电机三是否在旋转
			
			LED3_on;
			step_motor_2(Distance_Y_2);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_2/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
			
			//到达目的地
			
			//回到停放处
			DC_Motor_negative(Time_Negative);//松开轮子
			LED2_on;
			rt_thread_delay(Time_Negative/10);  //等待松开结束
			
			LED3_on;
			step_motor_2(-Distance_Y_2);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_2/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
			
			set_Servp_angle(0);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			  step_motor_3_work(1,500); //电机旋转一定角度便于下次的再次进入
			
			LED3_on;
			step_motor_1(-Distance_X_2-Distance_X_1+Distance_X_0);		//步进电机1前进Distance_X_1
			rt_thread_delay((Distance_X_1+Distance_X_2-Distance_X_0)/10);  //等待移动结束       
			while(step_num_1>0); //确认移动结束
			
			set_Servp_angle(90);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on;
			step_motor_2(-Distance_Y_1);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_1/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
      		 
			set_Servp_angle(0);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);

//理论已经回到点(X_0,Y_0)
}



//////////////////////////////////////////////////////////////////////////////////	
//取车过程 复位点出发
void state2()
{
	    step_motor_3_work(1,0); //步进电机开始旋转到0度，便于进入车库
	
			LED2_on;
	    step_motor_2(Distance_Y_0);   //步进电机2前进Distance_Y_0
			rt_thread_delay((Distance_Y_0)/10);  //为了防止此时挂着开关2
	    while(step_num_2>0); //确认移动结束
	
			set_Servp_angle(90);   //舵机旋转90
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
	    
			LED3_on;
			step_motor_2(Distance_Y_1);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_1/10);  //等待移动结束
			
	
			set_Servp_angle(0);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
	
			LED3_on; 
			step_motor_1(Distance_X_1+Distance_X_2);		//步进电机1前进Distance_X_1
			rt_thread_delay((Distance_X_1+Distance_X_2)/10);  //等待移动结束       
			while(step_num_1>0); //确认移动结束
			
			
			set_Servp_angle(90);   //舵机旋转90
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);

			
			//进入车库
			while(step_motor_3_flag);//进入车库前判断步进电机三是否在旋转
	
			LED3_on;
			step_motor_2(Distance_Y_2);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_2/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
			
			//到达目的地
			
			//回到停放处
			DC_Motor_positive(Time_Positive);//夹持轮子
			LED2_off;
			rt_thread_delay(Time_Positive/10);  //等待夹持结束
			
			LED3_on;
			step_motor_2(-Distance_Y_2);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_2/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
			
			set_Servp_angle(0);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			step_motor_3_work(1,500); //电机旋转一定角度便于下次的再次进入
			
			LED3_on;
			step_motor_1(-Distance_X_2);		//步进电机1前进Distance_X_1
			rt_thread_delay(Distance_X_2/10);  //等待移动结束       
			while(step_num_1>0); //确认移动结束
			
			set_Servp_angle(90);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on;
			step_motor_2(-Distance_Y_1);   //步进电机2前进Distance_Y_1
			rt_thread_delay(Distance_Y_1/10);  //等待移动结束
			while(step_num_2>0); //确认移动结束
			
			set_Servp_angle(0);   //舵机旋转0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			DC_Motor_negative(Time_Negative);//松开轮子
			LED2_on;
			rt_thread_delay(Time_Negative/10);  //等待松开结束
						
			LED3_on;
			step_motor_1(Distance_X_1-Distance_X_0);		//步进电机1前进Distance_X_1
			rt_thread_delay((Distance_X_1-Distance_X_0)/10);  //等待移动结束       
			while(step_num_1>0); //确认移动结束

//理论已经回到点(X_0,Y_0)
}


//////////////////////////////////////////////////////////////////////////////////	
//线程LED0
static void led0_thread_entry(void* parameter)
{
	while(1)
	{	
	
#if TASK
    LED1_on;
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 
        
		LED1_off;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);     
		
#else    //平时调试模块代码
		while(1)
		{
			step_motor_3_work(1,500);
			while(step_motor_3_flag);
			rt_thread_delay(RT_TICK_PER_SECOND);
			rt_thread_delay(RT_TICK_PER_SECOND);
			step_motor_3_work(1,0);
			while(step_motor_3_flag);
			rt_thread_delay(RT_TICK_PER_SECOND);
			rt_thread_delay(RT_TICK_PER_SECOND);
		}
		
		
		  if(exti_flag==1)
			{
			
				switch(key_number)            //变量通过按键更改
					{
						case 0:
//							step_motor_1(10);
//							step_motor_2(10);       //步进电机
						  //step_motor_3(0);
//							set_Servp_angle(45);      //舵机
									//step_motor_3(0);
//							setspeed_motor1(stop,99,50000);  //直线电机
							break;                    
						case 1:
//							step_motor_1(32000);
//							step_motor_2(32000);
							//step_motor_3(1);
							//	step_motor_3_work(1,500);
//							//step_motor_3_work(1,500);  //此函数需要配合码盘使用
							  //set_Servp_angle(0);
								//DC_Motor_positive(Time_Positive);//夹持轮子
//							setspeed_motor1(backward,50,50000);
							break;
						case 2:
//							step_motor_1(-32000);
//							step_motor_2(-32000);
							//	step_motor_3(-1);
							//	step_motor_3_work(1,0);
								//set_Servp_angle(90);
					       //DC_Motor_negative(Time_Negative);//松开轮子
//							setspeed_motor1(forward,50,50000);
//							
							break;
						default:
							break;
					}
				exti_flag=0;
			}
	#endif
		}
}

//线程LED1
static void main_task_thread_entry(void* parameter)
{
	#if TASK  //任务模式代码
	
	
	   while(unlimit_flag_1||unlimit_flag_2)   //等待步进电机复位
			{
			LED2_on;
			rt_thread_delay(RT_TICK_PER_SECOND);                 
			}		
			while(1)    
			{
					if(Task_state==0x00)  //当前处于任务空余状态
					{
						switch(Task_queue)            //变量通过按键更改
							{
							case 0x00:    //任务队列为空
								break;
							case 0x01:
								Task_queue=0x00;  //队列清空
								Task_state=0x01;  //is busy
								state1();    
								Task_state=0x00;	//is free
								break;
							case 0x02:
								Task_queue=0x00;
								Task_state=0x01;
								state2();    
								Task_state=0x00;
								break;
							default:
									break;								
							}
					}
					rt_thread_delay(RT_TICK_PER_SECOND);  //就在这里休息那么一秒钟咩！！！
			}
			
	#else     //调试模式代码
		while(1)
			{
			LED2_on;
			rt_thread_delay(RT_TICK_PER_SECOND);                //延时 
					
			LED2_off;  
			rt_thread_delay(RT_TICK_PER_SECOND);	
			}
	#endif
}




//////////////////////////////////////////////////////////////////////////////////	 
//主程序								  
////////////////////////////////////////////////////////////////////////////////// 
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
		Servo_init(1730);//舵机初始化  需注意这里需要初始化就为0度
	
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
				   
	rt_thread_init(&main_task_thread,              		//线程控制块
                   "main_task",                   		//线程名字，在shell里面可以看到
                   main_task_thread_entry,        		//线程入口函数
                   RT_NULL,                     	//线程入口函数参数
                   &main_task_thread_stack[0], 		//线程栈起始地址
                   sizeof(main_task_thread_stack),  	//线程栈大小
                   2,    //线程的优先级
                   20);         				   

	rt_thread_startup(&main_task_thread);  
   
}


 

