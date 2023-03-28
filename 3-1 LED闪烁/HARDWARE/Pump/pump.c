#include "pump.h"
 
void Pump_Init(void)
{

 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOa.7
 GPIO_SetBits(GPIOA,GPIO_Pin_7);						 //PA.7 输出高

}

void pump_start()
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_7); // IN+
    GPIO_ResetBits(GPIOA, GPIO_Pin_8); // IN-,当两者都是低电平时电机动，两者相异则不动
}

void pump_stop()
{
    GPIO_SetBits(GPIOA, GPIO_Pin_7); // IN+
    GPIO_SetBits(GPIOA, GPIO_Pin_8); // IN-,当两者都是低电平时电机动，两者相异则不动
}
