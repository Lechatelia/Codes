#include "stm32f10x.h"

//步进电机分段参数
#define Distance_X_0 50000
#define Distance_X_1 200000
#define Distance_X_2 350000

#define Distance_Y_0 50000
#define Distance_Y_1 100000
#define Distance_Y_2 350000

//直流电机夹持时间
#define Time_start 30000
#define Time_Positive 20000
#define Time_Negative 20000
