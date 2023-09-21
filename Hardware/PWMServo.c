#include "stm32f10x.h"                  // Device header

void PWM_Init2(void){
	//1.初始化TIM2通用定时器
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    
	
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_AF_PP;		//设置为复用推完输出引脚的控制权才能交给片上外设，PWM波形才可通过引脚输出
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	
	//2.选择时基单元的时钟
	TIM_InternalClockConfig(TIM3);		//设置TIM2时基单元由内部时钟驱动
	
	//3.配置时基单元
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	//计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1)= CK_PSC / (PSC + 1) / (ARR + 1)=72MHz/(PSC + 1) / (ARR + 1)
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;		//自动重装定时器ARR	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//预分频器PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//重复计数器，高级定时器才有，不需要使用，直接给0
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//4.初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);		//给结构体变量赋初始值
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;		//输出比较模式为PWM1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//输出比较极性为高极性，极性不翻转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		//输出状态，输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR捕获比较寄存器
	TIM_OC3Init(TIM3,&TIM_OCInitStructure);
	
	//启动定时器
	TIM_Cmd(TIM3,ENABLE);
}

void PWM_SetCompare3(uint16_t Compare){	//设置占空比
	TIM_SetCompare3(TIM3,Compare);		//更改通道1的CCR值来实现需要的效果
}

