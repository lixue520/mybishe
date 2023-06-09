#include "led.h"
void LED_Init(){
	GPIO_InitTypeDef GPIO_InitStructure; // 定义GPIO初始化结构体变量

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能GPIOC时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; // 设置引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 设置模式为推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置最大输出速率为50MHz
    GPIO_Init(GPIOC, &GPIO_InitStructure); // 初始化GPIOC口
}

// GPIO_SetBits(GPIOC, GPIO_Pin_13); // 点亮LED
// delay_ms(1000); // 延时1s
// GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 关闭LED
// delay_ms(1000); // 延时1s
