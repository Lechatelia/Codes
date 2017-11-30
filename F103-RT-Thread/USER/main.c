#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "remote.h"
#include <rtthread.h>
 
 
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
		rt_thread_delay(RT_TICK_PER_SECOND);                //延时 
        
		LED0=1;  
		rt_thread_delay(RT_TICK_PER_SECOND);                //延时 
	}
}

//线程LED1
static void led1_thread_entry(void* parameter)
{
	while(1)
	{
		LED1=0;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 
        
		LED1=1;   
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //延时 
	}
}


int main(void)
{
     
    LED_Init();    	//初始化LED  
    Remote_Init() ;  //红外遥控
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


 

