#include "stm32f10x.h"
                                  
 void init_exti(void);
 void my_NVIC_configuration(void);

#define KEY_1    GPIO_Pin_2    //定义K_LEFT管脚
#define KEY_2    GPIO_Pin_3    //定义K_DOWN管脚
#define KEY_3    GPIO_Pin_4   //定义K_RIGHT管脚
#define KEY_4    GPIO_Pin_0  //定义KEY_UP管脚
