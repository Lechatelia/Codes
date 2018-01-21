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
 ALIENTEKս��STM32������ʵ��1
 �����ʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//������
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
				switch(key_number)            //�������+������Դ��룬����ͨ����������
					{
						case 0:
							step_motor_1(0);
							step_motor_2(10);       //�������
						  step_motor_3(0);
							set_Servp_angle(45);      //���
							setspeed_motor1(stop,99,100000);  //ֱ�ߵ��
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
		RCC_Configuration();    //����ʱ�ӵ�������Ҫ����ǰ�棬��Ȼ����uart3��ʱ��ᷢ������
	  delay_init();  //��Ҫ�����жϺ��������øú������ڷ���  
	  Step_Motor_Init(); //������������ĳ�ʼ��
		My_EXTI_Init(); //�ⲿ�жϳ�ʼ�������ڲ�����������
    LED_Init();    	//��ʼ��LED
    //Remote_Init() ;  //����ң��
  	USART1_DMA_Init(); //uart1��ʼ��
	  USART3_DMA_Init();  //uart3��ʼ��
	  Encoder_Configuration();//���̳�ʼ��
	  DC_Motor_init_motor(); //ֱ�������ʼ��
		Servo_init(900);//�����ʼ��  ��ע��������Ҫ��ʼ����Ϊ0��
	
    
		main_function_entry();
   
}


 

