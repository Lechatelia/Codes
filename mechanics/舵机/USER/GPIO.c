#include"GPIO.h"
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;        //����GPIO��ʼ���ṹ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
		//����ģʽ���ã�Ĭ��Ϊ��������ģʽ��Ĭ��״̬Ϊ1
	
	//io������
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
}