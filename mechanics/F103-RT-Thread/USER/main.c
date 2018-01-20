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
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 
        
		LED0=1;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 

//	switch(key_number)            //DC_Motor���Դ��룬����ͨ����������
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
				switch(key_number)            //�������+������Դ��룬����ͨ����������
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

//�߳�LED1
static void led1_thread_entry(void* parameter)
{
	while(1)
	{
		LED1=0;  
		//delay_ms(1000);
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 
        
		LED1=1;  
	  //delay_ms(1000);		
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 
		
		
	}
}


int main(void)
{  
		RCC_Configuration();    //����ʱ�ӵ�������Ҫ����ǰ�棬��Ȼ����uart3��ʱ��ᷢ������
	  delay_init();  //��Ҫ�����жϺ��������øú������ڷ���  //�ú����Ѿ�lechatelia����  ����ʱ����ܺܲ�׼ȷ
	  Step_Motor_Init(); //������������ĳ�ʼ��
		My_EXTI_Init(); //�ⲿ�жϳ�ʼ�������ڲ�����������
    LED_Init();    	//��ʼ��LED
    //Remote_Init() ;  //����ң��
  	USART1_DMA_Init(); //uart1��ʼ��
	  USART3_DMA_Init();  //uart3��ʼ��
	  Encoder_Configuration();//���̳�ʼ��
	  DC_Motor_init_motor(); //ֱ�������ʼ��
		Servo_init(900);//�����ʼ��  ��ע��������Ҫ��ʼ����Ϊ0��
	
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


 

