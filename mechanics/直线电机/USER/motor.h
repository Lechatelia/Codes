#include "stm32f10x.h"
#include  "stm32f10x_tim.h"

enum Motor_Dir     //�������״̬
 {
	 forward,
	 backward,
	 stop
 } ;
 extern enum Motor_Dir motor1_dir;
 
void GPIO_init_motor(void);
void PWM_init_motor( void); 
void setspeed_motor1(enum Motor_Dir dir,int speed);
