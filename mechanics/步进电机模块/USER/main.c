#include "stm32f10x.h"
#include "exti.h"
#include "Step_Motor.h"
#include "delay.h"
 int flag=0;
 int dir_flag=0;
 int main(void)
 {	
      TIM2_Init(); //pa2
	    TIM3_Init(); //pa6
	 EXTI_PA2_config();
	 step_dir_init(); //方向初始化
	 unlimit_step_1();//复位1
	 unlimit_step_2();//复位2
	 delay_init();
	 while(1)
	 {
		    if(flag)
				{
					delay_ms (1000);
					flag=0;
					if(dir_flag==0)
					{
						dir_flag=1;
						step_motor_1(32000);
					}
					else
					{
						dir_flag=0;
						step_motor_1(-32000);
					}
				}
	  }
	 
 }

