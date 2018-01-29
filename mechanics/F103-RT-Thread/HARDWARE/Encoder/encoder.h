#ifndef __encoder
#define __encoder
#include "stm32f10x.h"

#define ENC_PPR 512
#define period_clk 1774    //初步估计一圈大约1780大约
extern struct Encoder_Stat Encoders;
	struct Encoder_Stat
{	
	int64_t Now;
	int64_t Last;
	int64_t Total;
	double 	Convert1;		//正转系数
	double	Convert2;		//反转系数
	int32_t dir;				//改变正反转方向（如果是需要的方向则置1，否则置-1）
  int32_t  Distance;
};
void Encoder_Update(void);
void Encoder_Init(void);
void QEI_Init (void);
void TIM3_Int_Init(void);
void TIM4_GPIO_Config(void);
void RCC_Configuration(void);
void Encoder_Configuration(void);
#endif
