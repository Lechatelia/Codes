#include "stm32f10x.h"
#include "motor.h"
#include "delay.h"
#include "led.h"
#include "exti.h"


 int main(void)
 {	
	 delay_init();
	 GPIO_init_motor();
	 PWM_init_motor();
	 LED_Init();
	 init_exti();
	 while(1)
	 {
//		 setspeed_motor1(forward,500);
//		 delay_ms(10);
//		 setspeed_motor1(backward,500);
//		 delay_ms(10);
//		 setspeed_motor1(stop,500);
//		 delay_ms(10);
//		 setspeed_motor1(backward,500);
//		 delay_ms(10);
//		 setspeed_motor1(stop,500);
//		 delay_ms(10);
	 }
	 
	 
 }
