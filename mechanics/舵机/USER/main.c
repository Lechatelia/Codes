#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "pwm.h"
//#include "GPIO.h"
#include "EXTI.h"
#include "lcd.h"
  double duty=0.075;
	int CCR=1500;
	int CCR_now=1500;
 int flag=0;	
 int main(void)
 {	
	all_lcd_init();
  SystemInit();	    	 		
  delay_init();	
  delay_ms(10);
	// duty=0.5;
  pwm_init(duty);//Ƶ��50HZ������Ϊ��ʼռ�ձ�//pwm1��ʼ��
	Init_Exti(); 
	
   while(1)
	 {

//		 while(CCR-CCR_now)
//		 {
//			 if(CCR>CCR_now)
//			 {
//				 CCR_now+=100;
//				 TIM_SetCompare2(TIM2,(int)(CCR_now));
//				 delay_ms(300);
//			  }
//			 else if(CCR<CCR_now)
//			 {
//				  CCR_now-=100;
//				 TIM_SetCompare2(TIM2,(int)(CCR_now));
//				 delay_ms(300);
//			 }
//		 }
					
					
   }
	 
	}
	
	
	
	
	    //�����ʼ�����룬��ʱ�����ⲿ�ж�ע���жϲ��ó�ʼ��
	    //pwm_init(0.1);//�������
	     // GPIO_Configuration();
	   // while( GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)==Bit_SET);//�ȴ�����key1����
  	 // TIM_SetCompare2(TIM2,(int)(20000*0.05*72/200));//������С
	   //while(1);
