#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
								  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PCout(0)// PB5
#define LED1 PCout(1)// PE5	

#define LED1_on GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define LED2_on GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define LED3_on GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define LED4_on GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define LED1_off GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define LED2_off GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define LED3_off GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define LED4_off GPIO_SetBits(GPIOC,GPIO_Pin_3)	

void LED_Init(void);//≥ı ºªØ

		 				    
#endif
