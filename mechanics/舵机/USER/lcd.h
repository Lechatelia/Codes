#ifndef _LCD_H
#define _LCD_H

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_dma.h"
#include "system_stm32f10x.h"
#include "misc.h"

	#define LCD_LEN  160
	#define RxBufferSize 8
	#define TxBufferSize 12
	extern uint8_t LCD_Table[160];
	extern uint8_t LCD_Index;
	
	void all_lcd_init(void);
	void LCD_PushByte(uint8_t data);
	void LCD_PushString(char *data);	
	void LCD_SetXY(int x,int y);
	void LCD_WriteChar(char data);
	void LCD_WriteString(char *data);
	void LCD_WriteDouble(double data,int m);
	void LCD_WriteNum(double data);
	void LCD_WriteInt(int data);
	void LCD_WriteCharXY(char data,int x,int y);
	void LCD_WriteStringXY(char *data,int x,int y);
	void LCD_WriteIntXY(int data,int x,int y);
	void LCD_WriteDoubleXY(double data,int m,int x,int y);
	void LCD_WriteNumXY(double data,int x,int y);
	void LCD_Clear( void );
	void TaskLcdRefresh(void);
	void USART3_Init(void);
	void USART3_NVIC_Config(void);
	void USART3_GPIO_Config(void);
	void USART3_MODE_Config(void);
	void USART3_DMA_TX_Config(void);
	void RefreshDma(void);
	void UART3_SendByte(uint8_t ch);
	void TIM5_LCD_INIT(void);
	void Input_IntValue(int *address,char *name);//整型输入
	void Input_DoubleValue(double * address,char *name);//浮点输入
	void Input_FloatValue(float * address,char *name);//浮点输入
	
		void SysTick_init(void);
	void delay(__IO uint32_t nTime);
	
// 	void delay_init(u8 SYSCLK);
// void delay_ms(u16 nms);
// void delay_us(u32 nus);
#endif
