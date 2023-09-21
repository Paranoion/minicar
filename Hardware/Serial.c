#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>



void Serial_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Structure;
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出，给USART的TX使用
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入，给RX使用
	GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Structure);
	
	//初始化USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;	//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;		//校验位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长
	USART_Init(USART1,&USART_InitStructure);
	
	//开启中断
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//开启RXNE标志位到NVIC的输出
	//配置NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//初始化NVIC的USART1通道
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

//发送一个数据
void Serial_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);		//判断发送数据寄存器是否为空
}

//发送一个数组
void Serial_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for(i = 0;i < Length;i ++){
		Serial_SendByte(Array[i]);
	}
}

//发送一个字符串
void Serial_SendString(char *String){
	uint8_t i;
	for(i = 0;String[i] != '\0';i	++){
		Serial_SendByte(String[i]);
	}
}

//求X的Y次方
uint32_t Serial_Pow(uint32_t X,uint32_t Y){
	uint32_t Result = 1;
	while(Y --){
		Result *= X;
	}
	return Result;
}

//以字符形式发送一个数字
void Serial_SendNumber(uint32_t Number,uint16_t Length){
	uint8_t i;
	for(i = 0;i < Length;i ++){
		Serial_SendByte(Number / Serial_Pow(10,Length - i - 1) % 10 + '0');
	}
}

//重写fputc()，重定向到串口
int fputc(int ch,FILE *f){
	Serial_SendByte(ch);
	return ch;
}

//封装sprintf()
void Serial_Printf(char *format,...){
	char String[100];
	va_list arg;	//参数列表
	va_start(arg,format);	//从format位置开始接收参数表，放到arg里面
	vsprintf(String,format,arg);
	va_end(arg);	//释放参数表
	Serial_SendString(String);
}

uint8_t Serial_GetRxFlag(void){
	if(Serial_RxFlag == 1){
		Serial_RxFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t Serial_GetRxData(void){
	return Serial_RxData;
}
