#include "stm32f10x.h"

void USART3_DMA_Init(void);
void USART3_SendByte(u8 data);
void USART3_Sendstring (uint8_t *str ,int string_length);
