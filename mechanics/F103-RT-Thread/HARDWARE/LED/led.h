#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
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

void LED_Init(void);//��ʼ��

		 				    
#endif
