#ifndef _exti_H
#define _exti_H


#include "sys.h"

//使用位操作定义
#define K_UP PAin(0)
#define K_DOWN PEin(3)
#define K_LEFT PEin(2)
#define K_RIGHT PEin(4)
void KEY_Init(void);
void My_EXTI_Init(void);

#endif
