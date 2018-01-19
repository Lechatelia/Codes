#include "all.h"
//Gps_State结构体个数

Gps_State Gps_List[GPS_STATCOUNT];
Pointfp64 Gps_Zero={0,0};
Pointfp64 Gps_assume;//利用摄像头或者激光等校正时试验用的变量

void GPS_Update(void)
{
	fp32	radianstep;						//弧度步进值
	fp32	theta_tmp;
	fp32	coeff_tmp;
	fp32	theta0a;
	fp32	theta0b;
	fp32	theta1a;
	fp32	theta1b;
//	static u8 mode = 1;
	
	Gps_List[1] = Gps_List[0];
	Gps_List[0].angle = Gyro_Angle_Total;//角度范围为-180~180

	//转换为弧度值并限制在-PI~PI
	Gps_List[0].radian = Gps_List[0].angle*PI/180.0f;
	
	//获得弧度步进值
	radianstep = Gps_List[0].radian-Gps_List[1].radian;
	while( radianstep > PI )
		radianstep -= 2*PI;
	while( radianstep < -PI )
		radianstep += 2*PI;
	
 	Gps_List[0].distance1 = Encoders[0].Distance;
 	Gps_List[0].distance2 = Encoders[1].Distance;
	
	Gps_List[0].distancestep1 = Gps_List[0].distance1 - Gps_List[1].distance1;
	Gps_List[0].distancestep2 = Gps_List[0].distance2 - Gps_List[1].distance2;
	
//	if ( Gps_List[0].position.y > 7250 && !mode ) mode = 1;
//	else if ( Gps_List[0].position.y < 5500 && mode ) mode = 0;
	/*if ( mode )*///if ( fabs(radianstep)> 0.00001f )
	if ( fabs(radianstep) > 0.0f )
	{
		theta_tmp = Encoders[0].radian-Encoders[1].radian;										//约等于PI/2
		coeff_tmp = radianstep / ( sin(theta_tmp-radianstep) + sin(theta_tmp+radianstep) - sin(theta_tmp)*2 );
//		coeff_tmp = radianstep / ( cos(radianstep) - 1 + 0.0001f ) / 2;	//theta_tmp近似为PI/2时可用此公式代替
		
		theta0a = Gps_List[1].radian+Encoders[0].radian;
		theta0b = Gps_List[0].radian+Encoders[0].radian;
		theta1a = Gps_List[1].radian+Encoders[1].radian;
		theta1b = Gps_List[0].radian+Encoders[1].radian;
		
		Gps_List[0].position.x += ((cos(theta1a)-cos(theta1b))*Gps_List[0].distancestep1 + (cos(theta0b)-cos(theta0a))*Gps_List[0].distancestep2)*coeff_tmp;
		Gps_List[0].position.y += ((sin(theta1a)-sin(theta1b))*Gps_List[0].distancestep1 + (sin(theta0b)-sin(theta0a))*Gps_List[0].distancestep2)*coeff_tmp;
	}
	else
	{
		Gps_List[0].distancestep1 -= Encoders[0].Radius*radianstep;
		Gps_List[0].distancestep2 -= Encoders[1].Radius*radianstep;
		theta0b = Gps_List[0].radian+Encoders[0].radian;
		theta1b = Gps_List[0].radian+Encoders[1].radian;
		Gps_List[0].position.x += ( sin(theta1b)*Gps_List[0].distancestep1 - sin(theta0b)*Gps_List[0].distancestep2) / sin(Encoders[1].radian-Encoders[0].radian);
		Gps_List[0].position.y += (-cos(theta1b)*Gps_List[0].distancestep1 + cos(theta0b)*Gps_List[0].distancestep2) / sin(Encoders[1].radian-Encoders[0].radian);
	}
}

void GPS_Clear(void)
{
	Gyro_Total=0;
// 	Gyro_Last=Gyro_Now;
	Encoder_Clear(0);
	Encoder_Clear(1);
	GPS_Init(Gps_Zero,0);
}
void GPS_Init(Pointfp64 position,fp64 radian)
{
	int i;
	Encoder_Clear(0);
	Encoder_Clear(1);
	for(i=0;i<GPS_STATCOUNT;i++)
	{
		Gps_List[i].distance1=0.0;
		Gps_List[i].distance2=0.0;
		Gps_List[i].distancestep1=0.0;
		Gps_List[i].distancestep2=0.0;
		Gps_List[i].position=position;
		Gps_List[i].angle=radian/PI*180.0;
		Gps_List[i].radian=radian;
		Gps_List[i].speed=0.0;
		Gps_List[i].acceleration=0.0;
		Gyro_Total=Gps_List[i].angle/0.0000001/Gyro_Convert1;
		Gyro_Now.count=0;
	}
}
