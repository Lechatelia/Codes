#include "stm32f10x.h"
#include "delay.h"

#define STEP_H_1  	GPIOA->BSRR = GPIO_Pin_2
#define STEP_L_1 	  GPIOA->BRR  = GPIO_Pin_2
#define DIR_ZHENG_1 GPIOA->BSRR = GPIO_Pin_3    
#define DIR_FAN_1 	GPIOA->BRR  = GPIO_Pin_3

#define STEP_H_2  	GPIOA->BSRR = GPIO_Pin_2
#define STEP_L_2 	  GPIOA->BRR  = GPIO_Pin_2
#define DIR_ZHENG_2 GPIOA->BSRR = GPIO_Pin_3    
#define DIR_FAN_2 	GPIOA->BRR  = GPIO_Pin_3

// void dianji_1(float m,int v);
// void dianji_2(float m,int v);
// void Step_Motor_Init();
void TIM1_Init(void); 
void TIM5_Init(void) ;
void TIM8_Init(void); 
void step_dir_init(void);
void step_motor_1(long num);
void step_motor_2(long num);
void step_motor_3(long num);
void unlimit_step_1(void);
void unlimit_step_2(void);
void unlimit_step_3(void);
void Step_Motor_Init(void);
