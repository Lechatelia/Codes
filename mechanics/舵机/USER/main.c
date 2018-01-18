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
  pwm_init(duty);//频率50HZ，参数为初始占空比//pwm1初始化
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
	
	
	
	
	    //电调初始化代码，此时不用外部中断注意中断不用初始化
	    //pwm_init(0.1);//油门最大
	     // GPIO_Configuration();
	   // while( GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)==Bit_SET);//等待按键key1按下
  	 // TIM_SetCompare2(TIM2,(int)(20000*0.05*72/200));//油门最小
	   //while(1);
