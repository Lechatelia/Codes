#ifndef _exti_h
#define _exti_h
#include "stm32f10x.h"
#include "misc.h"
#include "stm32f10x_exti.h"
void NVIC_Exti_Configuration(void);
void EXTI_PA2_config(void);
void EXTI2_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
#endif
