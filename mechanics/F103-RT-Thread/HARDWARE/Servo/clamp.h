#include "stm32f10x.h"
#include  "stm32f10x_tim.h"
void clamp_init(int ccr);
void clamp_gpio_init(void);
void clamp_mode_init(int ccr);
void set_clamp_distance(int distance);
