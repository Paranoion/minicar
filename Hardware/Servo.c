#include "stm32f10x.h"                  // Device header
#include "PWMServo.h"

void Servo_Init(void){
	PWM_Init2();
}

void Servo_SetAngle(float Angle){	//设置舵机角度
	PWM_SetCompare3(Angle*2000/180+500);
}	
