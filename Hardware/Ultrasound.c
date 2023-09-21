#include "stm32f10x.h"                  // Device header
#include "Delay.h"

uint16_t Cnt;
uint16_t OverCnt;

void Ultrasound_Init(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//1。配置GPIO
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_12;		//trig引脚
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IPD;		//下拉输入
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_13;		//echo引脚
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Init_Structure);
	
	//2.选择时基单元的时钟
	TIM_InternalClockConfig(TIM4);		//设置TIM4来当做计数器
	
	//3.配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//一分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	//计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1)= CK_PSC / (PSC + 1) / (ARR + 1)=72MHz/(PSC + 1) / (ARR + 1)
	TIM_TimeBaseInitStructure.TIM_Period = 60000 - 1;		//自动重装定时器ARR	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//预分频器PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//重复计数器，高级定时器才有，不需要使用，直接给0
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
}

float Test_Distence(){	//测量距离
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	Delay_us(20);
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == RESET);
	TIM_Cmd(TIM4,ENABLE);	//超声波返回，打开定时器
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13) == SET);		//超声波返回会置echo引脚为高电平（连接在了Pin13）高电平持续时间就是超声波发送到返回的时间
	TIM_Cmd(TIM4,DISABLE);
	Cnt = TIM_GetCounter(TIM4);		//获取计数器的值
	float distance = (Cnt*1.0/10*0.34)/2;
	TIM4->CNT = 0;
	Delay_ms(100);
	return distance;
}
