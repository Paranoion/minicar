#include "stm32f10x.h"                  // Device header

void Infrared_Init(void){	//红外模块
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_15;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
}
