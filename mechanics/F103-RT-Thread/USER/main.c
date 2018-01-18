#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "uart1.h"
#include "usart_3.h"
#include "led.h"
#include "remote.h"
#include "encoder.h"
#include <rtthread.h>
//#include "stm32f10x.h"
 
 
/************************************************
 ALIENTEKս��STM32������ʵ��1
 �����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 
static struct rt_thread led0_thread;//�߳̿��ƿ�
static struct rt_thread led1_thread;//�߳̿��ƿ�
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led0_thread_stack[1024];//�߳�ջ 
static rt_uint8_t rt_led1_thread_stack[1024];//�߳�ջ 

//�߳�LED0
static void led0_thread_entry(void* parameter)
{
	while(1)
	{
		LED0=0;  
		rt_thread_delay(RT_TICK_PER_SECOND);                //��ʱ 
        
		LED0=1;  
		rt_thread_delay(RT_TICK_PER_SECOND);                //��ʱ 
	}
}

//�߳�LED1
static void led1_thread_entry(void* parameter)
{
	while(1)
	{
		LED1=0;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 
        
		LED1=1;   
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 
	}
}


int main(void)
{  
    LED_Init();    	//��ʼ��LED  
    //Remote_Init() ;  //����ң��
  	//USART1_DMA_Init(); //uart1��ʼ��
	  USART3_DMA_Init();  //uart3��ʼ��
	  Encoder_Configuration();//���̳�ʼ��
    // ������̬�߳�
    rt_thread_init(&led0_thread,              		//�߳̿��ƿ�
                   "led0",                    		//�߳����֣���shell������Կ���
                   led0_thread_entry,         		//�߳���ں���
                   RT_NULL,                  		//�߳���ں�������
                   &rt_led0_thread_stack[0], 		//�߳�ջ��ʼ��ַ
                   sizeof(rt_led0_thread_stack), 	//�߳�ջ��С
                   3,    //�̵߳����ȼ�
                   20);                         //�߳�ʱ��Ƭ
				   
    rt_thread_startup(&led0_thread);             
				   
	rt_thread_init(&led1_thread,              		//�߳̿��ƿ�
                   "led1",                   		//�߳����֣���shell������Կ���
                   led1_thread_entry,        		//�߳���ں���
                   RT_NULL,                     	//�߳���ں�������
                   &rt_led1_thread_stack[0], 		//�߳�ջ��ʼ��ַ
                   sizeof(rt_led1_thread_stack),  	//�߳�ջ��С
                   2,    //�̵߳����ȼ�
                   20);         				   

	rt_thread_startup(&led1_thread);    
}


 

