#ifndef __encoder
#define __encoder
#include "stm32f10x.h"

#define ENC_PPR 512
#define period_clk 1774    //��������һȦ��Լ1780��Լ
extern struct Encoder_Stat Encoders;
	struct Encoder_Stat
{	
	int64_t Now;
	int64_t Last;
	int64_t Total;
	double 	Convert1;		//��תϵ��
	double	Convert2;		//��תϵ��
	int32_t dir;				//�ı�����ת�����������Ҫ�ķ�������1��������-1��
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
