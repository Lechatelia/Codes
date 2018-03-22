#include "stm32f10x.h"

//步进电机分段参数
#define Distance_X_0 10000
#define Distance_X_1 200000
#define Distance_X_2 390000
#define Distance_X_3 565000

#define Distance_Y_0 10000
#define Distance_Y_1 50000
#define Distance_Y_2 500000
#define Distance_Y_3 120000

//直流电机夹持时间
#define Time_start 35000
#define Time_Positive 30000
#define Time_Negative 30000
