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

#define TASK 1  //���Ի�����������
  extern int exti_flag ; 
  extern int key_number;
/************************************************
//������������V1.0
//��ש�ˣ�Lechatelia
************************************************/

 
static struct rt_thread led0_thread;//�߳̿��ƿ�
static struct rt_thread main_task_thread;//�߳̿��ƿ�
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led0_thread_stack[1024];//�߳�ջ 
static rt_uint8_t main_task_thread_stack[1024];//�߳�ջ 



//////////////////////////////////////////////////////////////////////////////////	
//ͣ������ λ�ڳ�����		δ�������̣��������3����
void state1()
{
	
			LED3_on; 
			step_motor_1(Distance_X_1);		//�������1ǰ��Distance_X_1
			rt_thread_delay(Distance_X_1/10);  //�ȴ��ƶ�����       
			while(step_num_1>0); //ȷ���ƶ�����
			
			DC_Motor_positive(Time_Positive);//�г�����
			LED2_off;
			rt_thread_delay(Time_Positive/10);  //�ȴ��гֽ���
			
			set_Servp_angle(90);   //�����ת90
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on;
			step_motor_2(Distance_Y_1);   //�������2ǰ��Distance_Y_1
			rt_thread_delay(Distance_Y_1/10);  //�ȴ��ƶ�����
			while(step_num_2>0); //ȷ���ƶ�����
			
			set_Servp_angle(0);   //�����ת0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on; 
			step_motor_1(Distance_X_2);		//�������1ǰ��Distance_X_1
			rt_thread_delay(Distance_X_2/10);  //�ȴ��ƶ�����       
			while(step_num_1>0); //ȷ���ƶ�����
			
			set_Servp_angle(90);   //�����ת90
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			//���복��
			LED3_on;
			step_motor_2(Distance_Y_2);   //�������2ǰ��Distance_Y_1
			rt_thread_delay(Distance_Y_2/10);  //�ȴ��ƶ�����
			while(step_num_2>0); //ȷ���ƶ�����
			
			//����Ŀ�ĵ�
			
			//�ص�ͣ�Ŵ�
			DC_Motor_negative(Time_Negative);//�ɿ�����
			LED2_on;
			rt_thread_delay(Time_Positive/10);  //�ȴ��гֽ���
			
			LED3_on;
			step_motor_2(-Distance_Y_2);   //�������2ǰ��Distance_Y_1
			rt_thread_delay(Distance_Y_2/10);  //�ȴ��ƶ�����
			while(step_num_2>0); //ȷ���ƶ�����
			
			set_Servp_angle(0);   //�����ת0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on;
			step_motor_1(Distance_X_2+Distance_X_1);		//�������1ǰ��Distance_X_1
			rt_thread_delay((Distance_X_1+Distance_X_2)/10);  //�ȴ��ƶ�����       
			while(step_num_1>0); //ȷ���ƶ�����
			
			set_Servp_angle(90);   //�����ת0
			LED3_off;
			rt_thread_delay(RT_TICK_PER_SECOND/2);
			
			LED3_on;
			step_motor_2(-Distance_Y_1);   //�������2ǰ��Distance_Y_1
			rt_thread_delay(Distance_Y_1/10);  //�ȴ��ƶ�����
			while(step_num_2>0); //ȷ���ƶ�����



//�����Ѿ��ص�������
}


//�߳�LED0
static void led0_thread_entry(void* parameter)
{
	while(1)
	{
		LED1_on;
		rt_thread_delay(RT_TICK_PER_SECOND/2);                //��ʱ 
        
		LED1_off;  
		rt_thread_delay(RT_TICK_PER_SECOND/2);   
 		
#if TASK
    
		
#else    //ƽʱ����ģ�����
		  if(exti_flag==1)
			{
				exti_flag=0;
				switch(key_number)            //����ͨ����������
					{
						case 0:
							step_motor_1(10);
							step_motor_2(10);       //�������
						  step_motor_3(0);
							set_Servp_angle(45);      //���
							setspeed_motor1(stop,99,50000);  //ֱ�ߵ��
							break;                    
						case 1:
							step_motor_1(32000);
							step_motor_2(32000);
							step_motor_3(1);
							//step_motor_3_work(1,500);  //�˺�����Ҫ�������ʹ��
							set_Servp_angle(0);
							setspeed_motor1(backward,99,50000);
							break;
						case 2:
							step_motor_1(-32000);
							step_motor_2(-32000);
						  step_motor_3(-1);
							set_Servp_angle(90);
							setspeed_motor1(forward,99,50000);
							
							break;
						default:
							break;
					}
			}
	#endif
		}
}

//�߳�LED1
static void main_task_thread_entry(void* parameter)
{
	#if TASK
	
	
	   while(unlimit_flag_1||unlimit_flag_2)   //�ȴ����������λ
			{
			LED2_on;
			rt_thread_delay(RT_TICK_PER_SECOND);                 
			}
			
	     state1();   //ͣ���̺߳���
			
			
			
			
			while(1)    
			{
			LED4_on;
			rt_thread_delay(RT_TICK_PER_SECOND);                //��ʱ 
					
			LED4_off;  
			rt_thread_delay(RT_TICK_PER_SECOND);	
			}
			
	#else     //����ģ�����
		while(1)
			{
			LED2_on;
			rt_thread_delay(RT_TICK_PER_SECOND);                //��ʱ 
					
			LED2_off;  
			rt_thread_delay(RT_TICK_PER_SECOND);	
			}
	#endif
}




//////////////////////////////////////////////////////////////////////////////////	 
//������								  
////////////////////////////////////////////////////////////////////////////////// 
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
				   
	rt_thread_init(&main_task_thread,              		//�߳̿��ƿ�
                   "main_task",                   		//�߳����֣���shell������Կ���
                   main_task_thread_entry,        		//�߳���ں���
                   RT_NULL,                     	//�߳���ں�������
                   &main_task_thread_stack[0], 		//�߳�ջ��ʼ��ַ
                   sizeof(main_task_thread_stack),  	//�߳�ջ��С
                   2,    //�̵߳����ȼ�
                   20);         				   

	rt_thread_startup(&main_task_thread);  
   
}


 

