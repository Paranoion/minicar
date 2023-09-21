#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void){
	//初始化控制方向的两个引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	PWM_Init();
}
//设置电机速度
void Motor_SetLeftSpeed(int8_t Speed){
	if(Speed >= 0){
		//一个高电平一个低电平控制方向
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		PWM_SetCompare3(Speed);
	}else if(Speed == 0){
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
	else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		PWM_SetCompare3(-Speed);
	}
}
void Motor_SetRightSpeed(int8_t Speed){
	if(Speed >= 0){
		//一个高电平一个低电平控制方向
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
		PWM_SetCompare3(Speed);
	}else if(Speed == 0){
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}
	else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
		PWM_SetCompare3(-Speed);
	}
}
