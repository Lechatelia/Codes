/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "delay.h"

extern double duty;
extern	int CCR ;
void EXTI2_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	{
		delay_ms(100);	
		CCR=900;
		TIM_SetCompare2(TIM2,900);     //-90
			EXTI_ClearITPendingBit(EXTI_Line2);
		
  }
	
}

void EXTI15_10_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
	{
		delay_ms(100);
    CCR=2000;	
			TIM_SetCompare2(TIM2,2000);      // 45
      EXTI_ClearITPendingBit(EXTI_Line10);

  }
	
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET)
	{
				delay_ms(100);
		    CCR=1000;
				TIM_SetCompare2(TIM2,(int)(1000));    //-45
			  EXTI_ClearITPendingBit(EXTI_Line11);
		
  }
	
	 if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
			delay_ms(100);
		   CCR=2100;
			TIM_SetCompare2(TIM2,(int)(2100));  //90
		  EXTI_ClearITPendingBit(EXTI_Line12);
		
  }
	
}
 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
