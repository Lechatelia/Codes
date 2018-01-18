#include"GPIO.h"
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;        //定义GPIO初始化结构体
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
		//输入模式配置，默认为浮空输入模式，默认状态为1
	
	//io口配置
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}