#include "stm32f10x.h"
                                  
 void init_exti(void);
 void my_NVIC_configuration(void);

#define KEY_1    GPIO_Pin_2    //����K_LEFT�ܽ�
#define KEY_2    GPIO_Pin_3    //����K_DOWN�ܽ�
#define KEY_3    GPIO_Pin_4   //����K_RIGHT�ܽ�
#define KEY_4    GPIO_Pin_0  //����KEY_UP�ܽ�
