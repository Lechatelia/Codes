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
void DC_Motor_init_motor(void);
void setspeed_motor1(enum Motor_Dir dir,int speed ,long number);
void DC_Motor_positive( long number) ;
void DC_Motor_negative( long number) ;
void TIM7_delay_1ms(void) ;
void DC_Motor_reset( void);
